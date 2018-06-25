# Problem #1
### Condition
- $\vec{a} = (2, 4)$
- $\vec{b} = (3, 5)$
- $s = 2$

### Calculation
- $\vec{a} + \vec{b}$
  - $(2 + 4, 3 + 5) = (5, 9)$
- $s \cdot \vec{a}$
  - $(2 \cdot 2, 2 \cdot 4) = (4, 8)$
- $\vec{a} \cdot \vec{b}$
  - $2 \cdot 3 + 4 \cdot 5 = 26$

---

# Problem #2
### Condition
@import "triangle.png"
- $A = (-1, 1)$
- $B = (2, 4)$
- $C = (3, 3)$

### Calculation
- $\vec{AB} = B - A$
  - $(2 + 1, 4 - 1) = (3, 3)$
- $|\vec{AB}|$
  - $\sqrt{3^2 + 3^2} = \sqrt{18}$
- $\vec{AC} = C - A$
  - $(3 + 1, 3 - 1) = (4, 2)$
- $|\vec{AC}|$
  - $\sqrt{4^2 + 2^2} = \sqrt{20}$
- $\theta = arccos(\dfrac{\vec{AB} \cdot \vec{AC}}{|\vec{AB}| \cdot |\vec{AC}|})$
  - $arccos(\dfrac{3 \cdot 4 + 3 \cdot 2}{\sqrt{18} \cdot \sqrt{20}}) = arccos(\dfrac{18}{6 \cdot \sqrt{10}}) \approx 18.4349\degree$

---

# Problem #3
### Condition
- **Arrow** → $(1, 0)$
- **Player** → $(4, 0)$
- **Object** → $(5, 6)$

### Calculation
- `(a)` What is the unit vector from the player's initial position to the new waypoint ?
  - $\vec{x} = (5, 6) - (4, 0) = (1, 6)$
  - $\vec{n} = \dfrac{\vec{x}}{|\vec{x}|} = \dfrac{(1, 6)}{\sqrt{37}} = (\dfrac{1}{\sqrt{37}}, \dfrac{6}{\sqrt{37}})$
- `(b)` Calculate the angle of rotation between the initial arrow direction and the vector calculated in part `(a)`
  - $\theta = arccos(\dfrac{(1, 0) \cdot (\dfrac{1}{\sqrt{37}}, \dfrac{6}{\sqrt{37}})}{1 \cdot 1}) = arccos(\dfrac{1}{\sqrt{37}}) \approx 80.5376\degree$
- `(c)` Calculate the vector perpendicular to the plane created by the initial arrow direction and the vector calculated in part `(a)`
  - $\vec{y} = (a_y \cdot b_z - a_z \cdot b_y, a_z \cdot b_x - a_x \cdot b_z, a_x \cdot b_y - a_y \cdot b_x)$
  - $\vec{y} = (1, 0, 0) \times (\dfrac{1}{\sqrt{37}}, \dfrac{6}{\sqrt{37}}, 0) = (0, 0, \dfrac{6}{\sqrt{37}})$
