#pragma once
#include "subsystems/Drivetrain.h"
#include "pros/misc.hpp" // for pros::Controller
#include <memory>

class AutonSelector;
class AutonSequence;
class Piston;

#include "pros/motor_group.hpp"
namespace lemlib { class Drivetrain; class Chassis; }

class RobotContainer {
public:
    RobotContainer();
    ~RobotContainer();

    // Accessors for key components
    Drivetrain* getDrivetrain() { return &Drivetrain::getInstance(); }
    AutonSelector* getAutonSelector() { return autonSelector.get(); }
    Piston* getTounge() { return tounge.get(); }
    Piston* getBlocker() { return blocker.get(); }

    // Call periodically from opcontrol to run default behaviors
    void runPeriodic();

    // Public master controller (consumed by subsystems during initialization)
    pros::Controller master{pros::E_CONTROLLER_MASTER};

private:
    // Pistons remain owned by RobotContainer (they depend on its controller callbacks)
    std::unique_ptr<Piston> tounge;
    std::unique_ptr<Piston> blocker;

    // Autonomous selector and sequences
    std::unique_ptr<AutonSelector> autonSelector;
    std::shared_ptr<AutonSequence> redLeftAuton;
    std::shared_ptr<AutonSequence> blueRightAuton;
    std::shared_ptr<AutonSequence> skillsAuton;
};
