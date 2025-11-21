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
Drivetrain *Drivetrain::instance = nullptr;

// lemlib::ControllerSettings
//     lateralController(90,  // proportional gain (kP)
//                       90, // integral gain (kI)
//                       0, // derivative gain (kD)
//                       0,   // anti windup
//                       1,   // small error range, in inches
//                       100, // small error range timeout, in milliseconds
//                       3,   // large error range, in inches
//                       300, // large error range timeout, in milliseconds
//                       25   // maximum acceleration (slew)
//     );

// // angular PID controller
// lemlib::ControllerSettings
//     angularController(3.5, // proportional gain (kP)
//                       0.5,   // integral gain (kI)
//                       1.75, // derivative gain (kD)
//                       3,   // anti windup
//                       0.75,   // small error range, in degrees
//                       100, // small error range timeout, in milliseconds
//                       1.5,   // large error range, in degrees
//                       300, // large error range timeout, in milliseconds
//                       0.5    // maximum acceleration (slew)
//     );

// Private default constructor
// Note: chassis is reconstructed in initialize() using placement-new
Drivetrain::Drivetrain() {}

// Static initializer: sets up motors, drivetrain, odom, and chassis singleton
void Drivetrain::initialize() {
  if (instance != nullptr)
    return;

  // Construct singleton instance (private ctor)
  instance = new Drivetrain();

  // Motor groups (ports/signs copied from RobotContainer)
  instance->rightGroup = std::make_unique<pros::MotorGroup>(
      std::initializer_list<std::int8_t>{11, 1, 17});
  instance->leftGroup = std::make_unique<pros::MotorGroup>(
      std::initializer_list<std::int8_t>{-13, -15, -19});

  instance->rightGroup->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  instance->leftGroup->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  // Lemlib drivetrain configuration object
  instance->lemlibDrivetrain = std::make_unique<lemlib::Drivetrain>(
      instance->leftGroup.get(), instance->rightGroup.get(), 12.194788f,
      lemlib::Omniwheel::NEW_275, 450, 2.0f);

  // Odom + IMU (matching previous RobotContainer config)
  static pros::Rotation horizontalRotation(20);
  static lemlib::TrackingWheel horizontalTrackingWheel(
      &horizontalRotation, lemlib::Omniwheel::NEW_2, -2);
  static pros::Imu imu(9);

  lemlib::OdomSensors sensors(nullptr, nullptr, &horizontalTrackingWheel,
                              nullptr, &imu);

  // Construct chassis in-place using placement-new (no copy/move/assign)
  instance->chassis = std::make_unique<lemlib::Chassis>(
      *instance->lemlibDrivetrain, lateralController, angularController, sensors);
  instance->chassis->calibrate();
  instance->chassis->setPose(0, 0, 0);
}

void Drivetrain::coordDisplayInit() {
    pros::lcd::initialize();

  pros::Task screen_task([&]() {
    while (true) {
      int i = 0;
      pros::lcd::print(i++, "X: %f", instance->chassis->getPose().x);
      pros::lcd::print(i++, "Y: %f", instance->chassis->getPose().y);
      pros::lcd::print(i++, "Theta: %f", instance->chassis->getPose().theta);
      pros::delay(20);

      //   std::cout << "Task Looped" << std::endl;
    }
  });

}

// Accessor for singleton instance (assumes initialize() called)
Drivetrain &Drivetrain::getInstance() {
  // No lazy initialization here to keep ports/config explicit via initialize()
  return *instance;
}

void Drivetrain::arcadeDrive(int forward, int turn) {
  chassis->arcade(forward, turn);
}

void Drivetrain::stop() { chassis->arcade(0, 0); }

void Drivetrain::periodic() {
  chassis->curvature(
      Globals::master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
      Globals::master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
}

void Drivetrain::simpleForward() {
  // chassis->setPose(0, 0, 0);
  // Print a debug message
  // std::cout << "Starting simpleForward autonomous routine." << std::endl;
  chassis->moveToPose(0, 24, 0, 10000);
  chassis->waitUntilDone();
  chassis->turnToHeading(180, 10000);
  chassis->waitUntilDone();
  // pros::c::delay(1000);
  chassis->turnToHeading(0, 10000);
  chassis->moveToPose(0, 0, 180, 10000);
  chassis->waitUntilDone();
}

void Drivetrain::leftAuton() {
  chassis->moveToPose(10.625, -30.126, -39.108, 10000, {.forwards = false});
  Intake::getInstance().intake();
  Globals::blocker.extend();
  chassis->waitUntilDone();
  // pros::delay(1000);
  chassis->turnToHeading(-30, 1000, {}, false);
  chassis->turnToHeading(-47, 1000, {}, false);
  chassis->turnToHeading(-42, 1000, {}, false);
  chassis->turnToHeading(-42, 1000, {}, false);
  Intake::getInstance().stop();
  chassis->moveToPose(10.818, 3.7545, 88.85, 10000);
  chassis->waitUntilDone();
  chassis->moveToPose(30.93, -11.90, 169.9, 10000);
  Globals::blocker.retract();
  pros::delay(100);
  Intake::getInstance().intake();
  EndEffector::getInstance().scoreTop();
  pros::delay(5000);
  
}
