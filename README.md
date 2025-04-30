# Eurobot-Project-2025


![GitHub contributors](https://img.shields.io/github/contributors/VictorVanO/Eurobot-Project-2025?color=blue)
![GitHub issues](https://img.shields.io/github/issues/VictorVanO/Eurobot-Project-2025?color=orange)

# Table of contents

* [Project participants](#project-participants)
* [About the project](#question-about-the-project)
* [Getting started](#rocket-getting-started)
    * [Prerequesites](#exclamation-prerequesites)
* [Big Robot](#big-robot)
* [Superstar](#superstar)
* [Groupie](#groupie)
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

Our participation to this competition was necessary in order to validate the **Embedded Project** course given in 1st year of informatics/electronics at ECAM.

## :rocket: Getting started

This repository is a presentation of our robots and our strategies.

Anyone willing to replicate our robots should be able to do so using this repository.

Every file we used for our robots are listed here, aswell as the components and the schematics.

### :exclamation: Prerequesites

To use our codes, you will need:
- [Arduino IDE][arduino] - To compile, debug and upload 

Arduino libraries:
- LiquidCrystal_I2C
- Servo

## Big Robot

### FSM

The initial FSM for our big robot was looking like this:

![Finite-State Machine of the big robot.](/images/FSM-Big-Robot-init.png)

Due to a lack of time and components, our final FSM ended up like this:

![Finite-State Machine of the big robot.](/images/FSM-Big-Robot-final.png)

### Components
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
   
### Strategy
The Big Robot should be able to do 2 actions in order to score points : 
1. The robot should deposit the banner using the 2 arm, one for each π-shaped support.
2. The arms should rotate around the robot and be placed up front
3. The robot moves to take the first construction in front
4. The robot grips the construction, lifts it using the 2 arms and places it on the side
5. The robot makes a 180° turn
6. The robot moves to the second construction and lifts it using the arms
7. The robot places the second construction on the starting zone
8. The robot makes a 180° turn and goes to the final construction zone

## Superstar

### FSM

![Finite-State Machine of the PAMI superstar.](/images/FSM-PAMI-Superstar.png)

## Groupie

### FSM

![Finite-State Machine of the PAMI groupie.](/images/FSM-PAMI-Groupie.png)


## :deciduous_tree: Tree Structure

```bash
Eurobot-Project-2025
    ├── LICENSE
    └── README.md
```

<!-- Links -->
[eurobot]: https://www.eurobot.org/
[eurobot-rules]: https://www.eurobot.org/eurobot-contest/eurobot-2025/
[arduino]: https://www.arduino.cc/en/software/
