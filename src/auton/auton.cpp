#include "auton/auton.h"
#include "pros/misc.h"
#include "subsystems/Drivetrain.h"
#include <memory>

namespace auton {

    static auto& chassis = Drivetrain::getInstance().chassis;

    // Autonomous routines
    void simpleForward() {
        // Use the drivetrain's lemlib chassis via RobotContainer accessor
        
        chassis.setPose(0, 0, 0);
        chassis.moveToPose(0, 12, 0, 1000);
    }

    void redLeft() {

    }

    void blueRight() {

    }

    void skills() {

    }
}