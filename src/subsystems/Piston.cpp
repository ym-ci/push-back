#include "subsystems/Piston.h"

Piston::Piston(std::function<bool()> buttonSupplier, char port)
    :piston(port),buttonSupplier(buttonSupplier),isExtended(false) {
   piston.set_value(false);
}

void Piston::extend() {
   piston.set_value(true);
   isExtended = true;
}

void Piston::retract() {
   piston.set_value(false);
   isExtended = false;
}

void Piston::toggle() {
    if (isExtended) {
        retract();
    } else {
        extend();
    }
}

void Piston::runWithSupplier() {
    if (!buttonSupplier) {
        return;
    }

    // Read button state from supplier
    bool buttonPressed =buttonSupplier();

    // Toggle only on button press (not hold)
    if (buttonPressed && !lastButtonState) {
        toggle();
    }

   lastButtonState = buttonPressed;
}

void Piston::periodic() {
    runWithSupplier();
}
