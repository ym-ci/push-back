#pragma once
#include "subsystems/Drivetrain.h"
#include "pros/misc.hpp" // for pros::Controller
#include <memory>

class AutonSelector;
class AutonSequence;
class Piston;

#include "pros/motor_group.hpp"
namespace lemlib { class Drivetrain; class Chassis; }

class RobotContainer {
public:
    RobotContainer();
    ~RobotContainer();
    Drivetrain *getDrivetrain();
    AutonSelector* getAutonSelector();
    Piston* getTounge() { return tounge.get(); }
    Piston* getBlocker() { return blocker.get(); }
    // Call periodically from opcontrol to run default behaviors
    void runPeriodic();
    pros::Controller master{pros::E_CONTROLLER_MASTER};
    std::unique_ptr<lemlib::Chassis>chassis;


private:
    Drivetrain *drivetrain;

    // Controller (value) used for default controls

    // Motor groups and lemlib objects owned by the container
    std::unique_ptr<pros::MotorGroup>leftGroup;
    std::unique_ptr<pros::MotorGroup>rightGroup;
    std::unique_ptr<lemlib::Drivetrain>lemlibDrivetrain;
    

    // Intake subsystem: motors owned by RobotContainer and Intake instance
    std::unique_ptr<pros::Motor>intakeMotor;
    std::unique_ptr<pros::Motor>middleMotor;
    std::unique_ptr<class Intake>intake;
    // End effector motor and subsystem (L1/L2 control)
    std::unique_ptr<pros::Motor>endEffectorMotor;
    std::unique_ptr<class EndEffector>endEffector;

    // Piston subsystem (toggle with X button)
    std::unique_ptr<Piston>tounge;
    std::unique_ptr<Piston>blocker;

    // Autonomous selector and sequences
    std::unique_ptr<AutonSelector>autonSelector;
    std::shared_ptr<AutonSequence>redLeftAuton;
    std::shared_ptr<AutonSequence>blueRightAuton;
    std::shared_ptr<AutonSequence>skillsAuton;
};
