# Pla de proves — Moviment

## Objectiu
Validar que la base:
1) avança recte, 2) gira de forma consistent, 3) frena i s’atura amb seguretat.

## Proves (ordre)
1. PWM manual (open-loop)
   - endavant 2 m, parar
   - enrere 2 m, parar
   - gir dreta 90°, parar
   - gir esquerra 90°, parar

2. Repetibilitat
   - fer 5 vegades “endavant 2 m” i mirar desviació lateral

3. Càrrega
   - repetir amb pes afegit (simular braços/bateria)

4. Seguretat
   - timeout de comunicació: si no rep comanda en X ms → stop motors


## Validació software (sense hardware)
- Compilació OK per Arduino Mega (Arduino IDE 2.3.7)
- Ús de memòria: ~5 KB flash, ~293 B RAM

