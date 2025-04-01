# Eurobot-Project-2025


![GitHub contributors](https://img.shields.io/github/contributors/VictorVanO/Eurobot-Project-2025?color=blue)
![GitHub issues](https://img.shields.io/github/issues/VictorVanO/Eurobot-Project-2025?color=orange)

# Table of contents

* [Project participants](#project-participants)
* [About the project](#question-about-the-project)
* [Getting started](#rocket-getting-started)
    * [Prerequesites](#exclamation-prerequesites)
* [Tree structure](#deciduoustree-tree-structure)

## Project participants
- 20253 Van Olffen Victor
- 195030 Jardinet Louis
- 18194 Dufour Arthur
- 195105 Simpalingabo Ange
- 195357 Comor Edin
- 195052 Sahib Oussama

## :question: About the project
The purpose of this project is to participate in the [Eurobot Contest][eurobot] organised in 2025. The rules are available [here][eurobot-rules].

Every file we used for our robots are listed in this repository.

## :rocket: Getting started

### :exclamation: Prerequesites

### PAMI

#### Groupie

This is the FSM we used for our groupie:

![Finite-State Machine of the PAMI groupie.](/images/FSM-PAMI-Groupie.png)

#### Superstar

This is the FSM we used for our superstar:

![Finite-State Machine of the PAMI superstar.](/images/FSM-PAMI-Superstar.png)

### Big Robot : Finite State Machine

This is the FSM we used for our big robot:

![Finite-State Machine of the big robot.](/images/FSM-Big-Robot.png)

### Big Robot : Components
Here are the main components for our robot : 
- 2 x 9V rechargeable power supplies to separately supply the driver and the microcontroller
- 2 x Voltage regulators
- 1 x Cytron MDD10A Dual Motors Driver
- 1 x Arduino Mega 2560
- 8 x HC-SR04 Ultrasonic sensors
- 2 x 3D Printed Robotic Arms
- 1 x Aluminium 25x25 cm frale
- 2 x DC Motors with corresponding wheels
- 1 x breadboard
- 1 x Emergency Button
- 1 x Starting Magnetic Reed
   
### Big Robot : Strategy
The Big Robot should be able to do 2 actions in order to score points : 
1. The robot should deposit the banner using the 2 arm, one for each π-shaped support.
2. The arms should rotate around the robot and be placed up front
3. The robot moves to take the first construction in front
4. The robot grips the construction, lifts it using the 2 arms and places it on the side
5. The robot makes a 180° turn
6. The robot moves to the second construction and lifts it using the arms
7. The robot places the second construction on the starting zone
8. The robot makes a 180° turn and goes to the final construction zone

### PAMI : Compenents
Here are the main components for ours PAMI's : 
- 1 x Arduino Adafruits
- 2 x HC-SR04 Ultrasonic sensors
- 2 x Infrared sensors
- 2 x DC Motors with corresponding wheels
- 1 x breadboard
- 1 x Emergency Button
- 2 x Optical encoders
- 1 x Servomotor
- 1 x LED

# Strategy  

## Groupie : Sensors Used

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

### Obstacle Avoidance  
When an obstacle is detected by the distance sensor, the robot interrupts line following and performs an avoidance maneuver based on a simple algorithm (stop, turn, go forwoard, turn).

## Superstar : Sensors Used

- **Distance Sensor (Ultrasonic)**  
  Detects obstacles from a distance to enable automatic avoidance and spatial orientation.

---

## Overall Strategy

### Obstacle Avoidance  
When an obstacle is detected by the distance sensor, the robot stop until there is no more obstacle. 



## :deciduous_tree: Tree Structure

```bash
Eurobot-Project-2025
    ├── LICENSE
    └── README.md
```

<!-- Links -->
[eurobot]: https://www.eurobot.org/
[eurobot-rules]: https://www.eurobot.org/eurobot-contest/eurobot-2025/
