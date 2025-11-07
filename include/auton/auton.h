#pragma once

#include "main.h"
#include "RobotContainer.h"

namespace auton {
    // Initialize autonomous functions
    void init(RobotContainer* container);
    
    // Different autonomous routines
    void simpleForward();
    void redLeft();
    void blueRight();
    void skills();
    
    // Helper functions
    void driveForward(int speed, int duration_ms);
    void driveBackward(int speed, int duration_ms);
    void turnLeft(int speed, int duration_ms);
    void turnRight(int speed, int duration_ms);
    void stop();
}