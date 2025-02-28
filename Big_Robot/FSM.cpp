#include "FSM.h"

FSM::FSM() : state(INIT), startTime(0), obstacle_treshold(5) {
    lcd = new LCD();
}

FSM::~FSM() {
    delete lcd;
}

void FSM::init() {
    initMotors();
    initUltrasonic();
    lcd->init();
}

void FSM::run() {
    handleState();
}

bool FSM::isObstacleDetected() {
    // Get ultrasonics' distances and check for obstacle below 10cm
    for (int i = 0; i < NUM_ULTRASONIC; i++) {
        float distance = readDistance(i);
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(distance);
        Serial.print(" cm.");
        Serial.println();

        if (distance > 0 && distance <= obstacle_treshold) {
            return true; // Obstacle detected
        }
    }
    return false; // No obstacle detected
}

void FSM::handleState() {
    unsigned long currentTime = millis();
    bool secondIsBuilt = false;

    // Check for obstacle
    if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
        previousState = state;  // Save current state before pausing
        state = PAUSE;
        Serial.println("Obstacle detected !!! Switching to PAUSE state.");
        return;  // Stop execution and enter PAUSE state
    }

    switch (state) {
        // Init state: Start timer
        case INIT:
            lcd->clear();
            lcd->printLine(0, "INIT LCD");
            lcd->printLine(1, "Starting robot...");

            if (startTime == 0) startTime = millis();

            // Start by moving arms. (For the tests, we start by moving in MOVE_TO_FIRST state)
            state = MOVE_TO_FIRST;
            break;

        // Move Arms state: Move the arms to fully extend the banner
        case MOVE_ARMS:
            // When arms are fully extended, change state to open hands
            state = OPEN_HANDS;
            // If arms were fully extended, move them back to initial extension width. Then start moving
            state = MOVE_TO_FIRST;
            break;
            
        // Open Hands state: Open hands to release the banner on the table & move the arms back to normal extension
        case OPEN_HANDS:
            state = MOVE_ARMS;
            break;
         
        // Move To First state: Hardcoded path to the first materials
        case MOVE_TO_FIRST:
            lcd->clear();
            lcd->printLine(0, "Moving to");
            lcd->printLine(1, "first bleacher");
            delay(2000);
            moveForward(220);
            delay(2500);
            state = GRAB_MATERIALS;
            break;
        
        // Grab Materials state: Grab the materials to build the bleachers
        case GRAB_MATERIALS:
            lcd->clear();
            lcd->printLine(0, "Grabbing");
            lcd->printLine(1, "Materials");
            stopMotors();
            state = MOVE_TO_CONSTRUCTION;
            break;
        
        // Move To Construction: Move to the construction zone with materials.
        // Hardcoded path depending on position (first or second materials).
        case MOVE_TO_CONSTRUCTION:
            lcd->clear();
            lcd->printLine(0, "Moving to");
            lcd->printLine(1, "construction");
            // Path from first bleacher
            if (!secondIsBuilt) {
                turnRight(220);
                delay(3000);
                moveForward(220);
                delay(2000);
            } else {
                stopMotors();
            }
            // Path from second bealcher
            // is in construction zone
            state = BUILD_BLEACHER;
            break;
            
        // Build Bleacher state: Move arms and hands to build a bleacher
        case BUILD_BLEACHER:
            lcd->clear();
            lcd->printLine(0, "Building...");
            if (!secondIsBuilt) {
                // After first bleacher is built
                state = MOVE_TO_SECOND;
            } else {
                // After second bleacher is built
                state = GO_HOME;
            }
            break;
        
        // Move To Second state: Move to the second materials spot
        case MOVE_TO_SECOND:
            lcd->clear();
            lcd->printLine(0, "Moving to");
            lcd->printLine(1, "second bleacher");
            turnLeft(220);
            delay(3000);
            moveForward(220);
            delay(2000);
            state = GRAB_MATERIALS;
            break;
            
        // Go Home state: Move to arrival zone. Pause for a few seconds when in front of PAMI.
        case GO_HOME:
            lcd->clear();
            lcd->printLine(0, "Going Home...");
            lcd->printLine(1, ":)");
            break;

        // PAUSE state: Robot stop moving.
        case PAUSE:
            lcd->clear();
            lcd->printLine(0, "In PAUSE state");
            lcd->printLine(1, "Motors stopped");
            stopMotors();
            delay(5000);
            if (!isObstacleDetected()) {
                lcd->clear();
                lcd->printLine(0, "No obstacle");
                lcd->printLine(1, "Resuming prev.");
                Serial.println("No more obstacle detected. Resuming previous state.");
                state = previousState;  // Go back to the last state before PAUSE
            } else {
                state = AVOID_OBSTACLE;
            }
            break;
            
        // Avoid Obstacle state: Avoid the obstacle by turning left or right, then going back on path
        case AVOID_OBSTACLE:
            lcd->clear();
            lcd->printLine(0, "Avoiding");
            lcd->printLine(1, "Obstacle...");
            
            // First back up a bit to create space
            moveBackward(200);
            delay(1000);
            stopMotors();
            
            // Turn right to try to go around
            turnRight(200);
            delay(1500);
            stopMotors();
            
            // Check if path is clear
            if (!isObstacleDetected()) {
                // Move forward to go around obstacle
                moveForward(200);
                delay(1500);
                stopMotors();
                
                // Turn left to get back on path
                turnLeft(200);
                delay(1500);
                stopMotors();
                
                // Resume previous state
                state = previousState;
            } else {
                // Try turning left instead
                turnLeft(200);
                delay(3000); // Turn more to try another direction
                stopMotors();
                
                // Stay in AVOID_OBSTACLE state and try again next iteration
            }
            break;
    }
}