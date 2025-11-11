#pragma once

namespace auton {
    // Initialize autonomous globals (uses Globals namespace instead of RobotContainer)
    void init();

    // Autonomous routines (selected via AutonSelector)
    void simpleForward();
    void redLeft();
    void blueRight();
    void skills();
}