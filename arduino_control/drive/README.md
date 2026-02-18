# Drive control (Arduino)

## Objectiu
Controlar moviment base diferencial via comandes sèrie.

## Protocol (proposta mínima)
Comanda:
M <pwm_left> <pwm_right>\n

On pwm_left i pwm_right són enters entre -255 i 255.

Exemples:
- M 120 120  -> endavant
- M -120 -120 -> enrere
- M 120 -120 -> gir sobre eix
- M 0 0 -> stop

## Seguretat
Si no arriba cap comanda en 300 ms -> stop motors.
