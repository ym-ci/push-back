#include "ui/AutonSelector.h"
#include "pros/screen.hpp"

AutonSelector::AutonSelector() : m_selectedIndex(0), m_screen(nullptr), 
                                  m_titleLabel(nullptr), m_selectedLabel(nullptr) {
}

AutonSelector::~AutonSelector() {
    // LVGL will clean up objects when the screen is deleted
}

void AutonSelector::initialize() {
    // Create a new screen for the autonomous selector
    m_screen = lv_obj_create(NULL);
    
    // Create title label at the top
    m_titleLabel = lv_label_create(m_screen);
    lv_label_set_text(m_titleLabel, "Autonomous Selector");
    lv_obj_set_style_text_font(m_titleLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(m_titleLabel, LV_ALIGN_TOP_MID, 0, 10);
    
    // Create selected label below title
    m_selectedLabel = lv_label_create(m_screen);
    lv_label_set_text(m_selectedLabel, "Selected: None");
    lv_obj_set_style_text_font(m_selectedLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(m_selectedLabel, LV_ALIGN_TOP_MID, 0, 40);
    
    // Create buttons for each autonomous option
    int yOffset = 80;
    for (size_t i = 0; i < m_autonOptions.size(); i++) {
        lv_obj_t* btn = lv_button_create(m_screen);
        lv_obj_set_size(btn, 200, 50);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, yOffset);
        
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, m_autonOptions[i].name.c_str());
        lv_obj_center(label);
        
        // Store the index in user_data so we can retrieve it in the event handler
        lv_obj_set_user_data(btn, (void*)i);
        lv_obj_add_event_cb(btn, buttonEventHandler, LV_EVENT_CLICKED, this);
        
        m_autonOptions[i].button = btn;
        yOffset += 60;
    }
    
    // Load the screen
    lv_screen_load(m_screen);
    
    // Select first option by default if available
    if (!m_autonOptions.empty()) {
        m_selectedIndex = 0;
        updateSelectedLabel();
    }
}

void AutonSelector::addAuton(const std::string& name, std::shared_ptr<AutonSequence> sequence) {
    AutonOption option;
    option.name = name;
    option.sequence = sequence;
    option.button = nullptr;  // Will be created in initialize()
    m_autonOptions.push_back(option);
}

std::shared_ptr<AutonSequence> AutonSelector::getSelectedAuton() {
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_autonOptions.size())) {
        return m_autonOptions[m_selectedIndex].sequence;
    }
    return nullptr;
}

std::string AutonSelector::getSelectedAutonName() {
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_autonOptions.size())) {
        return m_autonOptions[m_selectedIndex].name;
    }
    return "None";
}

void AutonSelector::buttonEventHandler(lv_event_t* e) {
    AutonSelector* selector = static_cast<AutonSelector*>(lv_event_get_user_data(e));
    lv_obj_t* btn = static_cast<lv_obj_t*>(lv_event_get_target(e));
    size_t index = (size_t)lv_obj_get_user_data(btn);
    
    selector->m_selectedIndex = index;
    selector->updateSelectedLabel();
}

void AutonSelector::updateSelectedLabel() {
    if (m_selectedLabel && m_selectedIndex >= 0 && 
        m_selectedIndex < static_cast<int>(m_autonOptions.size())) {
        std::string text = "Selected: " + m_autonOptions[m_selectedIndex].name;
        lv_label_set_text(m_selectedLabel, text.c_str());
    }
}
