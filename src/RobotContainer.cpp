#include "RobotContainer.h"
#include "ui/AutonSelector.h"
#include "subsystems/Intake.h"
#include "subsystems/EndEffector.h"
#include "subsystems/Piston.h"

// Implementation members moved into RobotContainer header (no Pimpl)

RobotContainer::RobotContainer() {
    // Create MotorGroup directly from port lists (signed 8-bit values)
   rightGroup = std::make_unique<pros::MotorGroup>(std::initializer_list<std::int8_t>{11, 1, 17});
   leftGroup = std::make_unique<pros::MotorGroup>(std::initializer_list<std::int8_t>{-13, -15, -19});

    // Create lemlib drivetrain and chassis
   lemlibDrivetrain = std::make_unique<lemlib::Drivetrain>(leftGroup.get(),rightGroup.get(), 10.0f, lemlib::Omniwheel::NEW_325, 360.0f, 2.0f);
    
    // pros::Rotation constructor takes a single port parameter; use negative port for reversed sensors
    static pros::Rotation horizontalRotation(2); // adjust port (positive) or use -2 for reversed
    static lemlib::TrackingWheel horizontalTrackingWheel(&horizontalRotation, lemlib::Omniwheel::NEW_2, -2 /*distance: behind center*/);

    // IMU
    static pros::Imu imu(1); // adjust port as needed

    // Use nullptr for verticals if you prefer to rely on IMU + horizontal pod only.
    // Here we set vertical tracking wheel pointers to nullptr (no vertical shafts),
    // keep the horizontal pod, and provide the IMU.
    lemlib::OdomSensors sensors(nullptr, nullptr, &horizontalTrackingWheel, nullptr, &imu);
   chassis = std::make_unique<lemlib::Chassis>(*lemlibDrivetrain, lemlib::ControllerSettings(1,0,0,0,0,0,0,0,0), lemlib::ControllerSettings(1,0,0,0,0,0,0,0,0), sensors);

    // Create our Drivetrain wrapper passing the chassis pointer
   drivetrain = new Drivetrain(chassis.get());

    // Create intake motors and Intake subsystem.
    // NOTE: Adjust ports and reversed flags as needed for your robot.
    // Using ports 7 and 8 here as placeholders.
   intakeMotor = std::make_unique<pros::Motor>(4);
   middleMotor = std::make_unique<pros::Motor>(-10);
   intake = std::make_unique<Intake>(&master,intakeMotor.get(),middleMotor.get());

    // Create piston subsystem on ADI port E (toggle with X button)
    tounge = std::make_unique<Piston>(
        [this]() { return master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X ); },
        'E'
    );

    blocker = std::make_unique<Piston>(
        [this]() { return master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B ); },
        'D'
    );

        // Create end effector motor and subsystem.
    // NOTE: Adjust port and reversed flag as needed. Using port 9 as placeholder.
   endEffectorMotor = std::make_unique<pros::Motor>(21);
   endEffector = std::make_unique<EndEffector>(&master, endEffectorMotor.get(), blocker.get());


    // No command object — we'll run arcade control directly from the drivetrain
    
    // Create autonomous selector and autonomous commands
   autonSelector = std::make_unique<AutonSelector>();
    // Build autonomous sequences directly using AutonSequence
   redLeftAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(80,0); }, nullptr, nullptr, 2000},
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(0,-60); }, nullptr, nullptr, 1000},
        AutonStep{nullptr, [this]{drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
   blueRightAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(80,0); }, nullptr, nullptr, 2000},
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(0,60); }, nullptr, nullptr, 1000},
        AutonStep{nullptr, [this]{drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
    // Build skills auton sequence: forward 3s, back 3s, stop
   skillsAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(80,0); }, nullptr, nullptr, 3000},
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(-80,0); }, nullptr, nullptr, 3000},
        AutonStep{nullptr, [this]{drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
    
    // Build simple forward auton: drive forward for 1 second, then stop
   auto simpleForwardAuton = std::make_shared<AutonSequence>(std::vector<AutonStep>{
        AutonStep{nullptr, [this]{drivetrain->arcadeDrive(-80,0); }, nullptr, nullptr, 100},
        AutonStep{nullptr, [this]{drivetrain->stop(); }, [](){ return true; }, nullptr, 0}
    });
    
    // Add autonomous routines to the selector
//    autonSelector->addAuton("Red Left",redLeftAuton);
//    autonSelector->addAuton("Blue Right",blueRightAuton);
//    autonSelector->addAuton("Skills",skillsAuton);
   autonSelector->addAuton("Simple Forward",simpleForwardAuton);
}

RobotContainer::~RobotContainer() {
    delete drivetrain;
}

Drivetrain *RobotContainer::getDrivetrain() { return drivetrain; }

AutonSelector* RobotContainer::getAutonSelector() { return autonSelector.get(); }

void RobotContainer::runPeriodic() {
    // Run default behaviors: execute arcade drive and subsystem periodic
    if (drivetrain) {
       drivetrain->runTank(&master);
    }
    if (drivetrain) {
       drivetrain->periodic();
    }

    // Run intake periodic (reads controller buttons)
    if (intake) {
       intake->periodic();
    }

    // Run end effector periodic (L1/L2 control)
    if (endEffector) {
       endEffector->periodic();
    }

    // Run piston periodic (X button toggle)
    if (tounge) {
       tounge->periodic();
    }

    if (blocker) {
       blocker->periodic();
    }    
}
