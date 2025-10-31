#pragma once
#include "command/Subsystem.h"
#include "pros/motors.hpp"
#include "pros/misc.hpp"

class Intake : public command::Subsystem {
public:
    // Construct from an existing lemlib::Chassis instance (ownership retained elsewhere)
    // Construct with controller pointer and motor pointers. Controller pointer
    // is stored (no ownership) so the intake can read buttons each periodic.
    explicit Intake(pros::Controller* controller, pros::Motor *intakeMotor, pros::Motor *middleMotor);
    // Read stored controller buttons and drive intake motors. R1 -> intake in, R2 -> out.
    // If both pressed or controller is null, motors are stopped.
    void runWithController();
    void stop();
    void periodic();
    void score();
    void intake();
    void outtake();

private:
    pros::Motor *m_intakeMotor;
    pros::Motor *m_middleMotor;
    pros::Controller* m_controller{nullptr};
};
