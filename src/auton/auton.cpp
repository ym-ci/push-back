#include "auton/auton.h"
#include "pros/misc.h"
#include "subsystems/Drivetrain.h"

namespace auton {

    static auto& chassis = Drivetrain::getInstance().chassis;

    // Initialize autonomous globals (no RobotContainer pointer needed)
    void init() {
        // No-op for now; Globals::init() is called during robot initialize.
    }

    // Autonomous routines
    void simpleForward() {
        chassis.setPose(0, 0, 0);
        chassis.moveToPose(0, 12, 0, 1000);
    }

    void redLeft() {
        // TODO: implement path
    }

    void blueRight() {
        // TODO: implement path
    }

    void skills() {
        // TODO: implement path
    }
}