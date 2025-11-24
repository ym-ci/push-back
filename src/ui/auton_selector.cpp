#include "ui/auton_selector.h"
#include "subsystems/Drivetrain.h"
#include "pros/rtos.hpp"

int AutonSelector::selectedAuton = 0; // Default to 0

static lv_obj_t * dropdown;

static void event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        AutonSelector::selectedAuton = lv_dropdown_get_selected(obj);
    }
}

void AutonSelector::init() {
    // Create a dropdown
    dropdown = lv_dropdown_create(lv_screen_active());
    lv_dropdown_set_options(dropdown, "Red Left\nRed Right\nBlue Left\nBlue Right\nSkills\nSimple Forward\nNone");
    lv_obj_align(dropdown, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(dropdown, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

void AutonSelector::runSelectedAuton() {
    switch(selectedAuton) {
        case 0: // Red Left
            Drivetrain::leftAuton();
            break;
        case 1: // Red Right
            // Drivetrain::rightAuton();
            break;
        case 2: // Blue Left
            // Drivetrain::leftAuton(); // Maybe mirrored?
            break;
        case 3: // Blue Right
            // Drivetrain::rightAuton();
            break;
        case 4: // Skills
            // Drivetrain::skillsAuton();
            break;
        case 5: // Simple Forward
            Drivetrain::simpleForward();
            break;
        case 6: // None
        default:
            break;
    }
}
