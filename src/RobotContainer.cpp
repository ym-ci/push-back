#include "RobotContainer.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "ui/AutonSelector.h"
#include "subsystems/Intake.h"
#include "subsystems/EndEffector.h"
#include "subsystems/Piston.h"
#include "auton/auton.h"
#include <cstddef>

// Implementation members moved into RobotContainer header (no Pimpl)

RobotContainer::RobotContainer() {
    // Initialize singleton subsystems and their owned hardware

    // Drivetrain (owns drive motors, lemlib drivetrain, chassis)
    Drivetrain::initialize();

    // Pistons remain here (they depend on controller button suppliers)
    tounge = std::make_unique<Piston>(
        [this]() { return master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X); },
        'E'
    );
    blocker = std::make_unique<Piston>(
        [this]() { return master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B); },
        'D'
    );

    // Intake singleton (owns intake + middle motors, reads controller)
    Intake::initialize(&master);

    // EndEffector singleton (owns its motor, uses blocker piston + controller)
    EndEffector::initialize(&master, blocker.get());

    // Autonomous selector with function pointers
    autonSelector = std::make_unique<AutonSelector>();

    // Initialize autonomous with this container
    auton::init(this);

    // Add autonomous routines to the selector
    autonSelector->addAuton("Simple Forward", auton::simpleForward);
    autonSelector->addAuton("Red Left", auton::redLeft);
    autonSelector->addAuton("Blue Right", auton::blueRight);
    autonSelector->addAuton("Skills", auton::skills);
}

RobotContainer::~RobotContainer() {
    // Singleton subsystems own their resources; RobotContainer only owns pistons and autonSelector.
}

void RobotContainer::runPeriodic() {
    // Drivetrain tank drive and periodic (singleton)
    Drivetrain::getInstance().runTank(&master);
    Drivetrain::getInstance().periodic();

    // Intake periodic (singleton)
    Intake::getInstance().periodic();

    // End effector periodic (singleton)
    EndEffector::getInstance().periodic();

    // Piston periodic (X/B button toggle; owned by container)
    if (tounge) {
        tounge->periodic();
    }
    if (blocker) {
        blocker->periodic();
    }
}
