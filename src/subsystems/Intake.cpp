
#include "subsystems/Intake.h"

// Controller-based intake: store controller pointer (non-owning) and motor pointers
Intake::Intake(pros::Controller* controller, pros::Motor *intakeMotor, pros::Motor *middleMotor)
    :controller(controller),intakeMotor(intakeMotor),middleMotor(middleMotor) {}

// Define intake velocities (tweak as needed)
const int forwardVel = 200; // forward
const int intakeMidVel = 80; // middle
const int outtakeVel = -200; // reverse

void Intake::score() {
   middleMotor->move_velocity(forwardVel);
   intakeMotor->move_velocity(forwardVel);

}

void Intake::intake() {
   intakeMotor->move_velocity(forwardVel);
   middleMotor->move_velocity(intakeMidVel);
}

void Intake::outtake() {
   middleMotor->move_velocity(outtakeVel);
   intakeMotor->move_velocity(outtakeVel);
}



void Intake::runWithController() {
    if (!controller) {
        stop();
        return;
    }

    // Read R1/R2 from the controller
    bool r1 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r2 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool l1 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l2 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);

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
   intakeMotor->move_velocity(0);
   middleMotor->move_velocity(0);
}

void Intake::periodic() {
    // Default periodic action: read controller and run intake logic if controller set
    runWithController();
}

