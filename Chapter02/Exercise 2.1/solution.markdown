
### Condition
- animal safari game
- object types
  - creatures
  - plants
  - vehicles
  - players

---

### Monolithic Class Hierarchy Object Model

```mermaid
graph TD
  actor[Actor] --> creatures[Creatures];
  actor --> vehicles[Vehicles];
  actor --> player[Player];

  creatures --> plants[Plants];
  creatures --> animal[Animals];

  vehicles --> cycle[Cycles];
  vehicles --> auto[Automobiles];
```

### Component Object Model

```mermaid
graph LR
  Player[Player] --> Player01[DrawComponent];
  Player --> Player02[TransformComponent];
  Player --> Player03[CollisionComponent];
  Player --> Player04[...];

  Vehicle[Vehicle] --> Vehicle01[DrawComponent];
  Vehicle --> Vehicle02[TransformComponent];
  Vehicle --> Vehicle03[CollisionComponent];
  Vehicle --> Vehicle04[...];

  Plant[Plant] --> Plant01[DrawComponent];
  Plant --> Plant02[CollisionComponent];
  Plant --> Plant03[...];

  Animal[Animal] --> Animal01[DrawComponent];
  Animal --> Animal02[TransformComponent];
  Animal --> Animal03[CollisionComponent];
  Animal --> Animal04[...];
```

---

### Which is more beneficial ?
- it depends on the game specification
- classifying onto objects isn't simple, `Component Object Model` would be better
- in this condition, `Monolithic Class Hierarchy Object Model` is better
  - due to ease of classification
