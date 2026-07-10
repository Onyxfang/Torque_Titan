# Torque Titan: The Project Lifecycle

Welcome to the engineering history of the Torque Titan. This page tracks the design, prototyping, and hardware iteration of an autonomous sumobot built under a strict $13\text{cm} \times 13\text{cm}$ footprint restriction and a $75 budget limit. 

---

## Chapter 1: The Drawn Boundary & Scavenged Parts

Our first challenge was navigating the school's procurement timeline. While waiting for parts to be ordered and approved, we needed to validate our physical envelope immediately to ensure we wouldn't fall behind schedule.

### 📐 Step 1: Initial Sizing & Manual Layout
Before opening CAD, I drew a literal $13\text{cm} \times 13\text{cm}$ square on a piece of paper on the workbench. Under our professor's supervision, I scavenged old wheels and parts from the lab storeroom to work backward from what we had on hand. I placed the wheels inside the drawn box, verifying that the back radius of the wheels ended exactly flush with the rear edge of the boundary line to maximize our wheelbase.
![Initial Sizing](Torque_Titan%20Initial%20sizing.jpg)

---

## Chapter 2: Silicon Architecture

With a basic physical footprint in mind, we shifted to evaluating our core electronics stack to understand our control capabilities before building a chassis.

### 🧠 Step 2: ESP32 Pin Mapping & Capabilities
We began mapping out the pin assignments for the dual-core ESP32 to establish our sensor inputs and motor control outputs. At this stage, we focused heavily on understanding the microcontroller's direct capabilities for power input and output, ensuring we had the proper baseline configuration established before connecting our active hardware components.
![Beginning of Wiring](Torque_Titan_Beginning%20of%20Wiring.jpg)

---

## Chapter 3: Low-Fidelity Prototyping & Power Diagnostics

To see our electrical architecture interact with the mechanical drivetrain without wasting budget, we built a zero-cost physical test bench.

### 📦 Step 3: Cardboard Layout Validation & Motor Driver Diagnostics
We fabricated a rapid prototype entirely out of cardboard to mount the scavenged yellow TT motors and the ESP32. This allowed us to run our early control firmware and observe the drivetrain spin under its own power for the first time. However, during these initial bench tests with our first motor driver, we diagnosed a massive drop in usable voltage across the board. This critical bottleneck proved that our initial electrical architecture couldn't efficiently deliver power under load, forcing us to pivot and research alternative methods.
![Initial Cardboard Design](Torque_Titan_Initial_Design_Cardboard.jpg)

---

## Chapter 4: Drivetrain Scaling & Mechanical Clearance

Transitioning into higher fidelity prototyping meant refining our mechanical components to fix the flaws identified during the first cardboard phase, still working entirely by hand.

### 🛞 Step 4: Wheel Sizing & Material Limitations
While the large diameter of our scavenged wheels gave us a great baseline, initial testing revealed that the hard material lacked the coefficient of friction required for high-traction sumo matches. We utilized this phase to model the spatial layout for custom wheel sizing while planning a future pivot toward alternative poured materials.
![Wheel Sizing](Torque_Titan_Wheel_Sizing.jpg)

### ⚙️ Step 5: Motor Clearance & Scaling
Using standard L-shaped yellow TT motors, we evaluated side-by-side clearances within the chassis envelope. Because of their right-angle design, these motors naturally tucked inward lengthwise, giving us comfortable lateral clearances within the $13\text{cm}$ limit and providing a baseline to plan future upgrades to high-power, straight-profile 25L metal gearmotors.
![Initial Motor Testing](Torque_Titan_Initial_Motor_Testing.jpg)

### 🛠️ Step 6: Hand-Cut Cardboard & Hardware Assembly
Before moving to any digital software, I fabricated the first structural geometric cage completely by hand. Using heavy cardboard and real metal hardware, I physically cut and fastened the structural paths together. This physical assembly wrapped around the components, transforming our loose collection of parts into a single, unified manual test frame to protect our electronics.
![First Chassis Concept](Torque_Titan_First_Chassis_Concept.jpg)

---

## Chapter 5: Fabrication & Multi-Layer Material Scavenging

As the structure evolved, we balanced manual manufacturing constraints with creative material re-use to increase our vertical space.

### 🖨️ Step 7: Manufacturing Limits
We translated our manual dimensions forward under the philosophy of *"Measure three times, print once."* However, real-world manufacturing introduced a steep learning curve: I mismeasured the physical tolerances for our sensor arrays, resulting in placement errors that required frame modifications. This taught us invaluable lessons about tolerancing for hardware integration.
![Printing Phase](Torque_Titan_Measure_Three_Times_Print_Once.jpg)

### 🏗️ Step 8: Scavenged Material Assembly & Layer Validation
Rather than relying completely on custom prints, we added structural complexity by integrating a scavenged piece of 3D-printed material to serve as our second layer. We subjected this multi-layer hybrid assembly to physical load and stability evaluations to ensure the combination of hand-built framing and scavenged components could withstand field stress without flexing.
![Material Testing](Torque_Titan_Material_Testing_for_Stability.jpg)

---

## Chapter 6: Sensor Mapping & Spatial Strategy

Our final phase focused on mapping out our sensory boundaries to ensure the robot could sense its surroundings accurately during competition.

### 👁️ Step 9: Cardboard Sensor Positioning Mockup
To build an effective offensive capability, we had to ensure our sensors would never accidentally hit physical obstacles or structural ring blocks during an exchange. We utilized a precise piece of hand-cut cardboard to mockup exactly where we wanted to mount the sensor arrays. This physical template allowed us to test clearances and calculate exact lines of sight before committing to a final, permanent installation.
![Front Scoop Integration](Torque_Titan_Birth_of_Front_Scoop.jpg)
