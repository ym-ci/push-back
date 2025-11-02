#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain(lemlib::Chassis *chassis) :chassis(chassis) {}

void Drivetrain::arcadeDrive(int forward, int turn) {
    if (chassis)
       chassis->arcade(forward, turn);
}

void Drivetrain::stop() {
    if (chassis)
       chassis->arcade(0, 0);
}

void Drivetrain::periodic() {
    // LemLib chassis handles its own internal periodic updates if needed
}

void Drivetrain::runArcade(pros::Controller* controller) {
    if (!controller) return;
    int forward = controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    // Use same sign inversion as previous ArcadeDriveCommand
    arcadeDrive(forward, turn);
}

void Drivetrain::runTank(pros::Controller* controller) {
    if (!controller) return;
    int left = controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int right = controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    if (chassis)
       chassis->tank(left, right);
    
}
