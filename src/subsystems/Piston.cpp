#include "subsystems/Piston.h"

Piston::Piston(std::function<bool()> buttonSupplier, char port)
    : m_piston(port), m_buttonSupplier(buttonSupplier), m_isExtended(false) {
    m_piston.set_value(false);
}

void Piston::extend() {
    m_piston.set_value(true);
    m_isExtended = true;
}

void Piston::retract() {
    m_piston.set_value(false);
    m_isExtended = false;
}

void Piston::toggle() {
    if (m_isExtended) {
        retract();
    } else {
        extend();
    }
}

void Piston::runWithSupplier() {
    if (!m_buttonSupplier) {
        return;
    }

    // Read button state from supplier
    bool buttonPressed = m_buttonSupplier();

    // Toggle only on button press (not hold)
    if (buttonPressed && !m_lastButtonState) {
        toggle();
    }

    m_lastButtonState = buttonPressed;
}

void Piston::periodic() {
    runWithSupplier();
}
