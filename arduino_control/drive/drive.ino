/*
  drive.ino — Control base diferencial (Arduino Mega)

  Protocol sèrie (ASCII):
    M <pwmL> <pwmR>\n    on pwmL, pwmR ∈ [-255, 255]
    S\n                 STOP immediat
    H\n                 Ajuda
    V\n                 Versió

  Exemple:
    M 120 120   -> endavant
    M -120 -120 -> enrere
    M 120 -120  -> gir sobre eix
    M 0 0       -> stop

  Seguretat:
    - Timeout: si no arriba cap comanda en CMD_TIMEOUT_MS -> STOP motors
*/

#include <Arduino.h>

// ========= CONFIGURACIÓ PINOUT (Arduino Mega) =========
// Pensat per a drivers tipus: DIR + PWM per motor.
// CANVIA AQUESTS PINS si el teu cablejat/driver és diferent.

static const uint8_t L_PWM_PIN = 5;   // Pin PWM (Mega: 2-13, 44-46)
static const uint8_t L_DIR_PIN = 22;  // Pin digital
static const uint8_t R_PWM_PIN = 6;   // Pin PWM
static const uint8_t R_DIR_PIN = 23;  // Pin digital

// Si un motor gira al revés, posa true
static const bool L_INVERT = false;
static const bool R_INVERT = false;

// ========= PARÀMETRES =========
static const uint32_t BAUDRATE = 115200;
static const uint32_t CMD_TIMEOUT_MS = 300;

// Si vols limitar cops de parell (suavitzar), activa-ho:
static const bool ENABLE_RAMP = true;
static const int RAMP_STEP = 10;         // increment màxim per loop
static const uint32_t RAMP_DT_MS = 15;   // interval del ramp

// ========= ESTAT =========
static int targetL = 0, targetR = 0;
static int currentL = 0, currentR = 0;
static uint32_t lastCmdMs = 0;
static uint32_t lastRampMs = 0;

// Buffer de línia
static char lineBuf[64];
static uint8_t lineLen = 0;

// ========= UTILS =========
static int clampPwm(int v) {
  if (v > 255) return 255;
  if (v < -255) return -255;
  return v;
}

static void applyMotor(uint8_t pwmPin, uint8_t dirPin, bool invert, int pwm) {
  pwm = clampPwm(pwm);
  if (invert) pwm = -pwm;

  if (pwm == 0) {
    analogWrite(pwmPin, 0);
    return;
  }

  const bool dir = (pwm >= 0);
  digitalWrite(dirPin, dir ? HIGH : LOW);
  analogWrite(pwmPin, abs(pwm));
}

static void setMotorsNow(int pwmL, int pwmR) {
  pwmL = clampPwm(pwmL);
  pwmR = clampPwm(pwmR);

  currentL = pwmL;
  currentR = pwmR;

  applyMotor(L_PWM_PIN, L_DIR_PIN, L_INVERT, currentL);
  applyMotor(R_PWM_PIN, R_DIR_PIN, R_INVERT, currentR);
}

static void stopMotors(const char* reason) {
  targetL = 0; targetR = 0;
  setMotorsNow(0, 0);
  Serial.print(F("STOP"));
  if (reason && reason[0]) {
    Serial.print(F(" ("));
    Serial.print(reason);
    Serial.print(F(")"));
  }
  Serial.println();
}

static void printHelp() {
  Serial.println(F("Comandes:"));
  Serial.println(F("  M <pwmL> <pwmR>   (pwm en [-255,255])"));
  Serial.println(F("  S                STOP"));
  Serial.println(F("  V                Versio"));
  Serial.println(F("  H                Ajuda"));
}

static void printVersion() {
  Serial.println(F("drive.ino v1.0 (Mega)"));
}

// Parseja una línia completa
static void handleLine(char* s) {
  // Treu espais inicials
  while (*s == ' ' || *s == '\t') s++;
  if (*s == '\0') return;

  const char cmd = s[0];

  if (cmd == 'H' || cmd == 'h') {
    printHelp();
    return;
  }
  if (cmd == 'V' || cmd == 'v') {
    printVersion();
    return;
  }
  if (cmd == 'S' || cmd == 's') {
    lastCmdMs = millis();
    stopMotors("manual");
    return;
  }

  if (cmd == 'M' || cmd == 'm') {
    // Esperem: M <int> <int>
    // Fem parse amb strtok per ser lleuger
    char* tok = strtok(s, " \t");
    tok = strtok(NULL, " \t");
    if (!tok) { Serial.println(F("ERR: falta pwmL")); return; }
    int pwmL = atoi(tok);

    tok = strtok(NULL, " \t");
    if (!tok) { Serial.println(F("ERR: falta pwmR")); return; }
    int pwmR = atoi(tok);

    pwmL = clampPwm(pwmL);
    pwmR = clampPwm(pwmR);

    targetL = pwmL;
    targetR = pwmR;
    lastCmdMs = millis();

    Serial.print(F("ACK M "));
    Serial.print(targetL);
    Serial.print(' ');
    Serial.println(targetR);
    return;
  }

  Serial.print(F("ERR: comanda desconeguda: "));
  Serial.println(cmd);
}

// Llegeix sèrie línia a línia (\n o \r\n)
static void pollSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r') continue;

    if (c == '\n') {
      lineBuf[lineLen] = '\0';
      handleLine(lineBuf);
      lineLen = 0;
      continue;
    }

    if (lineLen < sizeof(lineBuf) - 1) {
      lineBuf[lineLen++] = c;
    } else {
      // Overflow -> reset
      lineLen = 0;
      Serial.println(F("ERR: linia massa llarga"));
    }
  }
}

// Ramp suau cap a target
static void updateRamp() {
  if (!ENABLE_RAMP) {
    if (currentL != targetL || currentR != targetR) {
      setMotorsNow(targetL, targetR);
    }
    return;
  }

  uint32_t now = millis();
  if (now - lastRampMs < RAMP_DT_MS) return;
  lastRampMs = now;

  auto stepToward = [](int cur, int tgt) -> int {
    if (cur == tgt) return cur;
    int d = tgt - cur;
    if (d > RAMP_STEP) d = RAMP_STEP;
    else if (d < -RAMP_STEP) d = -RAMP_STEP;
    return cur + d;
  };

  int newL = stepToward(currentL, targetL);
  int newR = stepToward(currentR, targetR);

  if (newL != currentL || newR != currentR) {
    setMotorsNow(newL, newR);
  }
}

// Timeout de seguretat
static void safetyTimeout() {
  uint32_t now = millis();
  if (now - lastCmdMs > CMD_TIMEOUT_MS) {
    // Evita spam de STOP: només si realment està movent-se
    if (currentL != 0 || currentR != 0 || targetL != 0 || targetR != 0) {
      stopMotors("timeout");
    }
  }
}

void setup() {
  pinMode(L_PWM_PIN, OUTPUT);
  pinMode(L_DIR_PIN, OUTPUT);
  pinMode(R_PWM_PIN, OUTPUT);
  pinMode(R_DIR_PIN, OUTPUT);

  Serial.begin(BAUDRATE);
  delay(200);

  lastCmdMs = millis();
  lastRampMs = millis();
  stopMotors("boot");

  Serial.println(F("Drive control ready (Mega). Escriu H per ajuda."));
}

void loop() {
  pollSerial();
  safetyTimeout();
  updateRamp();
}

