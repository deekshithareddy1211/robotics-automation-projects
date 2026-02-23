# Robotics Automation Projects (C++)

This repository contains two C++ programs focused on basic robotics and automation logic. The code is written in a way that can be adapted to common robotics platforms (Arduino / embedded C++ / simulation environments) depending on the hardware setup.

## Projects Included

### 1) Line Following Robot (`linefollowingrobot.cpp`)

**Goal:** Keep the robot moving along a line/path using sensor feedback.

**Concept Overview:**

* Reads input from line sensors (typically IR reflectance sensors).
* Decides whether to go **straight**, **turn left**, or **turn right**.
* Sends control signals to motors (via a motor driver) based on sensor readings.

**Typical Logic (High Level):**

* If the line is centered → move forward
* If the line shifts left → turn left
* If the line shifts right → turn right
* If no line detected → stop or search (depends on implementation)

---

### 2) Segregation System (`Segregation.cpp`)

**Goal:** Classify items into categories and trigger a corresponding action/output.

**Concept Overview:**

* Takes an input (sensor reading / rule / condition).
* Applies classification logic to decide a category.
* Triggers an output action (example: sorting direction, bin selection, message/logging).

**Note:** The classification rules can be mapped to different real-world sensors (color/metal/weight/camera) depending on the final hardware.

---

## How to Run (C++)

### Option A: Compile & Run with g++

If you’re using a normal PC compiler (Linux/Mac/WSL):

```bash
g++ linefollowingrobot.cpp -o linefollow
./linefollow

g++ Segregation.cpp -o segregate
./segregate
```

### Option B: Use on Arduino / Embedded

If you want to run this on Arduino:

* Convert inputs/outputs to match Arduino pin reads/writes.
* Replace console I/O with `Serial.print()` and hardware pin logic.

---

## Tech Stack

* C++
* Robotics control logic (sensor → decision → actuator)
* Rule-based classification (for segregation)

---

## Future Improvements

* Add PID control for smoother line following
* Add calibration step for sensor thresholds
* Expand segregation categories + add sensor integration
* Add simulation/demo videos and wiring diagrams
