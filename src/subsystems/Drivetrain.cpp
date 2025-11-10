#include "subsystems/Drivetrain.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/motor_group.hpp"
#include "pros/imu.hpp"

// Static instance pointer
Drivetrain* Drivetrain::instance = nullptr;

// Static initializer: sets up motors, drivetrain, odom, and chassis singleton
void Drivetrain::initialize() {
    if (instance != nullptr) return;

    instance = new Drivetrain();

    // Motor groups (ports/signs copied from RobotContainer)
    instance->rightGroup = std::make_unique<pros::MotorGroup>(
        std::initializer_list<std::int8_t>{11, 1, 17});
    instance->leftGroup = std::make_unique<pros::MotorGroup>(
        std::initializer_list<std::int8_t>{-13, -15, -19});

    instance->rightGroup->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    instance->leftGroup->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    // Lemlib drivetrain
    instance->lemlibDrivetrain = std::make_unique<lemlib::Drivetrain>(
        instance->leftGroup.get(),
        instance->rightGroup.get(),
        10.0f,
        lemlib::Omniwheel::NEW_275,
        450,
        2.0f
    );

    // Odom + IMU (matching previous RobotContainer config)
    static pros::Rotation horizontalRotation(2);
    static lemlib::TrackingWheel horizontalTrackingWheel(
        &horizontalRotation,
        lemlib::Omniwheel::NEW_2,
        -2
    );
    static pros::Imu imu(1);

    lemlib::OdomSensors sensors(
        nullptr,
        nullptr,
        &horizontalTrackingWheel,
        nullptr,
        &imu
    );

    instance->chassisOwned = std::make_unique<lemlib::Chassis>(
        *instance->lemlibDrivetrain,
        lemlib::ControllerSettings(10, 0, 0, 0, 0, 0, 0, 0, 0),
        lemlib::ControllerSettings(1, 0, 0, 0, 0, 0, 0, 0, 0),
        sensors
    );

    instance->chassis = instance->chassisOwned.get();
}

// Accessor for singleton instance (assumes initialize() called)
Drivetrain& Drivetrain::getInstance() {
    // No lazy initialization here to keep ports/config explicit via initialize()
    return *instance;
}

void Drivetrain::arcadeDrive(int forward, int turn) {
    if (chassis)
        chassis->arcade(forward, turn);
}

void Drivetrain::stop() {
    if (chassis)
        chassis->arcade(0, 0);
}

void Drivetrain::periodic() {
    // LemLib chassis handles its own internal periodic updates if needed
}

void Drivetrain::runArcade(pros::Controller* controller) {
    if (!controller) return;
    int forward = controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    arcadeDrive(forward, turn);
}

void Drivetrain::runTank(pros::Controller* controller) {
    if (!controller) return;
    int left = controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int right = controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    if (chassis)
        chassis->tank(left, right);
}
