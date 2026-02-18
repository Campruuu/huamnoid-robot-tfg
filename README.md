# huamnoid-robot-tfg
# Robot Humanoide amb Base Roda — TFG

Projecte de Treball de Final de Grau centrat en el desenvolupament d’un robot humanoide amb base amb rodes orientat a la interacció en entorns educatius.  

El sistema integra control de baix nivell (Arduino) amb lògica d’alt nivell i processament intel·ligent (Raspberry Pi).

---

## Objectius del projecte

- Desenvolupar una base mòbil estable (endavant, enrere, gir)
- Implementar un sistema d’interacció per veu (STT / TTS)
- Integrar feedback visual mitjançant LEDs (estats i emocions)
- Definir una arquitectura modular hardware–software
- Documentar el procés amb metodologia d’enginyeria

---

## Arquitectura del sistema

### Raspberry Pi (nivell alt)
- Lògica de comportament (FSM – Finite State Machine)
- Processament de veu (Speech-to-Text i Text-to-Speech)
- Comunicació sèrie amb Arduino
- Gestió d’estats del robot

### Arduino (nivell baix)
- Control de motors (PWM)
- Control de servos o steppers
- Lectura de sensors
- Funcions de seguretat (timeouts, aturada d’emergència)

---

## Estructura del repositori

- `arduino_control/` → Firmware de control de motors i actuadors
- `raspberry_brain/` → Lògica principal en Python
- `hardware/` → Disseny CAD, càlculs mecànics i esquemes electrònics
- `docs/` → Diagrames, roadmap, proves i documentació tècnica

---

## Estat actual

- [x] Inicialització del repositori
- [x] Definició d’estructura modular
- [ ] Definició del BOM (motors, drivers, bateria, sensors)
- [ ] Protocol de comunicació Raspberry ⇄ Arduino
- [ ] Primera prova de moviment
- [ ] Primera prova de sistema de veu

---

## Roadmap tècnic (curt termini)

1. Selecció de motors i sistema d’alimentació (considerant pes, parell i autonomia)
2. Implementació de control bàsic de motors a Arduino
3. Disseny del protocol de comunicació sèrie
4. Implementació d’una FSM amb estats:  
   `IDLE`, `MOVE`, `LISTEN`, `SPEAK`, `ERROR`
5. Documentació de proves i resultats

---

## Autor

Pol Camprubí  
Estudiant d’Enginyeria Mecànica  
Projecte TFG orientat a robòtica i sistemes intel·ligents
