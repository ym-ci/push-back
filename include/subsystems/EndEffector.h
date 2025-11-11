#pragma once
#include "Piston.h"
#include "command/Subsystem.h"
#include "pros/motors.hpp"
#include "pros/misc.hpp"

class EndEffector : public command::Subsystem {
public:
    // Singleton access
    static EndEffector& getInstance();

    // Configure singleton (call once early)
    static void initialize();

    // Delete copy/move
    EndEffector(const EndEffector&) = delete;
    EndEffector& operator=(const EndEffector&) = delete;
    EndEffector(EndEffector&&) = delete;
    EndEffector& operator=(EndEffector&&) = delete;

    // Behavior
    void runWithController();
    void stop();
    void periodic();
    void scoreMiddle();
    void scoreTop();
    void intake();
    void outtake();

private:
    EndEffector() = default;

    static EndEffector* instance;

    // Owned motor
    std::unique_ptr<pros::Motor> endEffectorMotor;

    // Non-owning controller and blocker piston
    pros::Controller* controller{nullptr};
    Piston* blocker{nullptr};
};
