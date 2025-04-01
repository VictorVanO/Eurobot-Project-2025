# Eurobot Project 2025

## Sensor Strategy

Our robot uses a combination of carefully selected sensors to perceive its environment and make real-time decisions. The strategy relies on the complementarity of different types of sensors to ensure robustness, accuracy, and responsiveness.

---

## Sensors Used

- **Infrared (IR) Sensors**  
  Used for line-following and distinguishing between black and white surfaces. Placed at the front of the robot, they allow it to follow a line on the ground or detect nearby obstacles.

- **Optical Fork Sensors (Encoders)**  
  Each wheel is equipped with a slotted disk attached to the motor shaft and an optical fork sensor that detects interruptions of the infrared beam caused by the slots. For every detected interruption, a pulse is generated on a microcontroller's digital input. This allows the robot to repeat the same movements on both wheels, improving synchronization and control.

- **Distance Sensor (Ultrasonic)**  
  Detects obstacles from a distance to enable automatic avoidance and spatial orientation.

---

## Overall Strategy

### Line Following  
The robot follows a black line on a light surface using the IR sensors. By analyzing the left/right sensor signals, it adjusts its trajectory to stay centered on the line.

### ➤ Obstacle Avoidance  
When an obstacle is detected by the distance sensor, the robot interrupts line following and performs an avoidance maneuver based on a simple algorithm (stop, turn, go forwoard, turn).


### ➤ Contextual Decision-Making  
Data fusion from all sensors allows the robot to dynamically adapt to its environment using a hierarchical decision-making logic:  
**Obstacle > Line > Precise Rotation**

---

*This sensor strategy ensures our robot can operate autonomously, adapt to changing environments, and achieve optimal task execution during Eurobot 2025.*

