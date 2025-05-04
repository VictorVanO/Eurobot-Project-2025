#ifndef FSM_COMMON_H
#define FSM_COMMON_H


enum RobotState {
    IDLE,             
    MOVE_FORWARD_STATE,  
    TURN_LEFT,
    TURN_RIGHT,
    AWAIT_OBSTACLE_STATE,
    PARTY_STATE
};

#endif
