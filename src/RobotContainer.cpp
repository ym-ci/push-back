#include "RobotContainer.h"
#include "ui/AutonSelector.h"
#include "subsystems/Intake.h"
#include "subsystems/EndEffector.h"
#include "subsystems/Piston.h"

// Implementation members moved into RobotContainer header (no Pimpl)

RobotContainer::RobotContainer() {
    // Create MotorGroup directly from port lists (signed 8-bit values)
    m_leftGroup = std::make_unique<pros::MotorGroup>(std::initializer_list<std::int8_t>{-11, -1, -17});
    m_rightGroup = std::make_unique<pros::MotorGroup>(std::initializer_list<std::int8_t>{13, 15, 19});

    // Create lemlib drivetrain and chassis
    m_lemlibDrivetrain = std::make_unique<lemlib::Drivetrain>(m_leftGroup.get(), m_rightGroup.get(), 10.0f, lemlib::Omniwheel::NEW_325, 360.0f, 2.0f);
    
    // pros::Rotation constructor takes a single port parameter; use negative port for reversed sensors
    static pros::Rotation horizontalRotation(2); // adjust port (positive) or use -2 for reversed
    static lemlib::TrackingWheel horizontalTrackingWheel(&horizontalRotation, lemlib::Omniwheel::NEW_2, -2 /*distance: behind center*/);

    // IMU
    static pros::Imu imu(1); // adjust port as needed

    // Use nullptr for verticals if you prefer to rely on IMU + horizontal pod only.
    // Here we set vertical tracking wheel pointers to nullptr (no vertical shafts),
    // keep the horizontal pod, and provide the IMU.
    lemlib::OdomSensors sensors(nullptr, nullptr, &horizontalTrackingWheel, nullptr, &imu);
    m_chassis = std::make_unique<lemlib::Chassis>(*m_lemlibDrivetrain, lemlib::ControllerSettings(1,0,0,0,0,0,0,0,0), lemlib::ControllerSettings(1,0,0,0,0,0,0,0,0), sensors);

    // Create our Drivetrain wrapper passing the chassis pointer
    m_drivetrain = new Drivetrain(m_chassis.get());

    // Create intake motors and Intake subsystem.
    // NOTE: Adjust ports and reversed flags as needed for your robot.
    // Using ports 7 and 8 here as placeholders.
    m_intakeMotor = std::make_unique<pros::Motor>(4);
    m_middleMotor = std::make_unique<pros::Motor>(-10);
    m_intake = std::make_unique<Intake>(&m_master, m_intakeMotor.get(), m_middleMotor.get());

    // Create end effector motor and subsystem.
    // NOTE: Adjust port and reversed flag as needed. Using port 9 as placeholder.
    m_endEffectorMotor = std::make_unique<pros::Motor>(21);
    m_endEffector = std::make_unique<EndEffector>(&m_master, m_endEffectorMotor.get());

    // Create piston subsystem on ADI port E (toggle with X button)
    m_piston = std::make_unique<Piston>(
        [this]() { return m_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X ); },
        'E'
    );

    // No command object — we'll run arcade control directly from the drivetrain
    
    // Create autonomous selector and autonomous commands
    m_autonSelector = std::make_unique<AutonSelector>();
    // Build autonomous sequences directly using AutonSequence
    m_redLeftAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{ m_drivetrain->arcadeDrive(80,0); }, nullptr, nullptr, 2000},
        AutonStep{nullptr, [this]{ m_drivetrain->arcadeDrive(0,-60); }, nullptr, nullptr, 1000},
        AutonStep{nullptr, [this]{ m_drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
    m_blueRightAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{ m_drivetrain->arcadeDrive(80,0); }, nullptr, nullptr, 2000},
        AutonStep{nullptr, [this]{ m_drivetrain->arcadeDrive(0,60); }, nullptr, nullptr, 1000},
        AutonStep{nullptr, [this]{ m_drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
    // Build skills auton sequence: forward 3s, back 3s, stop
    m_skillsAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{ m_drivetrain->arcadeDrive(80,0); }, nullptr, nullptr, 3000},
        AutonStep{nullptr, [this]{ m_drivetrain->arcadeDrive(-80,0); }, nullptr, nullptr, 3000},
        AutonStep{nullptr, [this]{ m_drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
    
    // Add autonomous routines to the selector
    m_autonSelector->addAuton("Red Left", m_redLeftAuton);
    m_autonSelector->addAuton("Blue Right", m_blueRightAuton);
    m_autonSelector->addAuton("Skills", m_skillsAuton);
}

RobotContainer::~RobotContainer() {
    delete m_drivetrain;
}

Drivetrain *RobotContainer::getDrivetrain() { return m_drivetrain; }

AutonSelector* RobotContainer::getAutonSelector() { return m_autonSelector.get(); }

void RobotContainer::runPeriodic() {
    // Run default behaviors: execute arcade drive and subsystem periodic
    if (m_drivetrain) {
        m_drivetrain->runArcade(&m_master);
    }
    if (m_drivetrain) {
        m_drivetrain->periodic();
    }

    // Run intake periodic (reads controller buttons)
    if (m_intake) {
        m_intake->periodic();
    }

    // Run end effector periodic (L1/L2 control)
    if (m_endEffector) {
        m_endEffector->periodic();
    }

    // Run piston periodic (X button toggle)
    if (m_piston) {
        m_piston->periodic();
    }

    
}
