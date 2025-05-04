#include "FSM_Yellow.h"   // Finite State Machine for the yellow configuration
#include "FSM_Blue.h"     // Finite State Machine for the blue configuration

// Pin used to determine the robot's mode (yellow or blue)
const int modeSwitchPin = 12;
ModeSelector selector(modeSwitchPin);

// Pin connected to the physical pull trigger (formerly "tirette")
const int pullPin = 11;
PullPin pullTrigger(pullPin);  // Manages start signal

// FSM instances
FSM_Yellow fsmYellow;
FSM_Blue fsmBlue;

// Flags to track system state
bool startAuthorized = false;  // Indicates if the robot is allowed to start
bool useBlue = false;          // Determines whether to run the blue FSM or the yellow FSM

void setup() {
    Serial.begin(9600);

    // Initialize selector and pull trigger hardware
    selector.begin();
    pullTrigger.begin();

    // Determine mode based on the selector state
    if (selector.isPrimaryMode()) {
        Serial.println("FSM_Blue selected");
        useBlue = true;
        fsmBlue.init();  // Initialize the blue FSM
    } else {
        Serial.println("FSM_Yellow selected");
        useBlue = false;
        fsmYellow.init();  // Initialize the yellow FSM
    }
}

void loop() {
    // Wait for the pull trigger to be activated before starting
    if (!startAuthorized) {
        if (pullTrigger.isActivated()) {
            startAuthorized = true;

            // Notify the selected FSM to begin
            if (useBlue) {
                fsmBlue.authorizeStart();
            } else {
                fsmYellow.authorizeStart();
            }
        } else {
            // Wait and check again later
            delay(200);
            return;
        }
    }

    // Run the appropriate FSM
    if (useBlue) {
        fsmBlue.run();
    } else {
        fsmYellow.run();
    }

    // Small delay to avoid overloading the loop
    delay(100);
}
