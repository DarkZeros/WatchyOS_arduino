
#include "Arduino.h"
#include "Watchy.h"

Watchy kWatchy;

void setup(){
    Serial.begin(115200);
}

void loop(){
    kWatchy.loop();
}
/*

//file: main.cpp
#include "Arduino.h"
#include <time.h>

//#include "config.h"
//#include "EventLoop.h"
#include "ArduinoJson.h"

// All the data we are going to use across Deep Sleeps
RTC_DATA_ATTR struct 
{
    // Time related
    struct {
        time_t epoch;
    } time;

    // Watchy Config
    StaticJsonDocument<100> config;

} kStorage;




// Data that is valid for current run
std::queue<Event> pendingEvents;


RTC_DATA_ATTR std::list<uint64_t> pendingButtons;
RTC_DATA_ATTR tmElements_t storedTime;
RTC_DATA_ATTR bool second_boot = false;
int started;

Watchy::Watchy(){
    started = millis();

Watchy::Watchy(){} //constructor
    //Serial.begin(460800); Serial.println("Serial setup done"); 

    // Inmediately on construction set the ISRs to not lose further button presses
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(MENU_BTN_PIN), [](){ interrupt(0, MENU_BTN_PIN, MENU_BTN_MASK); }, RISING);
    attachInterrupt(digitalPinToInterrupt(BACK_BTN_PIN), [](){ interrupt(1, BACK_BTN_PIN, BACK_BTN_MASK); }, RISING);
    attachInterrupt(digitalPinToInterrupt(UP_BTN_PIN), [](){ interrupt(2, UP_BTN_PIN, UP_BTN_MASK); }, RISING);
    attachInterrupt(digitalPinToInterrupt(DOWN_BTN_PIN), [](){ interrupt(3, DOWN_BTN_PIN, DOWN_BTN_MASK); }, RISING);

}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

RTC_DATA_ATTR bool firstTimeSetup = false;
RTC_DATA_ATTR time_t curTime = 0;

#include "rtc.h"
#include "clk.h"

#include "ArduinoNvs.h"
#include "Config.h"

#include "SpiffsJson.h"
#include "NvsJson.h"
#include "Display.h"

#include <future>

Display d;

void setup(){
    Serial.begin(112500);
    //print_wakeup_reason();

    if (firstTimeSetup) {
        Serial.println("firsttime");
        firstTimeSetup = false;
        hw_timer_t * timer = NULL;
        timer = timerBegin(0, 160, true);
        timerAlarmWrite(timer, 2000000, true);
        timerSetAutoReload(timer, true);
        timerAlarmEnable(timer);
    }
    //struct tm timeinfo;

    //std::async(&Display::init, &d);
}

void loop(){
    curTime++;
    Serial.println(micros()); Serial.flush();

    //auto spiff = SpiffsJson::create("/config", false);
    //if (!spiff)
    //    Serial.println("failed to load spiff");
    //Serial.println(micros()); Serial.flush();

    //auto config = NvsBackedJson("watchy", "config24441", kDefaultConfig); // ~0.5ms //
    auto config = NvsJson::create("aseqef", true, Json::parse(kDefaultConfig, nullptr, false)); // ~0.9ms
    //auto config = NvsBackedJson3("watchy", "config23", kDefaultConfig); // ~0.6ms !!
    
    //Serial.println(config->get().dump(-1, ' ', true).c_str());
    Serial.println(micros()); Serial.flush();

    auto displ = Json::parse(kDefaultWatchFace);

    //std::async(&Display::render, &d, config, displ, displ);
    d.render(config->get(), displ, displ);

    //Serial.print(config.get().dump().c_str());
    //serializeJson(config.get(), Serial);

    curTime += 10;
    Serial.printf("RTC TIme %llu cal %u\n", esp_rtc_get_time_us(), esp_clk_slowclk_cal_get());
    Serial.printf("Time %ld\n", curTime);
    struct tm timeinfo;
    //getLocalTime(&timeinfo);
    localtime_r(&curTime, &timeinfo);
    Serial.println(&timeinfo, "%B %d %Y %H:%M:%S (%A)");

    Serial.println(micros()); Serial.flush();
    delay(2000);
    esp_sleep_enable_timer_wakeup(10 * 1000000);
    //esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
    //esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
    //esp_sleep_enable_timer_wakeup(curTime * 1000000 - esp_rtc_get_time_us());
    esp_deep_sleep_start();
}*/
