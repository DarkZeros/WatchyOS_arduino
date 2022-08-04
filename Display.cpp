
#include "Display.h"
#include "NvsJson.h"

#define CS 5
#define DC 10
#define RESET 9
#define BUSY 19

RTC_DATA_ATTR bool kDisplayFullInit = true;

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> Display::display(GxEPD2_154_D67(CS, DC, RESET, BUSY));

static void busyCallBack(const void *) {
    gpio_wakeup_enable((gpio_num_t)BUSY, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
}

void Display::init() {
    //std::unique_lock<std::mutex> lock(mMutex);
    if (!mInit) {
        display.init(/*115200*/0, kDisplayFullInit, 10, true);
        display.epd2.setBusyCallback(busyCallBack);
        mInit = true;
    }
}

static Json asd = "muajajajaa";
//static JsonBacked asd2 = {{"muajajajaa2"}};

void Display::render() {
    //std::unique_lock<std::mutex> lock(mMutex);

    // get from the DB the nodes we need
    auto config = mDB.get("config");
    auto data = mDB.get("data");
    auto render = mDB.get("watchface");

    // Select if we are in normal, or inverted mode
    const auto inverted = config["display"]["inverted"].bool_or(false);
    const auto foreColor = !inverted ? GxEPD_BLACK : GxEPD_WHITE;
    const auto backColor = !inverted ? GxEPD_WHITE : GxEPD_BLACK;

    // Clear previous content
    display.epd2.darkBorder = inverted;
    init();
    display.fillScreen(backColor);

    // Check if we have fonts


    // For each element recursively traverse
    enum class ListType {
        NONE,
        V,
        H
    };

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

            // get sizes, and common params
            auto x = v["x"].int_or(0);
            auto y = v["y"].int_or(0);
            auto w = v["w"].int_or(1);
            auto h = v["h"].int_or(1);
            auto size = v["size"].int_or(-1);
            auto fill = v["fill"].bool_or(false);
            auto cInt = v["color"].int_or(-1);
            auto color = cInt == -1 ? foreColor : cInt == 0 ? GxEPD_BLACK : GxEPD_WHITE;

            // For each type specific
            if (type == "circ") {
                auto rad = v["r"].int_or(std::min(x, y));
                auto q = v["q"].int_or(0xF);
                auto d = v["d"].int_or(0);
                if (size < 0) {
                    if (list == ListType::V)
                        size = y + rad;
                    if (list == ListType::H)
                        size = x + rad;
                }

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
            } else if (type == "rect") {
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
            } else if (type == "bitmap") {
                auto data = v["data"];
                auto jsd = Json::binary({0xCA, 0xFE, 0xBA, 0xBE}, 23);
                //if (data && data.mOpt->is_binary())
                //    display.drawBitmap(r.l + x, r.t + y, data.mOpt->get_binary().data(), w, h, color);
                if (data && jsd.is_binary())
                    display.drawBitmap(r.l + x, r.t + y, jsd.get_binary().data(), w, h, color);
                
            }  else if (type == "text") {
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

    display.display(!kDisplayFullInit);
    kDisplayFullInit = false;


    display.hibernate();
    }