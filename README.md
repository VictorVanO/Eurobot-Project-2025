# Eurobot-Project-2025


![GitHub contributors](https://img.shields.io/github/contributors/VictorVanO/Eurobot-Project-2025?color=blue)
![GitHub issues](https://img.shields.io/github/issues/VictorVanO/Eurobot-Project-2025?color=orange)

# Table of contents

* [Project participants](#project-participants)
* [About the project](#question-about-the-project)
* [Getting started](#rocket-getting-started)
    * [Prerequesites](#exclamation-prerequesites)
* [Big Robot](#car-big-robot)
* [PAMI](#pami)
    * [Superstar](#star2-superstar)
    * [Groupie](#dancer-groupie)
* [Tree structure](#deciduous_tree-tree-structure)

# Project participants
- 20253 Van Olffen Victor
- 195030 Jardinet Louis
- 18194 Dufour Arthur
- 195105 Simpalingabo Ange
- 195357 Comor Edin
- 195052 Sahib Oussama

# :question: About the project
The purpose of this project is to participate in the [Eurobot Contest][eurobot] organised in 2025. The rules are available [here][eurobot-rules].

Our participation to this competition was necessary in order to validate the **Embedded Project** course given in 1st year of informatics/electronics at ECAM.

# :rocket: Getting started

This repository is a presentation of our robots and our strategies.

Anyone willing to replicate our robots should be able to do so using this repository.

Every file we used for our robots are listed here, aswell as the components and the schematics.

 Below is a photo of our robots (1 Big robot and 2 PAMI units):
 <p align="center">
    <img src="/images/Robots-Photo.jpg" alt="Photo Robots" width="600"/>
 </p>

## :exclamation: Prerequesites

To use our codes, you will need:
- [Arduino IDE][arduino] - To compile, debug and upload the .ino files into the arduino hardwares

You will also need to install these arduino libraries:
- LiquidCrystal_I2C
- Servo

# :car: Big Robot

## 🧩 Bloc Diagram
The Bloc diagram below shows the main components of the Big robot and how they are connected together.
![Bloc Diagram of the big robot.](/images/Bloc-Diagram-Big-Robot.png)

## :memo: FSM

The FSM diagram illustrates the different states of the Big robot and how it transitions between them during a match.
![Finite-State Machine of the big robot.](/images/FSM-Big-Robot-final.png)

## :wrench: Components
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
   
## Strategy
The Big Robot should be able to do 2 actions in order to score points : 
1. The robot should deposit the banner using the 2 arm, one for each π-shaped support.
2. The arms should rotate around the robot and be placed up front
3. The robot moves to take the first construction in front
4. The robot grips the construction, lifts it using the 2 arms and places it on the side
5. The robot makes a 180° turn
6. The robot moves to the second construction and lifts it using the arms
7. The robot places the second construction on the starting zone
8. The robot makes a 180° turn and goes to the final construction zone

## Schematic wiring

This is the schematic circuit of the big robot :
 
![Schematic wiring of the big robot](/images/Schematic_Wiring-Big-Robot.png)


## Grippers
The Big Robot is now equipped with 2 independent grippers, each controlled by a servo motor and mounted on a dedicated gripper support.
Each gripper holds one end of a tube, and each tube is attached to a banner holder shaped to fit the edge of the match table.
The banner itself measures 60 cm in length and 25 cm in height.

<p align="center">
  <img src="/images/Model-Banner-Big-Robot-1.png" alt="Model Banner Step 1" width="300" height="287"/>
  <img src="/images/Model-Banner-Big-Robot-2.png" alt="Model Banner Step 2" width="300"/>
</p>

During deployment, the robot releases the 1st tube, allowing the 1st banner holder to drop and clip into the table edge.
Then, the robot moves forward about 20 cm and releases the 2nd tube, completing the deployment of the banner.

In the ```3D-Models/big-robot``` folder, you’ll find the STL file for the gripper support. This part needs to be printed twice — once for each gripper:

| STL File            | Required Quantity |
|---------------------|-------------------|
| gripper-support.stl | 2                 |

# PAMI

## Components
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

# :star2: Superstar

## 🧩 Bloc Diagram
The Bloc diagram below shows the main components of the Superstar and how they are connected together :
![Bloc Diagram of the big robot.](/images/Bloc-Diagram-Superstar.png)


## :memo: FSM

![Finite-State Machine of the PAMI superstar.](/images/FSM-PAMI-Superstar.png)

## Sensors

- **Distance Sensor (Ultrasonic)**  
    Detects obstacles from a distance to enable automatic avoidance and spatial orientation.

## Strategy
In order to score points the Superstar should be able to : 
1. The robot climbs the ramp by moving straight for 6 seconds at full speed.
2. It then turns right for 1.5 seconds to align itself as close as possible to the edge of the platform to maximize scoring potential.
3. It drives forward briefly to stabilize or adjust its final position.
4. Enter PARTY_STATE to celebrate the arrival. In this state, the robot rotates its servo motor arms back and forth and blinks the LED lights to provide a visual and animated signal.
   
## Obstacle Avoidance
When an obstacle is detected by the distance sensor, the robot stop until there is no more obstacle. 

## Schematic wiring

This is the schematic circuit of the superstar :

![Schematic wiring of the Superstar robot](/images/Schematic_Superstar.png)

### Obstacle Avoidance
When an obstacle is detected by the distance sensor, the robot stop until there is no more obstacle. 

## Assembly 
- The two motors and encoders are already placed on the chassis
- Glue the breadboard to the chassis
- With four M3x20mm screws, attach the Arduino mount on the second plate and place the Arduino in it
- With four M3x15mm standoff screws, assemble the two plates
- With two M3x30mm screws fix the Ultrasonic sensor mount on the front of the Arduino mount and slide the sensors into the support
- With two M3x20mm standoff screws fix the Servo/Emergency button mount on the rear of the Arduino mount and slide motor and the button into the support
- With two M3 screws and nuts fix the battery support to the lower plate

# :dancer: Groupie

## 🧩 Bloc Diagram
The Bloc diagram below shows the main components of the Groupie and how they are connected together :
![Bloc Diagram of the big robot.](/images/Bloc-Diagram-Groupie.png)

## :memo: FSM

![Finite-State Machine of the PAMI groupie.](/images/FSM-PAMI-Groupie.png)

## Sensors

- **Infrared (IR) Sensors**  
    Used for line-following and distinguishing between black and white surfaces. Placed at the front of the robot, they allow it to follow a line on the ground or detect nearby obstacles.

- **Optical Fork Sensors (Encoders)**  
    Each wheel is equipped with a slotted disk attached to the motor shaft and an optical fork sensor that detects interruptions of the infrared beam caused by the slots. For every detected interruption, a pulse is generated on a microcontroller's digital input. This allows the robot to repeat the same movements on both wheels, improving synchronization and control.

- **Distance Sensor (Ultrasonic)**  
    Detects obstacles from a distance to enable automatic avoidance and spatial orientation.

## Strategy
In order to score points the groupie should be able to : 
1. Follow a line to reach the arrival zone. The robot follows a black line on a light surface using IR sensors. By analyzing the left/right sensor signals, it adjusts its trajectory to stay centered on the line.
2. Enter PARTY_STATE to celebrate the arrival. In this state, the robot rotates its servo motor arms back and forth and blinks the LED lights to provide a visual and animated signal.

## Schematic wiring

This is the schematic circuit of the groupie :

![Schematic wiring of the Groupie robot](/images/Schematic_Groupie.png)


## Obstacle Avoidance

When an obstacle is detected by the distance sensor, the robot interrupts line following and performs an avoidance maneuver based on a simple algorithm (stop, turn, go forwoard, turn).

## Assembly 
- Place the two motors on the first robot plate and assure that it is positioned on the appropriate holes. Mount the encoder wheels on the inside of the motors
- On the second plate, place the encoders on there dedicated mount 
- With two M3x15mm screws and nuts, fix the Infrared sensors mount at the front of the second plate and fix the sensors on the mount with two M3x15mm screws and nuts
- With four M3x20mm screws, attach the Arduino mount on the second plate and place the Arduino in it
- With four M3x15mm standoff screws, assemble the two red plates. Be sure to keep the motors well in place as they will fixed between the two plates and guide the encoder wheels trough the sensors
- With two M3x30mm screws fix the Ultrasonic sensor mount on the front of the Arduino mount and slide the sensors into the support
- With two M3x20mm standoff screws fix the Servo/Emergency button mount on the rear of the Arduino mount and slide motor and the button into the support

# :deciduous_tree: Tree Structure

```bash
Eurobot-Project-2025
    ├── LICENSE
    └── README.md
```

<!-- Links -->
[eurobot]: https://www.eurobot.org/
[eurobot-rules]: https://www.eurobot.org/eurobot-contest/eurobot-2025/
[arduino]: https://www.arduino.cc/en/software/
