#include "main.h"
#include "RobotContainer.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "subsystems/Drivetrain.h"
#include "ui/AutonSelector.h"


static RobotContainer *robotContainer = nullptr;

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

  // Create RobotContainer which wires subsystems and commands
  robotContainer = new RobotContainer();
  if (robotContainer) {
    // robotContainer->getAutonSelector()->initialize();
  }
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
  if (robotContainer) {
    auto routine = robotContainer->getAutonSelector()->getSelectedAuton();

    if (routine) {

      // Run the selected autonomous routine
      routine();
    }
  }
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
  // The RobotContainer has created subsystems and the default arcade command.
  // Run the default periodic behavior directly (no Scheduler).
  while (true) {
    // pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() &
    // LCD_BTN_LEFT) >> 2, 				 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
    // 				 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >>
    // 0);

    pros::lcd::print(
        0, "Auton running: %s",
        robotContainer->getAutonSelector()->getSelectedAutonName().c_str());

    // Log robot location using the drivetrain's chassis pose (if available)
    static int i = 1; // start from line 1; line 0 used above
    if (robotContainer != nullptr) {
        auto chassis = Drivetrain::getInstance().getChassis();
        if (chassis != nullptr) {
            auto pose = chassis->getPose();
            pros::lcd::print(i++, "AUTON RUNNING");
            pros::lcd::print(i++, "X: %f", pose.x);
            pros::lcd::print(i++, "Y: %f", pose.y);
            pros::lcd::print(i++, "Theta: %f", pose.theta);
        }
    }
    if (i > 7) i = 1; // keep lines bounded

    if (robotContainer)
      robotContainer->runPeriodic();
    pros::delay(10);

    if (true) {
      if (robotContainer->master.get_digital_new_press(
              pros::E_CONTROLLER_DIGITAL_DOWN)) {
        autonomous();
      }
    }
  }
}