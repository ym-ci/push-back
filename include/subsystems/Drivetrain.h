#pragma once
#include "command/Subsystem.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"

// Define motor port lists here so they are easy to change project-wide.
// Use a preprocessor list so it can be used inside braced-init-lists.
#define RIGHT_MOTOR_PORTS 11, 1, 17
#define LEFT_MOTOR_PORTS -13, -15, -19

// Lemlib drivetrain configuration constants
#define DRIVETRAIN_WHEEL_DIAMETER 12.194788f
#define DRIVETRAIN_OMNIWHEEL lemlib::Omniwheel::NEW_275
#define DRIVETRAIN_GEARSET 450
#define DRIVETRAIN_TRACK_WIDTH 2.0f

// Expo curve configuration (throttle + steer)
#define THROTTLE_DEADBAND 3
#define THROTTLE_MIN_OUTPUT 10
#define THROTTLE_EXPO_GAIN 1.019f

#define STEER_DEADBAND 3
#define STEER_MIN_OUTPUT 10
#define STEER_EXPO_GAIN 1.019f

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
 
    static lemlib::Chassis chassis;

     void simpleForward();
     void leftAuton();
     void coordDisplayInit();
 private:
     // Private default constructor so we can allocate in initialize()
     Drivetrain();
 
     static Drivetrain* instance;
 
    pros::MotorGroup* leftGroup = nullptr;
    pros::MotorGroup* rightGroup = nullptr;
    // Stored drivetrain helper (lifetime managed by statics in initialize)
    lemlib::Drivetrain* lemlibDrivetrain = nullptr;
};

