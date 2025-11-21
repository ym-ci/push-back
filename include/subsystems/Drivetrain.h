#pragma once
#include "command/Subsystem.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"

class Drivetrain : public command::Subsystem {
public:
    // Access singleton instance
    static Drivetrain& getInstance();

    // Delete copy/move to enforce singleton
    Drivetrain(const Drivetrain&) = delete;
    Drivetrain& operator=(const Drivetrain&) = delete;
    Drivetrain(Drivetrain&&) = delete;
    Drivetrain& operator=(Drivetrain&&) = delete;

    // Initialization (call once early in program)
    // Creates internal lemlib drivetrain, motors, and chassis
    static void initialize();

    void arcadeDrive(int forward, int turn);
    void stop();
    void periodic();
    // Run arcade drive using a controller (reads axes and drives)
    void runArcade(pros::Controller* controller);
     // Run tank drive using a controller (reads axes and drives)
     void runTank(pros::Controller* controller);
 
     lemlib::Chassis chassis;

     void simpleForward();
     void leftAuton();
     void coordDisplayInit();
 private:
     // Private default constructor so we can allocate in initialize()
     Drivetrain();
 
     static Drivetrain* instance;
 
     std::unique_ptr<pros::MotorGroup> leftGroup;
     std::unique_ptr<pros::MotorGroup> rightGroup;
     // Stored chassis object; lifetime managed by Drivetrain singleton
     std::unique_ptr<lemlib::Drivetrain> lemlibDrivetrain;
};

