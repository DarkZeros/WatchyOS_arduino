
#include "Display.h"
#include "NvsJson.h"

#include <thread>

#define CS 5 // Same // SS
#define DC 18 // GPIO10-Pin29
#define RESET 23 // GPIO9-PIN28
#define BUSY 19 // Same
// GPIO9-SD2-pin28 // MOSI GPIO23-pin36
// GPIO10-SD3-pin29 // SCK  GPIO18-pin35

int kCounter = 0;

bool kDisplayFullInit = true;

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> Display::display(GxEPD2_154_D67(CS, DC, RESET, BUSY));

static void busyCallBack(const void *) {
    gpio_wakeup_enable((gpio_num_t)BUSY, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
}

void Display::init() {
    //std::unique_lock<std::mutex> lock(mMutex);
    if (!mInit) {
        mInit = true;
        display.init(115200, kDisplayFullInit, 10, true, SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
        // SPI.begin(10, -1, 9, 5);
        display.epd2.setBusyCallback(busyCallBack);
    }
}

void Display::render() {
    //std::unique_lock<std::mutex> lock(mMutex);
    // We need to init, if we haven't done it yet
    init();

    // get from the DB the nodes we need
    auto config = mDB.get("config");
    auto data = mDB.get("data");
    auto render = mDB.get("watchface");

    // Select if we are in normal, or inverted mode
    const auto inverted = config["display"]["inverted"].bool_or(false);
    const auto foreColor = !inverted ? GxEPD_BLACK : GxEPD_WHITE;
    const auto backColor = !inverted ? GxEPD_WHITE : GxEPD_BLACK;

    // Clear previous content
    display.epd2.darkBorder = true;
    display.fillScreen(backColor);
    
    // Check if we have fonts

    // For each element recursively traverse
    enum class ListType {
        NONE,
        V,
        H
    };

    Serial.printf("Start ");
    Serial.println(micros()); Serial.flush();

    std::function<void(const JsonOpt&, Rect, ListType)> traverse = 
    [&](const JsonOpt& cur, Rect r, ListType list){
        int16_t xTotal = 0;
        int16_t yTotal = 0;
        for (auto& [k, val] : cur.value_or(Json()).items()) {
            if (k.c_str()[0] == '_')
                continue;
            auto v = JsonOpt{{val}};
            
            // Process element
            auto type = v["type"];
            if (!type || !type.is_string())
                continue;

            auto typeStr = v["type"].mOpt->get<std::string>();

            // get sizes, and common params
            const auto x = v["x"].int_or(0);
            const auto y = v["y"].int_or(0);
            const auto w = v["w"].int_or(1);
            const auto h = v["h"].int_or(1);
            auto size = v["size"].int_or(-1);
            const auto fill = v["fill"].bool_or(false);
            const auto cInt = v["color"].int_or(-1);
            const auto color = cInt == -1 ? foreColor : cInt == 0 ? GxEPD_BLACK : GxEPD_WHITE;

            // For each type specific
            if (typeStr == "circ") {
                const auto rad = v["r"].int_or(std::min(x, y));
                const auto q = v["q"].int_or(0xF);
                const auto d = v["d"].int_or(0);
                if (size < 0) {
                    if (list == ListType::V)
                        size = y + rad;
                    if (list == ListType::H)
                        size = x + rad;
                }

                //std::async(std::launch::async, [&](){
                std::thread([&](){
                if (fill) {
                    if (q == 0xF)
                        display.fillCircle(r.l + x, r.t + y, rad, color);
                    else
                        display.fillCircleHelper(r.l + x, r.t + y, rad, q, d, color);
                } else {
                    if (q == 0xF){
                        display.drawCircle(r.l + x, r.t + y, rad, color);
                    } else {
                        display.drawCircleHelper(r.l + x, r.t + y, rad, q, color);
                    }
                }
                }).detach();
            } else if (typeStr == "rect") {
                if (size < 0) {
                    if (list == ListType::V)
                        size = y + h;
                    if (list == ListType::H)
                        size = x + w;
                }

                if (fill)
                    display.fillRect(r.l + x, r.t + y, w, h, color);
                else
                    display.drawRect(r.l + x, r.t + y, w, h, color);
            } else if (typeStr == "bitmap") {
                auto data = v["data"];
                auto jsd = Json::binary({0xCA, 0xFE, 0xBA, 0xBE}, 23);
                //if (data && data.mOpt->is_binary())
                //    display.drawBitmap(r.l + x, r.t + y, data.mOpt->get_binary().data(), w, h, color);
                if (data && jsd.is_binary())
                    display.drawBitmap(r.l + x, r.t + y, jsd.get_binary().data(), w, h, color);
                
            }  else if (typeStr == "text") {
                auto text = v["text"];
                display.setCursor(r.l + x, r.t + y);
                //display.setFont();
                display.setTextColor(color);
                if (text && text.is_string()) {
                    display.print(text.mOpt->get_ref<const std::string&>().c_str());
                    display.print(kCounter++);
                }
            }

            // Add the list offset
            if (list == ListType::V)
                r.t += size;
            if (list == ListType::H)
                r.l += size;

            if (v["sub"]) {
                auto subRect = r;
                subRect.l += 20;
                traverse(v["sub"], subRect, ListType::V);
            }

            Serial.printf("Rendered %s ", typeStr.c_str());
            Serial.println(micros()); Serial.flush();
        }
    };
    traverse(render, {0, display.height(), 0, display.width()}, ListType::V);
    
    
    /*if (json.contains("tree"))
        traverse(json["tree"], )
    for (auto& i : json) {
        if (!i.count("type"))
            continue;
        auto& type = i["type"];
        if (type == "circle")
            display.drawCircle(i["x"], i["y"], i["r"], GxEPD_BLACK);
        if (type == "text") {
            display.setTextColor(GxEPD_BLACK);
            display.setCursor(i["x"], i["y"]);
            display.print(i["text"].get<std::string>().c_str());
            display.setFont();
        }
    }*/

    Serial.print("Pre-Update ");
    Serial.println(micros()); Serial.flush();

    display.display(!kDisplayFullInit);
    kDisplayFullInit = false;

    Serial.print("Updated ");
    Serial.println(micros()); Serial.flush();

    display.hibernate();
    }