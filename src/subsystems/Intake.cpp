
#include "subsystems/Intake.h"

// Controller-based intake: store controller pointer (non-owning) and motor pointers
Intake::Intake(pros::Controller* controller, pros::Motor *intakeMotor, pros::Motor *middleMotor)
    : m_controller(controller), m_intakeMotor(intakeMotor), m_middleMotor(middleMotor) {}

// Define intake velocities (tweak as needed)
const int forwardVel = 200; // forward
const int intakeMidVel = 80; // middle
const int outtakeVel = -200; // reverse

void Intake::score() {
    m_middleMotor->move_velocity(forwardVel);
    m_intakeMotor->move_velocity(forwardVel);

}

void Intake::intake() {
    m_intakeMotor->move_velocity(forwardVel);
    m_middleMotor->move_velocity(intakeMidVel);
}

void Intake::outtake() {
    m_middleMotor->move_velocity(outtakeVel);
    m_intakeMotor->move_velocity(outtakeVel);
}



void Intake::runWithController() {
    if (!m_controller) {
        stop();
        return;
    }

    // Read R1/R2 from the controller
    bool r1 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r2 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool l1 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l2 = m_controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    if (l1 || l2) {
        score();
    } else if (r1 && !r2) {
        intake();
    } else if (r2 && !r1) {
        outtake();
    } else {
        // both pressed or neither -> stop
        stop();
    }
}

void Intake::stop() {
    m_intakeMotor->move_velocity(0);
    m_middleMotor->move_velocity(0);
}

void Intake::periodic() {
    // Default periodic action: read controller and run intake logic if controller set
    runWithController();
}

