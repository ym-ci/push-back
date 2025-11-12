#include "subsystems/Drivetrain.h"
#include "Intake.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/llemu.hpp"
#include "pros/motor_group.hpp"
#include "pros/imu.hpp"
#include "Globals.h"
#include <iostream>

// Static instance pointer
Drivetrain* Drivetrain::instance = nullptr;

lemlib::ControllerSettings lateralController(16,  // proportional gain (kP)
                                              1.5,   // integral gain (kI)
                                              0.4, // derivative gain (kD)
                                              3,   // anti windup
                                              1,   // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3,   // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              25   // maximum acceleration (slew)
);


// angular PID controller
lemlib::ControllerSettings angularController(2.5,    // proportional gain (kP)
                                              0,    // integral gain (kI)
                                              9.5, // derivative gain (kD)
                                              3,    // anti windup
                                              1,    // small error range, in degrees
                                              100,  // small error range timeout, in milliseconds
                                              2,    // large error range, in degrees
                                              500,  // large error range timeout, in milliseconds
                                              0     // maximum acceleration (slew)
);


// Private default constructor
// Note: chassis is reconstructed in initialize() using placement-new
Drivetrain::Drivetrain()
    : chassis(
        lemlib::Drivetrain(nullptr, nullptr, 0.0f, lemlib::Omniwheel::NEW_275, 0, 0.0f),
        lateralController,
        angularController,
        lemlib::OdomSensors(nullptr, nullptr, nullptr, nullptr, nullptr)
    ) {}

// Static initializer: sets up motors, drivetrain, odom, and chassis singleton
void Drivetrain::initialize() {
    if (instance != nullptr) return;

    // Construct singleton instance (private ctor)
    instance = new Drivetrain();

    // Motor groups (ports/signs copied from RobotContainer)
    instance->rightGroup = std::make_unique<pros::MotorGroup>(
        std::initializer_list<std::int8_t>{11, 1, 17});
    instance->leftGroup = std::make_unique<pros::MotorGroup>(
        std::initializer_list<std::int8_t>{-13, -15, -19});

    instance->rightGroup->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    instance->leftGroup->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    // Lemlib drivetrain configuration object
    instance->lemlibDrivetrain = std::make_unique<lemlib::Drivetrain>(
        instance->leftGroup.get(),
        instance->rightGroup.get(),
        12.194788f,
        lemlib::Omniwheel::NEW_275,
        450,
        2.0f
    );

    // Odom + IMU (matching previous RobotContainer config)
    static pros::Rotation horizontalRotation(20);
    static lemlib::TrackingWheel horizontalTrackingWheel(
        &horizontalRotation,
        lemlib::Omniwheel::NEW_2,
        -2
    );
    static pros::Imu imu(9);

    lemlib::OdomSensors sensors(
        nullptr,
        nullptr,
        &horizontalTrackingWheel,
        nullptr,
        &imu
    );

    // Construct chassis in-place using placement-new (no copy/move/assign)
    new (&instance->chassis) lemlib::Chassis(
        *instance->lemlibDrivetrain,
        lemlib::ControllerSettings(10, 0, 0, 3, 1, 100, 3, 500, 5),
        lemlib::ControllerSettings(2, 0, 10, 3, 1, 100, 3, 500, 5),
        sensors
    );

    instance->chassis.calibrate();
    instance->chassis.setPose(0, 0, 0);

    pros::lcd::initialize();

      pros::Task screen_task([&]() {
    while (true) {
      int i = 0;
      pros::lcd::print(i++, "X: %f", instance->chassis.getPose().x);
      pros::lcd::print(i++, "Y: %f", instance->chassis.getPose().y);
      pros::lcd::print(i++, "Theta: %f", instance->chassis.getPose().theta);
      pros::delay(20);

    //   std::cout << "Task Looped" << std::endl;
    }
  });

}

// Accessor for singleton instance (assumes initialize() called)
Drivetrain& Drivetrain::getInstance() {
    // No lazy initialization here to keep ports/config explicit via initialize()
    return *instance;
}

void Drivetrain::arcadeDrive(int forward, int turn) {
    chassis.arcade(forward, turn);
}

void Drivetrain::stop() {
    chassis.arcade(0, 0);
}

void Drivetrain::periodic() {
 chassis.curvature(Globals::master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), Globals::master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
}

    void Drivetrain::simpleForward() {
        // chassis.setPose(0, 0, 0);
        // Print a debug message
        std::cout << "Starting simpleForward autonomous routine." << std::endl;
        chassis.moveToPose(0, 24, 0, 10000);
        Intake::getInstance().intake();
        chassis.waitUntilDone();
        Intake::getInstance().stop();
        chassis.moveToPose(0, 0, 180, 10000);
        chassis.waitUntilDone();
    }
