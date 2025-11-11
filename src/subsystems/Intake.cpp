
#include "subsystems/Intake.h"
#include "Globals.h"

// Velocities (kept internal)
namespace {
    constexpr int forwardVel = 200;     // forward
    constexpr int intakeMidVel = 80;    // middle
    constexpr int outtakeVel = -200;    // reverse
}

// Static instance pointer
Intake* Intake::instance = nullptr;

// Configure singleton
void Intake::initialize() {
    if (instance != nullptr) return;

    instance = new Intake();
    instance->controller = &Globals::master;

    // Ports copied from RobotContainer
    instance->intakeMotor = std::make_unique<pros::Motor>(4);
    instance->middleMotor = std::make_unique<pros::Motor>(-10);
}

// Accessor
Intake& Intake::getInstance() {
    return *instance;
}

void Intake::score() {
    if (!intakeMotor || !middleMotor) return;
    middleMotor->move_velocity(forwardVel);
    intakeMotor->move_velocity(forwardVel);
}

void Intake::intake() {
    if (!intakeMotor || !middleMotor) return;
    intakeMotor->move_velocity(forwardVel);
    middleMotor->move_velocity(intakeMidVel);
}

void Intake::outtake() {
    if (!intakeMotor || !middleMotor) return;
    middleMotor->move_velocity(outtakeVel);
    intakeMotor->move_velocity(outtakeVel);
}

void Intake::runWithController() {
    if (!controller) {
        stop();
        return;
    }

    bool r2 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r1 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool l2 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l1 = controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    if (l1 || l2) {
        score();
    } else if (r1 && !r2) {
        intake();
    } else if (r2 && !r1) {
        outtake();
    } else {
        stop();
    }
}

void Intake::stop() {
    if (intakeMotor)
        intakeMotor->move_velocity(0);
    if (middleMotor)
        middleMotor->move_velocity(0);
}

void Intake::periodic() {
    runWithController();
}

