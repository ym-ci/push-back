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
    Piston* getPiston() { return m_piston.get(); }
    // Call periodically from opcontrol to run default behaviors
    void runPeriodic();

private:
    Drivetrain *m_drivetrain;

    // Controller (value) used for default controls
    pros::Controller m_master{pros::E_CONTROLLER_MASTER};

    // Motor groups and lemlib objects owned by the container
    std::unique_ptr<pros::MotorGroup> m_leftGroup;
    std::unique_ptr<pros::MotorGroup> m_rightGroup;
    std::unique_ptr<lemlib::Drivetrain> m_lemlibDrivetrain;
    std::unique_ptr<lemlib::Chassis> m_chassis;

    // Intake subsystem: motors owned by RobotContainer and Intake instance
    std::unique_ptr<pros::Motor> m_intakeMotor;
    std::unique_ptr<pros::Motor> m_middleMotor;
    std::unique_ptr<class Intake> m_intake;
    // End effector motor and subsystem (L1/L2 control)
    std::unique_ptr<pros::Motor> m_endEffectorMotor;
    std::unique_ptr<class EndEffector> m_endEffector;

    // Piston subsystem (toggle with X button)
    std::unique_ptr<Piston> m_piston;

    // Autonomous selector and sequences
    std::unique_ptr<AutonSelector> m_autonSelector;
    std::shared_ptr<AutonSequence> m_redLeftAuton;
    std::shared_ptr<AutonSequence> m_blueRightAuton;
    std::shared_ptr<AutonSequence> m_skillsAuton;
};
