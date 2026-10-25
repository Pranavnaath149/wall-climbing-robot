# Vertically Wall-Climbing Glass Cleaning Robot 🤖

An Arduino-based robotic prototype designed to adhere to vertical glass facades using thrust-driven aerodynamic negative pressure, controlled wirelessly via Bluetooth.

---

## 🛠️ Hardware Architecture & Components

* **Microcontroller:** Arduino Uno R3 (ATmega328P)
* **Adhesion System:** 
  * A2212 2450KV Brushless DC (BLDC) Motor
  * 1045 (10-inch diameter, 4.5 pitch) Propeller
  * Simonk 30A Electronic Speed Controller (ESC)
* **Locomotion System:** 
  * 4x N20 Geared DC Motors (60 RPM)
  * L298N Dual H-Bridge Motor Driver
* **Sensing & Feedback:** 
  * HC-SR04 Ultrasonic Sensor (Obstacle detection)
  * Active Buzzer & Status LEDs
* **Wireless Control:** HC-05 Bluetooth Module (UART Serial interface)
* **Power Management:** 12V 3S 30C 1300mAh LiPo Battery distributed via Matek PDB-XT60
* **Chassis:** Custom plus-shaped frame fabricated with lightweight rigid Forex sheet

---

## 📐 Adhesion & Physics Calculations

To determine the minimum normal suction force (Fs) required to prevent the robot from slipping down vertical surfaces:

**Formula:**
> **Fs = (m × g) / μ**

**Parameters:**
* **Robot Mass (m):** 1.2 kg
* **Gravitational Acceleration (g):** 9.81 m/s²
* **Friction Coefficient (μ):** 0.6 (rubber tires on clean glass)

**Calculation:**
* Fs = (1.2 kg × 9.81 m/s²) / 0.6
* Fs = 11.772 / 0.6
* **Fs = 19.62 N ≈ 2.0 kgf**

The BLDC motor and propeller assembly must generate at least **19.62 N** of inward aerodynamic thrust / downward airflow to maintain vertical equilibrium against gravity.
## 💻 Control Logic & Pin Mapping

| Peripheral | Arduino Pin | Function |
| :--- | :--- | :--- |
| **ESC (BLDC Motor)** | Pin 3 | PWM speed signal (1000 µs - 1500 µs) |
| **Ultrasonic Trig** | Pin 4 | Trigger pulse output |
| **Ultrasonic Echo** | Pin 5 | Echo duration input |
| **L298N IN1, IN2** | Pin 6, Pin 7 | Left side drive direction |
| **L298N IN3, IN4** | Pin 8, Pin 9 | Right side drive direction |
| **Buzzer** | Pin 11 | PWM acoustic frequency warning |
| **Green LED / Red LED** | A0 / A1 | Visual proximity warnings |
| **HC-05 RX / TX** | Pin 1 / Pin 2 | Bluetooth serial commands |

---

## 🔍 Prototype Limitations & Testing Observations

During prototype vertical testing:
1. **Adhesion Threshold:** The 2450KV motor with an open 1045 propeller produced marginal suction near the 19.62 N threshold, making adhesion sensitive to small surface variations and air leaks.
2. **Thermal Dissipation:** Continuous high-RPM operation caused significant heat buildup in the ESC and BLDC motor coils, leading to thermal throttling after short runtimes.
3. **Power Draw:** Simultaneous high-discharge load from the BLDC suction and N20 drive motors depleted the 1300mAh 3S LiPo quickly.

---

## 🚀 Future Scope & Improvements

* **Ducting & Vacuum Shroud:** Implement a closed aerodynamic skirt around the propeller blades to drastically increase vacuum suction pressure with lower motor RPM.
* **Dual-Propeller Setup:** Counter-rotating dual motors to neutralize propeller torque twist and double normal adhesion force.
* **Chassis Optimization:** Replace Forex sheet with CNC-milled carbon fiber to reduce total weight below 800g.
* **Autonomous Navigation:** Integrate an IMU/gyroscope (MPU6050) for vertical self-leveling and automated grid-pattern window coverage.
