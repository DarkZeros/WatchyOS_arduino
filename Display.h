
/*
 * Display Renderer for Watchy
 *
 * Takes care of display init/update
 * as well as optimizations
 * It unpacks the JSON data and renders
 * accordingly
*/

#pragma once

#include <GxEPD2_BW.h>
#include <mutex>

#include "Database.h"

extern RTC_DATA_ATTR bool kDisplayFullInit;

struct Rect {
    int16_t t, b, l, r;
};

class Display {
    static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
    Database& mDB;

    bool mInit = false;
    std::mutex mMutex;

public:
    explicit Display(Database& db) : mDB(db){}

    void init();
    void render();
    
};

