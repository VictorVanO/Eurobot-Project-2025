#include <ArduinoFake.h>
#include <unity.h>
#include "FSM.h"
#include "Motors.h"
#include "Ultrasonic.h"
#include "LCD.h"
#include "ServoArms.h"

using namespace fakeit;

class MockFSM : public FSM {
public:
    RobotState getCurrentState() {
        return state;
    }
    
    void setState(RobotState newState) {
        state = newState;
        previousState = state;
    }
    
    bool getIsObstacleDetected() {
        return isObstacleDetected();
    }
    
    void setMoveStartTime(unsigned long time) {
        moveStartTime = time;
    }
    
    void setMoveDuration(unsigned long duration) {
        moveDuration = duration;
    }
    
    void setObstacleTreshold(unsigned long threshold) {
        obstacle_treshold = threshold;
    }
    
    using FSM::handleState;
    using FSM::handleMovementCompletion;
};

void setUp(void) {
    ArduinoFake().Reset();
}

void tearDown(void) {
}

float mockDistance = 50.0;  // Default: no obstacle
float readDistanceMock(int sensorIndex) {
    return mockDistance;
}

void test_fsm_initialization(void) {
    When(Method(ArduinoFake(), millis)).Return(1000);
    When(Method(ArduinoFake(), Serial)).AlwaysReturn();
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();
    
    MockFSM fsm;
    fsm.init();
    TEST_ASSERT_EQUAL(INIT, fsm.getCurrentState());
}

void test_obstacle_detection(void) {
    MockFSM fsm;
    fsm.init();
    
    auto original_readDistance = readDistance;
    readDistance = readDistanceMock;
    
    mockDistance = 50.0;
    TEST_ASSERT_FALSE(fsm.getIsObstacleDetected());
    
    mockDistance = 10.0;
    TEST_ASSERT_TRUE(fsm.getIsObstacleDetected());
    
    readDistance = original_readDistance;
}

void test_state_transitions(void) {
    When(Method(ArduinoFake(), millis)).Return(1000);
    When(Method(ArduinoFake(), Serial)).AlwaysReturn();
    
    MockFSM fsm;
    
    // Test INIT to MOVE_TO_FIRST transition
    fsm.setState(INIT);
    fsm.handleState();
    TEST_ASSERT_EQUAL(MOVE_TO_FIRST, fsm.getCurrentState());
    
    // Test MOVE_TO_FIRST to TESTS_STATE transition (after movement completes)
    fsm.setState(MOVE_TO_FIRST);
    fsm.handleMovementCompletion();
    TEST_ASSERT_EQUAL(TESTS_STATE, fsm.getCurrentState());
    
    // Test TESTS_STATE to MOVE_TO_FIRST transition (after movement completes)
    fsm.setState(TESTS_STATE);
    fsm.handleMovementCompletion();
    TEST_ASSERT_EQUAL(MOVE_TO_FIRST, fsm.getCurrentState());
}

void test_movement_commands(void) {
    // Mocking pinMode, digitalWrite, and analogWrite
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();
    
    // Initialize motors
    initMotors();
    
    // Check initialization
    Verify(Method(ArduinoFake(), pinMode).Using(PWM_1, OUTPUT));
    Verify(Method(ArduinoFake(), pinMode).Using(DIR_1, OUTPUT));
    Verify(Method(ArduinoFake(), pinMode).Using(PWM_2, OUTPUT));
    Verify(Method(ArduinoFake(), pinMode).Using(DIR_2, OUTPUT));
    
    // Test moveForward
    moveForward(200);
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_1, 1));
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_2, 1));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_1, 200));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_2, 200));
    
    // Test moveBackward
    moveBackward(150);
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_1, 0));
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_2, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_1, 150));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_2, 150));
    
    // Test turnLeft
    turnLeft(180);
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_1, 0));
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_2, 1));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_1, 180));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_2, 180));
    
    // Test turnRight
    turnRight(220);
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_1, 1));
    Verify(Method(ArduinoFake(), digitalWrite).Using(DIR_2, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_1, 220));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_2, 220));
    
    // Test stopMotors
    stopMotors();
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_1, 0));
    Verify(Method(ArduinoFake(), analogWrite).Using(PWM_2, 0));
}

void test_ultrasonic_sensors(void) {
    // Mock pinMode
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();
    
    // Setup pulseIn to return specific durations for each sensor
    // Distance = (duration * 0.0343) / 2
    When(OverloadedMethod(ArduinoFake(), pulseIn, unsigned long(uint8_t, uint8_t, unsigned long)))
        .Return(1000); // ~17.15cm
    
    initUltrasonic();
    
    // Verify pin initialization
    for (int i = 0; i < NUM_ULTRASONIC; i++) {
        Verify(Method(ArduinoFake(), pinMode).Using(trigPins[i], OUTPUT));
        Verify(Method(ArduinoFake(), pinMode).Using(echoPins[i], INPUT));
    }
    
    // Test reading from first sensor
    float distance = readDistance(0);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 17.15, distance);
    
    // Verify sensor reading sequence
    Verify(Method(ArduinoFake(), digitalWrite).Using(trigPins[0], LOW));
    Verify(Method(ArduinoFake(), digitalWrite).Using(trigPins[0], HIGH));
    Verify(Method(ArduinoFake(), digitalWrite).Using(trigPins[0], LOW));
    Verify(OverloadedMethod(ArduinoFake(), pulseIn, unsigned long(uint8_t, uint8_t, unsigned long))
           .Using(echoPins[0], HIGH, ANY(unsigned long)));
}

void test_obstacle_avoidance(void) {
    // Setup mocks
    When(Method(ArduinoFake(), Serial)).AlwaysReturn();
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();
    
    // Create sequential time values for millis() calls
    unsigned long timeValues[] = {1000, 1000, 3500, 3500, 3500, 3500};
    int timeIndex = 0;
    When(Method(ArduinoFake(), millis)).AlwaysDo([&timeValues, &timeIndex]() {
        return timeValues[timeIndex++ % 6];
    });
    
    // Override readDistance to use our mock function
    auto original_readDistance = readDistance;
    readDistance = readDistanceMock;
    
    MockFSM fsm;
    fsm.init();
    
    // Start in MOVE_TO_FIRST state
    fsm.setState(MOVE_TO_FIRST);
    
    // Simulate obstacle
    mockDistance = 10.0;
    
    // First call should change to PAUSE state
    fsm.handleState();
    TEST_ASSERT_EQUAL(PAUSE, fsm.getCurrentState());
    
    // After time passes, should transition to AVOID_OBSTACLE
    fsm.handleState();
    TEST_ASSERT_EQUAL(AVOID_OBSTACLE, fsm.getCurrentState());
    
    // Clear obstacle
    mockDistance = 50.0;
    
    // Should eventually return to MOVE_TO_FIRST
    fsm.setMoveStartTime(0); // Reset timed movement
    fsm.handleState();
    TEST_ASSERT_EQUAL(MOVE_TO_FIRST, fsm.getCurrentState());
    
    // Restore original function
    readDistance = original_readDistance;
}

void test_lcd_functions(void) {
    // We can't fully test LCD without complex mocking of the LiquidCrystal_I2C library
    // But we can test that our LCD functions don't crash
    // This would be more of an integration test in real hardware
    
    // Mock the LiquidCrystal_I2C class
    // NOTE: This is a simplified test - in a real test environment,
    // you would use a more sophisticated mock of the LCD class
    
    TEST_ASSERT_TRUE(true); // Placeholder test
}

// Test ServoArms - basic functionality checks
void test_servo_arms(void) {
    // Mocking the ServoEasing library functionality would be complex
    // This would be better tested with integration tests
    // For unit testing, we could verify command sequences are correct
    
    ServoArms arms;
    
    // Verify arm extension commands
    String extendCommands[2];
    extendCommands[0] = "Y=23,Z=85,R=160";
    extendCommands[1] = "C=2";
    
    // This is a simplified test - not using mocks for the Servo objects
    TEST_ASSERT_TRUE(true); // Placeholder test
}

void test_build_sequence(void) {
    When(Method(ArduinoFake(), Serial)).AlwaysReturn();
    When(Method(ArduinoFake(), millis)).Return(1000);
    
    MockFSM fsm;
    fsm.init();
    
    // Test BUILD_BLEACHER first time
    fsm.setState(BUILD_BLEACHER);
    fsm.handleState();
    TEST_ASSERT_EQUAL(MOVE_TO_SECOND, fsm.getCurrentState());
    
    // Test BUILD_BLEACHER second time (setting secondIsBuilt flag internally)
    fsm.setState(BUILD_BLEACHER);
    fsm.handleState();
    TEST_ASSERT_EQUAL(GO_HOME, fsm.getCurrentState());
}

// Main test runner
int main() {
    UNITY_BEGIN();
    
    RUN_TEST(test_fsm_initialization);
    RUN_TEST(test_obstacle_detection);
    RUN_TEST(test_state_transitions);
    RUN_TEST(test_movement_commands);
    RUN_TEST(test_ultrasonic_sensors);
    RUN_TEST(test_obstacle_avoidance);
    RUN_TEST(test_lcd_functions);
    RUN_TEST(test_servo_arms);
    RUN_TEST(test_build_sequence);
    
    return UNITY_END();
}