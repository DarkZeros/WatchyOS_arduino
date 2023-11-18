
#include "Watchy.h"
#include "NvsJson.h"
#include <future>

RTC_DATA_ATTR int sCounter = 0;

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
    /*Serial.print("Setup: Executing on core ");
    Serial.println(xPortGetCoreID());   
    //auto t1 = std::async(std::launch::async, [&](){
    //auto t1 = std::thread([&](){
        Serial.print("Init: Executing on core ");
    Serial.println(xPortGetCoreID());
        Serial.print("Init Start ");
        Serial.println(micros()); Serial.flush();
        mDisplay.init();
        Serial.print("Init done ");
        Serial.println(micros()); Serial.flush();
    //});
    
    Serial.print("Loop Start ");
    Serial.println(micros()); Serial.flush();

    mDatabase.emplace("config", *NvsJson::create("config", false, kDefaultConfig));
     Serial.print("FIrst JSON ");
    Serial.println(micros()); Serial.flush();

    mDatabase.emplace("watchface", *NvsJson::create("watchface", false, kDefaultWatchFace));
 Serial.print("Second JSON ");
    Serial.println(micros()); Serial.flush();

    
    sol::state lua;
    int x = 0;
    //Json& json = mDatabase.mNvsJson.at("config").get();
    //lua.set_function("beep", [&json] (bool v){ json["display"]["inverted"] = v; });
    //lua.script("beep(false)");
    //lua.script("beep(true)");
    Serial.print("Waitining... ");
    //a1.wait();
    Serial.print("Wait done ");*/
    //mDisplay.render();//.mNvsJson.at("config"), mDatabase.mNvsJson.at("watchface"), mDatabase.mNvsJson.at("watchface"));

    Serial.print("Loop End ");
    Serial.println(sCounter++);
    Serial.println(micros()); Serial.flush();

  /*struct timeval tv;
  if (gettimeofday(&tv, NULL)!= 0) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("TimeVal-cd sec = ");
  Serial.print(tv.tv_sec);
  Serial.print(" ");
  Serial.print("TimeVal-usec = ");
  Serial.print(tv.tv_usec);
  Serial.println();*/

    pinMode(13, OUTPUT);
    digitalWrite(13, sCounter % 2 ? HIGH : LOW);
    // gpio_hold_en((gpio_num_t)13);

    esp_sleep_enable_timer_wakeup(10 * 1000000);
    //esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
    //esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
    //esp_sleep_enable_timer_wakeup(curTime * 1000000 - esp_rtc_get_time_us());
    esp_deep_sleep_start();
}