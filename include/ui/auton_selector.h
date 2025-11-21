#pragma once
#include "liblvgl/lvgl.h"

class AutonSelector {
public:
    static void init();
    static void runSelectedAuton();
    static int selectedAuton;
};
