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

static pros::Rotation horizontalRotation(-20);
static lemlib::TrackingWheel horizontalTrackingWheel(&horizontalRotation, lemlib::Omniwheel::NEW_2, 5.25);
static pros::Imu imu(9);

static lemlib::OdomSensors s_sensors(nullptr, nullptr, &horizontalTrackingWheel, nullptr, &imu);

static lemlib::ControllerSettings s_lateralController(
  30,  // proportional gain (kP)
  10,   // integral gain (kI)
  0,   // derivative gain (kD)
  20,   // anti windup
  0.25,   // small error range, in inches
  200,   // small error range timeout, in milliseconds
  0.75,   // large error range, in inches
  400,   // large error range timeout, in milliseconds
  10    // maximum acceleration (slew)
);

static lemlib::ControllerSettings s_angularController(
  2,  // proportional gain (kP)
  0.1,  // integral gain (kI)
  0.5, // derivative gain (kD)
  10,    // anti windup
  1, // small error range, in degrees
  100,  // small error range timeout, in milliseconds
  2,  // large error range, in degrees
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
  chassis.moveToPose(0, -12, 0, 10000, {.forwards = false});
  // chassis.waitUntilDone();
  // chassis.moveToPose(0, 0, 180, 10000);
  chassis.waitUntilDone();
  // // pros::c::delay(1000);
  // chassis.turnToHeading(0, 10000);
  chassis.turnToHeading(180, 10000);
  chassis.waitUntilDone();
}

void Drivetrain::leftAuton() {
  Intake::getInstance().intake();
  EndEffector::getInstance().intake();
  chassis.moveToPose(0, -15, 0, 2000, {.forwards = false, .minSpeed=40});
  chassis.waitUntilDone();
  chassis.moveToPose(0, -26, 0, 2000, {.forwards = false, .maxSpeed=40, .minSpeed=20});
  chassis.waitUntilDone();
  pros::delay(20);
  chassis.moveToPose(0, -36, 0, 2000, {.forwards = false, .maxSpeed=20});
  chassis.waitUntilDone();
  pros::delay(500);
  chassis.moveToPose(7.12, -8.5, 99.5, 2000, {.minSpeed=80});
  chassis.waitUntilDone();
  chassis.moveToPose(25.25, -26, 187.3, 2000);
  chassis.waitUntilDone();
  Intake::getInstance().score();
  EndEffector::getInstance().scoreTop();
  pros::delay(5000);
}

void Drivetrain::rightAuton() {
  // chassis.calibrate();
  // Intake the group of three 
  Intake::getInstance().intake();
  EndEffector::getInstance().intake();
  chassis.moveToPose(0, -15, 0, 2000, {.forwards = false, .minSpeed=40});
  chassis.waitUntilDone();
  chassis.moveToPose(0, -26, 0, 2000, {.forwards = false, .maxSpeed=40, .minSpeed=20});
  chassis.waitUntilDone();
  pros::delay(20);
  chassis.moveToPose(0, -38, 0, 2000, {.forwards = false, .maxSpeed=20});
  chassis.waitUntilDone();
  pros::delay(500);

  // Big curve to the right to align with the goal 
  chassis.moveToPose(-7.12, -8.5, 260.5, 2500, {.minSpeed=80});
  chassis.waitUntilDone();
  // Just so we dont over do it
  Intake::getInstance().stop();
  EndEffector::getInstance().stop();
  chassis.moveToPose(-27, -20, -191, 5000, {.minSpeed=30});
  chassis.waitUntilDone();
  chassis.moveToPose(-25.75, -29, -191, 750);
  chassis.waitUntilDone();
  // Force the alignment and unstuck some balls
  chassis.arcade(100, 0);
  Intake::getInstance().outtake();
  EndEffector::getInstance().outtake();
  pros::delay(100);
  Intake::getInstance().stop();
  EndEffector::getInstance().stop();
  pros::delay(400);
  chassis.arcade(0, 0);
  // Score the preloaded balls
  Intake::getInstance().score();
  EndEffector::getInstance().scoreTop();
  pros::delay(2000);
  Intake::getInstance().stop();
  EndEffector::getInstance().stop();
  // Because we have aligned we can assume a new know pose
  chassis.setPose(0, 0, chassis.getPose().theta);
  // Go get more balls at the match loader
  Globals::tounge.extend();
  // literally do it manually because path planner is being dumb
  chassis.arcade(-80, 0);
  pros::delay(2000);
  chassis.arcade(0, 0);
  Intake::getInstance().intake();
  EndEffector::getInstance().intake();
  // jiggle the balls a bit and intake
  Globals::tounge.retract();
  chassis.turnToHeading(-180, 500);
  chassis.waitUntilDone();
  Globals::tounge.extend();
  chassis.turnToHeading(-200, 500);
  chassis.waitUntilDone();
  pros::delay(1500);
  // go back to scoring position
  chassis.moveToPose(0, 0, -191, 2500);
  chassis.waitUntilDone();
  // Force the alignment again
  chassis.arcade(100, 0);
  pros::delay(500);
  chassis.arcade(0, 0);
  // Score the balls
  Intake::getInstance().score();
  EndEffector::getInstance().scoreTop();
  pros::delay(10000);

}
