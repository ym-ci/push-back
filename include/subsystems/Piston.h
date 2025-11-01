#pragma once
#include "command/Subsystem.h"
#include "pros/adi.hpp"
#include <functional>

class Piston : public command::Subsystem {
public:
    // Construct with a supplier function (boolean supplier) and ADI port.
    // The supplier is called each periodic to determine if toggle should occur.
    explicit Piston(std::function<bool()> buttonSupplier, char port);
    
    // Read supplier and toggle piston state on button press
    void runWithSupplier();
    
    // Manual control
    void extend();
    void retract();
    void toggle();
    bool isExtended{false};

    void periodic();

private:
    pros::adi::DigitalOut piston;
    std::function<bool()> buttonSupplier;
    bool lastButtonState{false};
};
