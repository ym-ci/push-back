#pragma once
#include "liblvgl/lvgl.h"
#include "auton/AutonSequence.h"
#include <memory>
#include <vector>
#include <string>

/**
 * AutonSelector - A LVGL-based autonomous routine selector
 * 
 * Provides a graphical interface for selecting autonomous routines before a match.
 * Uses LVGL buttons and labels to display available autonomous options.
 */
class AutonSelector {
public:
    AutonSelector();
    ~AutonSelector();

    /**
     * Initialize the autonomous selector UI
     * Should be called in competition_initialize()
     */
    void initialize();

    /**
     * Add an autonomous routine to the selector
     * @param name Display name of the autonomous routine
     * @param command The command to run for this autonomous
     */
    void addAuton(const std::string& name, std::shared_ptr<AutonSequence> sequence);

    /**
     * Get the currently selected autonomous command
     * @return Shared pointer to the selected command, or nullptr if none selected
     */
    std::shared_ptr<AutonSequence> getSelectedAuton();

    /**
     * Get the name of the currently selected autonomous
     */
    std::string getSelectedAutonName();

    /**
     * Get the index of the currently selected autonomous
     */
    int getSelectedIndex() const { return m_selectedIndex; }

private:
    struct AutonOption {
        std::string name;
        std::shared_ptr<AutonSequence> sequence;
        lv_obj_t* button;
    };

    std::vector<AutonOption> m_autonOptions;
    int m_selectedIndex;
    lv_obj_t* m_screen;
    lv_obj_t* m_titleLabel;
    lv_obj_t* m_selectedLabel;

    static void buttonEventHandler(lv_event_t* e);
    void updateSelectedLabel();
};
