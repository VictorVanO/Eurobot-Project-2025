#include "FSM_Yellow.h"
#include "FSM_Blue.h"

const int modeSwitchPin = 3;
ModeSelector selector(modeSwitchPin);

const int pullPin = 4;
PullPin pullTrigger(pullPin);  

FSM_Yellow fsmYellow;
FSM_Blue fsmBlue;

bool startAuthorized = false;
bool useBlue = false;

void setup() {
    Serial.begin(9600);

    selector.begin();
    pullTrigger.begin();

    if (selector.isPrimaryMode()) {
        Serial.println("FSM_Blue selected");
        useBlue = true;
        fsmBlue.init();
    } else {
        Serial.println("FSM_Yellow selected");
        useBlue = false;
        fsmYellow.init();
    }
}

void loop() {
    // Wait for pull-pin to be removed before starting
    if (!startAuthorized) {
        if (!pullTrigger.isActivated()) {  // Logic stays the same (LOW = not pulled)
            startAuthorized = true;

            if (useBlue) {
                fsmBlue.authorizeStart();
            } else {
                fsmYellow.authorizeStart();
            }

        } else {
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

    delay(100);
}
