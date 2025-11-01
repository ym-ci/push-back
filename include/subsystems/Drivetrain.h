#pragma once
#include "command/Subsystem.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"

class Drivetrain : public command::Subsystem {
public:
    // Construct from an existing lemlib::Chassis instance (ownership retained elsewhere)
    explicit Drivetrain(lemlib::Chassis *chassis);
    void arcadeDrive(int forward, int turn);
    void stop();
    void periodic();
    lemlib::Chassis* getChassis() { return chassis; }

    // Run arcade drive using a controller (reads axes and drives)
    void runArcade(pros::Controller* controller);

private:
    lemlib::Chassis *chassis;
};
