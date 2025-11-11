#include "main.h"
#include "Globals.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "subsystems/Drivetrain.h"
#include "ui/AutonSelector.h"

static auto& chassis = Drivetrain::getInstance().chassis;


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();

  chassis.calibrate();
  chassis.setPose(0, 0, 0);

  // Initialize global wiring (subsystems, pistons, auton selector)
  Globals::init();

  // autonSelector is an object, not a pointer
  Globals::autonSelector.initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // Initialize the autonomous selector UI
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  auto selector = Globals::autonSelector;

  auto routine = selector.getSelectedAuton();
  if (routine) {
    routine();
  }

  pros::Task screen_task([&]() {
    while (true) {
      int i = 0;
      pros::lcd::print(i++, "AUTON RUNNING");
      pros::lcd::print(i++, "X: %f", chassis.getPose().x);
      pros::lcd::print(i++, "Y: %f", chassis.getPose().y);
      pros::lcd::print(i++, "Theta: %f", chassis.getPose().theta);
      pros::delay(20);
    }
  });
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  while (true) {
    // Access the selector as an object (Globals::autonSelector is not a pointer)
    const std::string selectedName = Globals::autonSelector.getSelectedAutonName();
    const char* name = selectedName.c_str(); // valid while selectedName is in scope

    pros::lcd::print(0, "Auton running: %s", name);

    Globals::periodic();
    pros::delay(10);

    if (Globals::master.get_digital_new_press(
            pros::E_CONTROLLER_DIGITAL_DOWN)) {
      autonomous();
    }
  }
}