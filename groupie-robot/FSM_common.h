#ifndef FSM_COMMON_H
#define FSM_COMMON_H

enum RobotState {
    IDLE,             
    FOLLOW_LINE_STATE,  
    AWAIT_OBSTACLE_STATE,
    WAIT_CLEAR_STATE,
    PARTY_STATE,

};

#endif