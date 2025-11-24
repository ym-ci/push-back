#include "subsystems/EndEffector.h"
#include "Globals.h"

// Velocities (internal)
namespace {
    constexpr int topVel = 200;      // L1
    constexpr int midVel = -200;     // L2
    constexpr int intakeVel = 75;    // R2
    constexpr int outtakeVel = -200; // R1
}

// Static instance pointer
EndEffector* EndEffector::instance = nullptr;

// Configure singleton
void EndEffector::initialize() {
    if (instance != nullptr) return;

    instance = new EndEffector();
    instance->controller = &Globals::master;
    instance->blocker = &Globals::blocker;

    // Port copied from RobotContainer (21)
    instance->endEffectorMotor = std::make_unique<pros::Motor>(21);
}

// Accessor
EndEffector& EndEffector::getInstance() {
    return *instance;
}

void EndEffector::scoreMiddle() {
    if (!endEffectorMotor) return;
    endEffectorMotor->move_velocity(midVel);
}

void EndEffector::scoreTop() {
    if (!endEffectorMotor) return;
    endEffectorMotor->move_velocity(topVel);
    if (blocker) blocker->extend();
}

void EndEffector::intake() {
    if (!endEffectorMotor) return;
    endEffectorMotor->move_velocity(intakeVel);
    if (blocker) blocker->retract();
}

void EndEffector::outtake() {
    if (!endEffectorMotor) return;
    endEffectorMotor->move_velocity(outtakeVel);
}

void EndEffector::runWithController() {
    if (!controller) {
        stop();
        return;
    }

    bool l2 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l1 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool r2 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r1 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2);

    if (l1 && !l2) {
        // Score to top
        scoreTop();
    } else if (l2 && !l1) {
        // Score to middle
        scoreMiddle();
    } else if (r1 && !r2) {
        // Intake
        intake();
    } else if (r2 && !r1) {
        // Outtake
        outtake();
    } else {
        stop();
    }
}

void EndEffector::stop() {
    if (endEffectorMotor)
        endEffectorMotor->move_velocity(0);
}

void EndEffector::periodic() {
    runWithController();
}
