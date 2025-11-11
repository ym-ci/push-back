#pragma once
#include "command/Subsystem.h"
#include "pros/motors.hpp"
#include "pros/misc.hpp"

class Intake : public command::Subsystem {
public:
    // Singleton access
    static Intake& getInstance();

    // Configure singleton (call once early)
    static void initialize();

    // Delete copy/move
    Intake(const Intake&) = delete;
    Intake& operator=(const Intake&) = delete;
    Intake(Intake&&) = delete;
    Intake& operator=(Intake&&) = delete;

    // Control methods
    void runWithController();
    void stop();
    void periodic();
    void score();
    void intake();
    void outtake();

private:
    Intake() = default;

    static Intake* instance;

    // Motors owned by subsystem
    std::unique_ptr<pros::Motor> intakeMotor;
    std::unique_ptr<pros::Motor> middleMotor;

    // Non-owning controller pointer
    pros::Controller* controller{nullptr};
};
