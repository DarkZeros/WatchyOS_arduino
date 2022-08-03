
#include "Watchy.h"
#include "NvsJson.h"

Watchy::Watchy()
: mDatabase()
, mDisplay(mDatabase)
{ 
    // Set up the interrupts to attend button presses

    // Insert the wake up reason in the event loop

    // Setup the Database

    
    // Prepare in another threads, the rest of the required stuff
    //std::thread
}

void Watchy::loop(){
    Serial.print("Loop Start ");
    Serial.println(micros()); Serial.flush();

    mDatabase.emplace("config", *NvsJson::create("config", false, kDefaultConfig));
    mDatabase.emplace("watchface", *NvsJson::create("watchface", false, kDefaultWatchFace));

    sol::state lua;
    int x = 0;
    Json& json = mDatabase.mNvsJson.at("config").get();
    lua.set_function("beep", [&json] (bool v){ json["display"]["inverted"] = v; });
    //lua.script("beep(false)");
    //lua.script("beep(true)");


    mDisplay.render();//.mNvsJson.at("config"), mDatabase.mNvsJson.at("watchface"), mDatabase.mNvsJson.at("watchface"));

    Serial.print("Loop End ");
    Serial.println(micros()); Serial.flush();

    esp_sleep_enable_timer_wakeup(10 * 1000000);
    //esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
    //esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
    //esp_sleep_enable_timer_wakeup(curTime * 1000000 - esp_rtc_get_time_us());
    esp_deep_sleep_start();
}