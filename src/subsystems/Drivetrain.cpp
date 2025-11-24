#include "subsystems/Drivetrain.h"
#include "EndEffector.h"
#include "Globals.h"
#include "Intake.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <iostream>

// Static instance pointer

// File-scope static objects for drivetrain configuration. These live for the
// program lifetime and are used to initialize the static `Drivetrain::chassis`.
static pros::MotorGroup s_rightGroup(std::vector<std::int8_t>{RIGHT_MOTOR_PORTS});
static pros::MotorGroup s_leftGroup(std::vector<std::int8_t>{LEFT_MOTOR_PORTS});

static lemlib::Drivetrain s_lemlibDrivetrain(&s_leftGroup, &s_rightGroup,
                      DRIVETRAIN_WHEEL_DIAMETER,
                      DRIVETRAIN_OMNIWHEEL,
                      DRIVETRAIN_GEARSET,
                      DRIVETRAIN_TRACK_WIDTH);

static pros::Rotation horizontalRotation(20);
static lemlib::TrackingWheel horizontalTrackingWheel(&horizontalRotation, lemlib::Omniwheel::NEW_2, -2);
static pros::Imu imu(9);

static lemlib::OdomSensors s_sensors(nullptr, nullptr, &horizontalTrackingWheel, nullptr, &imu);

static lemlib::ControllerSettings s_lateralController(
  15,  // proportional gain (kP)
  0,   // integral gain (kI)
  0,   // derivative gain (kD)
  0,   // anti windup
  0,   // small error range, in inches
  0,   // small error range timeout, in milliseconds
  0,   // large error range, in inches
  0,   // large error range timeout, in milliseconds
  0    // maximum acceleration (slew)
);

static lemlib::ControllerSettings s_angularController(
  3.5,  // proportional gain (kP)
  0.5,  // integral gain (kI)
  1.75, // derivative gain (kD)
  3,    // anti windup
  0.75, // small error range, in degrees
  100,  // small error range timeout, in milliseconds
  1.5,  // large error range, in degrees
  300,  // large error range timeout, in milliseconds
  0     // maximum acceleration (slew)
);

// static expo curves so their addresses remain valid
static lemlib::ExpoDriveCurve s_throttleCurve(THROTTLE_DEADBAND, THROTTLE_MIN_OUTPUT, THROTTLE_EXPO_GAIN);
static lemlib::ExpoDriveCurve s_steerCurve(STEER_DEADBAND, STEER_MIN_OUTPUT, STEER_EXPO_GAIN);

// Define the static chassis (uses other file-scope statics above).
lemlib::Chassis Drivetrain::chassis(s_lemlibDrivetrain, s_lateralController, s_angularController, s_sensors, &s_throttleCurve, &s_steerCurve);





// Static initializer: sets up motors, drivetrain, odom, and chassis singleton
void Drivetrain::initialize() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    s_rightGroup.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    s_leftGroup.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  // Use the file-scope `s_sensors`, `s_throttleCurve`, and `s_steerCurve`.
  // Calibrate and set pose on the static chassis object.
  chassis.calibrate();
  chassis.setPose(0, 0, 0);
}

void Drivetrain::coordDisplayInit() {
  pros::lcd::initialize();

  pros::Task screen_task([&]() {
    while (true) {
      int i = 0;
      // pros::lcd::print(i++, "X: %f", chassis.getPose().x);
      // pros::lcd::print(i++, "Y: %f", chassis.getPose().y);
      // pros::lcd::print(i++, "Theta: %f", chassis.getPose().theta);
      // All in one line at the bottom centerned
      pros::lcd::print(5, "X: %f Y: %f Theta: %f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
      pros::delay(20);

      //   std::cout << "Task Looped" << std::endl;
    }
  });
}



void Drivetrain::arcadeDrive(int forward, int turn) {
  chassis.arcade(forward, turn);
}

void Drivetrain::stop() { chassis.arcade(0, 0); }

void Drivetrain::periodic() {
  chassis.curvature(
      Globals::master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
      Globals::master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
}

void Drivetrain::simpleForward() {
  chassis.setPose(0, 0, 0);
  // Print a debug message
  // std::cout << "Starting simpleForward autonomous routine." << std::endl;
  chassis.moveToPose(0, -12, 0, 10000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(180, 10000);
  // chassis.waitUntilDone();
  // // pros::c::delay(1000);
  // chassis.turnToHeading(0, 10000);
  // chassis.moveToPose(0, 0, 180, 10000);
  // chassis.waitUntilDone();
}

void Drivetrain::leftAuton() {
  chassis.moveToPose(10.625, -30.126, -39.108, 10000, {.forwards = false});
  Intake::getInstance().intake();
  Globals::blocker.extend();
  chassis.waitUntilDone();
  // pros::delay(1000);
  chassis.turnToHeading(-30, 1000, {}, false);
  chassis.turnToHeading(-47, 1000, {}, false);
  chassis.turnToHeading(-42, 1000, {}, false);
  chassis.turnToHeading(-42, 1000, {}, false);
  Intake::getInstance().stop();
  chassis.moveToPose(10.818, 3.7545, 88.85, 10000);
  chassis.waitUntilDone();
  chassis.moveToPose(30.93, -11.90, 169.9, 10000);
  Globals::blocker.retract();
  pros::delay(100);
  Intake::getInstance().intake();
  EndEffector::getInstance().scoreTop();
  pros::delay(5000);
}
