#include "subsystems/EndEffector.h"

// Controller-based end effector: store controller pointer (non-owning) and motor pointer
EndEffector::EndEffector(pros::Controller* controller, pros::Motor *endEffectorMotor)
    : m_controller(controller), m_endEffectorMotor(endEffectorMotor) {}

// Define end effector velocities (tweak as needed)
const int topVel = 200; // L1 -> deploy/close
const int midVel = -200; // L2 -> retract/open

void EndEffector::scoreMiddle() {
    m_endEffectorMotor->move_velocity(midVel);
}

void EndEffector::scoreTop() {
    m_endEffectorMotor->move_velocity(topVel);
}

void EndEffector::runWithController() {
    if (!m_controller) {
        stop();
        return;
    }

    // Read L1/L2 from the controller
    bool l1 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l2 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);



    if (l1 && !l2) {
        scoreTop();
    } else if (l2 && !l1) {
        scoreMiddle();
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
