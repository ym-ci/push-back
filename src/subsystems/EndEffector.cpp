#include "subsystems/EndEffector.h"

// Controller-based end effector: store controller pointer (non-owning) and motor pointer
EndEffector::EndEffector(pros::Controller* controller, pros::Motor *endEffectorMotor)
    : m_controller(controller), m_endEffectorMotor(endEffectorMotor) {}

// Define end effector velocities (tweak as needed)
const int topVel = 200; // L1
const int midVel = -200; // L2
const int intakeVel = 40; // R2
const int outtakeVel = -200; // R1

void EndEffector::scoreMiddle() {
    m_endEffectorMotor->move_velocity(midVel);
}

void EndEffector::scoreTop() {
    m_endEffectorMotor->move_velocity(topVel);
}

void EndEffector::intake() {
    m_endEffectorMotor->move_velocity(intakeVel);
}

void EndEffector::outtake() {
    m_endEffectorMotor->move_velocity(outtakeVel);
}

void EndEffector::runWithController() {
    if (!m_controller) {
        stop();
        return;
    }

    // Read L1/L2 from the controller
    bool l1 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l2 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool r1 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r2 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2);



    if (l1 && !l2) {
        // Score to top
        scoreTop();
    } else if (l2 && !l1) {
        // Score to middle
        scoreMiddle();
    } else if( r1 && !r2 ) {
        // Intake
        intake();
    } else if( r2 && !r1 ) {
        // Outtake
        outtake();
    } else {
        // both pressed or neither -> stop
        stop();
    }
}

void EndEffector::stop() {
    m_endEffectorMotor->move_velocity(0);
}

void EndEffector::periodic() {
    // Default periodic action: read controller and run end effector logic if controller set
    runWithController();
}
