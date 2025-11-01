#include "ui/AutonSelector.h"
#include "pros/screen.hpp"

AutonSelector::AutonSelector() :selectedIndex(0),screen(nullptr), 
                                 titleLabel(nullptr),selectedLabel(nullptr) {
}

AutonSelector::~AutonSelector() {
    // LVGL will clean up objects when the screen is deleted
}

void AutonSelector::initialize() {
    // Create a new screen for the autonomous selector
   screen = lv_obj_create(NULL);
    
    // Create title label at the top
   titleLabel = lv_label_create(screen);
    lv_label_set_text(titleLabel, "Autonomous Selector");
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 10);
    
    // Create selected label below title
   selectedLabel = lv_label_create(screen);
    lv_label_set_text(selectedLabel, "Selected: None");
    lv_obj_set_style_text_font(selectedLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(selectedLabel, LV_ALIGN_TOP_MID, 0, 40);
    
    // Create buttons for each autonomous option
    int yOffset = 80;
    for (size_t i = 0; i <autonOptions.size(); i++) {
        lv_obj_t* btn = lv_button_create(screen);
        lv_obj_set_size(btn, 200, 50);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, yOffset);
        
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label,autonOptions[i].name.c_str());
        lv_obj_center(label);
        
        // Store the index in user_data so we can retrieve it in the event handler
        lv_obj_set_user_data(btn, (void*)i);
        lv_obj_add_event_cb(btn, buttonEventHandler, LV_EVENT_CLICKED, this);
        
       autonOptions[i].button = btn;
        yOffset += 60;
    }
    
    // Load the screen
    lv_screen_load(screen);
    
    // Select first option by default if available
    if (!autonOptions.empty()) {
       selectedIndex = 0;
        updateSelectedLabel();
    }
}

void AutonSelector::addAuton(const std::string& name, std::shared_ptr<AutonSequence> sequence) {
    AutonOption option;
    option.name = name;
    option.sequence = sequence;
    option.button = nullptr;  // Will be created in initialize()
   autonOptions.push_back(option);
}

std::shared_ptr<AutonSequence> AutonSelector::getSelectedAuton() {
    if (selectedIndex >= 0 &&selectedIndex < static_cast<int>(autonOptions.size())) {
        return autonOptions[selectedIndex].sequence;
    }
    return nullptr;
}

std::string AutonSelector::getSelectedAutonName() {
    if (selectedIndex >= 0 &&selectedIndex < static_cast<int>(autonOptions.size())) {
        return autonOptions[selectedIndex].name;
    }
    return "None";
}

void AutonSelector::buttonEventHandler(lv_event_t* e) {
    AutonSelector* selector = static_cast<AutonSelector*>(lv_event_get_user_data(e));
    lv_obj_t* btn = static_cast<lv_obj_t*>(lv_event_get_target(e));
    size_t index = (size_t)lv_obj_get_user_data(btn);
    
    selector->selectedIndex = index;
    selector->updateSelectedLabel();
}

void AutonSelector::updateSelectedLabel() {
    if (selectedLabel &&selectedIndex >= 0 && 
       selectedIndex < static_cast<int>(autonOptions.size())) {
        std::string text = "Selected: " +autonOptions[selectedIndex].name;
        lv_label_set_text(selectedLabel, text.c_str());
    }
}
