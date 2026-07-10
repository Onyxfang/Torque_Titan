# Project Goals & Core Objectives

## Design Constraints & Success Criteria

To achieve a passing build, the project had to meet a strict set of parameters:
* **Dimensional Envelope:** Must fit entirely within a $13\text{cm} \times 13\text{cm}$ bounding box.
* **Mass Limit:** Must weigh less than or equal to $1500\text{g}$.
* **Autonomy:** Fully autonomous operation with visual indicators for start and countdown.
* **Sensor Constraints:** No camera allowed.
* **Budget:** Entirely within a $\$75$ limit, accompanied by a complete Bill of Materials (BOM) for open-source replication.

> ⚖️ **Mass Optimization Result:** Our final build came in at **1498g out of 1500g**. We intentionally maximized the allowable weight to get the highest possible downward force and tire traction on the field.

---

## Mechanical Design & CAD Challenges

When modeling the layout in CAD, I initiated the design using a strict $13\text{cm} \times 13\text{cm}$ baseplate footprint. Managing this tight space envelope proved to be our biggest hurdle due to our drivetrain choices. 

### 1. The Drivetrain Packaging Hurdle
To maximize pushing power, we selected high-torque motors that measured $65\text{mm}$ each, including the shaft. Mounting these two motors axially aligned in a **butt-to-butt configuration** meant the motor housings alone consumed the entire width of our allowable footprint ($65\text{mm} + 65\text{mm} = 130\text{mm}$ or $13\text{cm}$). 

* **The Solution:** To prevent the wheels from sticking out past the $13\text{cm}$ limit, I designed custom wheels that **reversed back over the motor shafts**, nesting the motor bodies inside the wheel hubs. 
* **Torque & Safety Mitigation:** Because the motors were tightly coupled end-to-end, the mounting system had to rigidly counteract motor torque. If the motor housings spun in their mounts, it posed a severe risk of tearing or short-circuiting the electrical terminals.

### 2. Chassis Dynamics & Tactics
Maximizing the front and back spacing was much more straightforward. I toyed with several geometric design options, ultimately implementing an aggressive front lip engineered to wedge underneath and lift an opposing robot or obstacle.
