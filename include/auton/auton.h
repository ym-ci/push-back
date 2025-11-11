#pragma once

#include "RobotContainer.h"

namespace auton {
    // Initialize autonomous functions
    void init(RobotContainer* container);
    
    // Different autonomous routines
    void simpleForward();
    void redLeft();
    void blueRight();
    void skills();
    
}