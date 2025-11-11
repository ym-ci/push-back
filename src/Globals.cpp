#include "Globals.h"
#include "ui/AutonSelector.h"
#include "subsystems/Intake.h"
#include "subsystems/EndEffector.h"
#include "subsystems/Piston.h"
#include "auton/auton.h"
#include "subsystems/Drivetrain.h"

// Define Globals storage
namespace Globals {
    Piston tounge = Piston(
            []() { return master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X); },
            'E'
        );
    Piston blocker = Piston(
            []() { return master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B); },
            'D'
        );

    AutonSelector autonSelector = AutonSelector();
;

    void init() {
        // Initialize singleton subsystems and their owned hardware

        // Drivetrain (owns drive motors, lemlib drivetrain, chassis)
        Drivetrain::initialize();


        // Intake singleton (owns intake + middle motors, reads controller)
        Intake::initialize();

        // EndEffector singleton (owns its motor, uses blocker piston + controller)
        EndEffector::initialize();

        // Add autonomous routines to the selector
        autonSelector.addAuton("Simple Forward", auton::simpleForward);
        autonSelector.addAuton("Red Left", auton::redLeft);
        autonSelector.addAuton("Blue Right", auton::blueRight);
        autonSelector.addAuton("Skills", auton::skills);
    }


    void periodic() {
        // Drivetrain tank drive and periodic (singleton)
        Drivetrain::getInstance().periodic();

        // Intake periodic (singleton)
        Intake::getInstance().periodic();

        // End effector periodic (singleton)
        EndEffector::getInstance().periodic();
    }
}
