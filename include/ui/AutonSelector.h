#pragma once
#include "liblvgl/lvgl.h"
#include <functional>
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
    using AutonFunction = std::function<void()>;

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
     * @param routine Function pointer to the autonomous routine
     */
    void addAuton(const std::string& name, AutonFunction routine);

    /**
     * Get the currently selected autonomous routine
     * @return Function pointer to the selected routine, or nullptr if none selected
     */
    AutonFunction getSelectedAuton();

    /**
     * Get the name of the currently selected autonomous
     */
    std::string getSelectedAutonName();

    /**
     * Get the index of the currently selected autonomous
     */
    int getSelectedIndex() const { return selectedIndex; }

private:
    struct AutonOption {
        std::string name;
        AutonFunction routine;
    };

    std::vector<AutonOption>autonOptions;
    int selectedIndex;
    lv_obj_t*screen;
    lv_obj_t*titleLabel;
    lv_obj_t*dropdown;

    static void dropdownEventHandler(lv_event_t* e);
};
