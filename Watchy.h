
#pragma once

#define ARDUINO_WATCHY_V10
#include "Config.h"
#include "Database.h"
#include "Display.h"

#include <sol/sol.hpp>

class Watchy {
    Database mDatabase;
    Display mDisplay;
    

    void deepSleep();
    void lightSleep();

public:
    Watchy();


    void loop();
    
};