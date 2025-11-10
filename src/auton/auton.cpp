#include "auton/auton.h"
#include "pros/misc.h"
#include <memory>

namespace auton {
    // Static pointer to robot container
    static RobotContainer* robot = nullptr;

    void init(RobotContainer* container) {
        robot = container;
    }

    // Helper functions
    void driveForward(int speed, int duration_ms) {
        robot->getDrivetrain()->arcadeDrive(speed, 0);
        pros::delay(duration_ms);
    }

    void driveBackward(int speed, int duration_ms) {
        robot->getDrivetrain()->arcadeDrive(-speed, 0);
        pros::delay(duration_ms);
    }

    void turnLeft(int speed, int duration_ms) {
        robot->getDrivetrain()->arcadeDrive(0, -speed);
        pros::delay(duration_ms);
    }

    void turnRight(int speed, int duration_ms) {
        robot->getDrivetrain()->arcadeDrive(0, speed);
        pros::delay(duration_ms);
    }

    void stop() {
        robot->getDrivetrain()->stop();
    }

    // Autonomous routines
    void simpleForward() {
        // Use the drivetrain's lemlib chassis via RobotContainer accessor
        auto chassis = robot->getChassis();
        if (chassis != nullptr) {
            chassis->setPose(0, 0, 0);
            chassis->moveToPose(0, 12, 0, 1000);
        }
    }

    void redLeft() {
        driveForward(80, 2000);   // Drive forward at 80% speed for 2 seconds
        turnLeft(60, 1000);       // Turn left at 60% speed for 1 second
        stop();
    }

    void blueRight() {
        driveForward(80, 2000);   // Drive forward at 80% speed for 2 seconds
        turnRight(60, 1000);      // Turn right at 60% speed for 1 second
        stop();
    }

    void skills() {
        driveForward(80, 3000);   // Drive forward at 80% speed for 3 seconds
        driveBackward(80, 3000);  // Drive backward at 80% speed for 3 seconds
        stop();
    }
}