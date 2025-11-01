#pragma once
#include "command/Subsystem.h"
#include "pros/motors.hpp"
#include "pros/misc.hpp"

class EndEffector : public command::Subsystem {
public:
    // Construct from an existing lemlib::Chassis instance (ownership retained elsewhere)
    // Construct with controller pointer and motor pointers. Controller pointer
    // is stored (no ownership) so the intake can read buttons each periodic.
    explicit EndEffector(pros::Controller* controller, pros::Motor *endEffectorMotor);
    // Read stored controller buttons and drive intake motors. R1 -> intake in, R2 -> out.
    // If both pressed or controller is null, motors are stopped.
    void runWithController();
    void stop();
    void periodic();
    void scoreMiddle();
    void scoreTop();
    void intake();
    void outtake();

private:
    pros::Motor *endEffectorMotor;
    pros::Controller*controller{nullptr};
};
