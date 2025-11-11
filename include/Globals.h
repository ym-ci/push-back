#pragma once
#include "pros/misc.hpp" // for pros::Controller
#include "subsystems/Piston.h"
#include "ui/AutonSelector.h"

namespace lemlib { class Drivetrain; class Chassis; }

namespace Globals {
    // Public master controller (shared across subsystems and routines)
    inline pros::Controller master{pros::E_CONTROLLER_MASTER};

    // Pistons (own their lifetime; depend on controller button callbacks)
    extern Piston tounge;
    extern Piston blocker;

    // Autonomous selector and sequences
    extern AutonSelector autonSelector;

    // Initialization / teardown
    void init();

    // Periodic update to be called from opcontrol
    void periodic();
}
