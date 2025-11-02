#include "subsystems/EndEffector.h"

// Controller-based end effector: store controller pointer (non-owning) and motor pointer
EndEffector::EndEffector(pros::Controller* controller, pros::Motor *endEffectorMotor, Piston* blocker)
    :controller(controller), endEffectorMotor(endEffectorMotor), blocker(blocker) {}

// Define end effector velocities (tweak as needed)
const int topVel = 200; // L1
const int midVel = -200; // L2
const int intakeVel = 75; // R2
const int outtakeVel = -200; // R1

void EndEffector::scoreMiddle() {
   endEffectorMotor->move_velocity(midVel);
}

void EndEffector::scoreTop() {
   endEffectorMotor->move_velocity(topVel);
}

void EndEffector::intake() {
   endEffectorMotor->move_velocity(intakeVel);
}

void EndEffector::outtake() {
   endEffectorMotor->move_velocity(outtakeVel);
}

void EndEffector::runWithController() {
    if (!controller) {
        stop();
        return;
    }

    // Read L1/L2 from the controller
    bool l2 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l1 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool r2 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool r1 =controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2);



    if (l1 && !l2) {
        // Score to top
        scoreTop();
        blocker->extend();
    } else if (l2 && !l1) {
        // Score to middle
        scoreMiddle();
    } else if( r1 && !r2 ) {
        // Intake
        intake();
        blocker->retract();
    } else if( r2 && !r1 ) {
        // Outtake
        outtake();
    } else {
        // both pressed or neither -> stop
        stop();
    }
}

void EndEffector::stop() {
   endEffectorMotor->move_velocity(0);
}

void EndEffector::periodic() {
    // Default periodic action: read controller and run end effector logic if controller set
    runWithController();
}
