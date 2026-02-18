# Càlcul inicial de parell i potència (estimació)

## Força total requerida
Assumim:
- resistència rodament: F_rr = Crr * m * g
- pendent: F_slope = m * g * sin(θ)
- acceleració: F_acc = m * a

Força total:
F = F_rr + F_slope + F_acc

## Parell per roda (base diferencial)
Parell total a la roda:
T_total = F * R

Parell per roda motriu:
T_wheel = (T_total / n) * SF

## Potència aproximada
P = F * v

## Pas a parell del motor (si hi ha reductora)
T_motor ≈ T_wheel / (G * η)
on G és la relació de reducció.
