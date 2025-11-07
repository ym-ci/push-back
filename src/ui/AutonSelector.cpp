#include "ui/AutonSelector.h"
#include "pros/screen.hpp"

AutonSelector::AutonSelector() :selectedIndex(0), screen(nullptr), 
                                 titleLabel(nullptr), dropdown(nullptr) {
}

AutonSelector::~AutonSelector() {
    // LVGL will clean up objects when the screen is deleted
}

void AutonSelector::initialize() {
    // Create a new screen for the autonomous selector
    screen = lv_obj_create(NULL);
    
    // Create title label at the top
    titleLabel = lv_label_create(screen);
    lv_label_set_text(titleLabel, "Select Autonomous:");
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 20);
    
    // Create dropdown widget
    dropdown = lv_dropdown_create(screen);
    lv_obj_set_size(dropdown, 240, 50);
    lv_obj_align(dropdown, LV_ALIGN_TOP_MID, 0, 60);
    
    // Add all autonomous options to the dropdown
    std::string options;
    for (size_t i = 0; i < autonOptions.size(); i++) {
        if (i > 0) options += "\n";
        options += autonOptions[i].name;
    }
    lv_dropdown_set_options(dropdown, options.c_str());
    
    // Set event callback
    lv_obj_add_event_cb(dropdown, dropdownEventHandler, LV_EVENT_VALUE_CHANGED, this);
    
    // Load the screen
    lv_screen_load(screen);
    
    // Select first option by default
    if (!autonOptions.empty()) {
        selectedIndex = 0;
        lv_dropdown_set_selected(dropdown, 0);
    }
}

void AutonSelector::addAuton(const std::string& name, AutonFunction routine) {
    AutonOption option;
    option.name = name;
    option.routine = routine;
    autonOptions.push_back(option);
}

AutonSelector::AutonFunction AutonSelector::getSelectedAuton() {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(autonOptions.size())) {
        return autonOptions[selectedIndex].routine;
    }
    return nullptr;
}

std::string AutonSelector::getSelectedAutonName() {
    if (selectedIndex >= 0 &&selectedIndex < static_cast<int>(autonOptions.size())) {
        return autonOptions[selectedIndex].name;
    }
    return "None";
}

void AutonSelector::dropdownEventHandler(lv_event_t* e) {
    AutonSelector* selector = static_cast<AutonSelector*>(lv_event_get_user_data(e));
    lv_obj_t* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(e));
    
    uint16_t selected = lv_dropdown_get_selected(dropdown);
    selector->selectedIndex = selected;
}
