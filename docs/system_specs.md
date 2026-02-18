# Especificacions del sistema (Moviment)

## Objectius de moviment
- Tipus de base: diferencial (2 rodes motrius + 1-2 rodes bogie/castor)
- Mode control: open-loop inicial (PWM), opcional closed-loop (encoders)

## Variables de disseny (a definir)
| Paràmetre | Símbol | Valor inicial | Notes |
|---|---:|---:|---|
| Massa total robot | m [kg] | TBD | estimació per blocs |
| Diàmetre roda | D [m] | TBD | afecta parell i velocitat |
| Radi roda | R [m] | TBD | R = D/2 |
| Velocitat màxima | v_max [m/s] | TBD | segur en entorn aula |
| Acceleració desitjada | a [m/s²] | TBD | confort i estabilitat |
| Pendent màxim | θ [°] | TBD | rampes/llindars |
| Coef. resistència rodament | Crr [-] | 0.02 (assumpció) | depèn rodes i terra |
| Nº rodes motrius | n | 2 | habitual |
| Factor seguretat | SF [-] | 1.5–2.0 | recomanat |
| Tensió bateria | V [V] | TBD | 12V/24V típic |
| Autonomia objectiu | t [h] | TBD | ex: 1–2 h |
| Eficiència transmissió | η [-] | 0.7–0.85 | motor+driver+reductora |

## Notes
- Primer prototip: prioritzar estabilitat i control sobre velocitat.
- Afegir encoders millora molt el moviment recte i el gir.

