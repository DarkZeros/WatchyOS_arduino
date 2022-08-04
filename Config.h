
#pragma once 

#include <array>
#include <string_view>


template <std::size_t N>
constexpr std::array<char, N> without(const char (&s)[N])
{
    std::array<char, N> res{};
    std::size_t i = 0;

    for (auto c : s) {
        if (c != ' ' && c != '\n' && c != '\t') {
            res[i++] = c;
        }
    }
    return res;
}

// Config used by WatchyOS execution, can be altered by user
constexpr std::string_view kDefaultConfig = R""""({
    "time":{
        "ntpServer": "http://worldtimeapi.org/api/ip",
        "ntpMinutes": 1440,
        "timezone": {
            "_opt": ["Auto", ""],
            "_sel": 0
        }
    },
    "display":{
        "_name": "Display Settings",
        "inverted": false
    },
    "buttons": {
        "a":1
    }
})"""";
/*constexpr std::array<uint8_t> kDefaultConfigC = []() {
    return {};
};*/

constexpr std::string_view kDefaultData = R""""({
    "time":{
        "epoch": 26439104385,
        "m": 45,
        "h": 14,
        "s": 12
    }
})"""";

constexpr std::string_view kDefaultWatchFace = R""""({
    "__fonts__":{
        "A": 3
    },
    "elena" : {
        "type": "text",
        "text": "{Hola Elenas :)}",
        "x": 10,
        "y": 10
    },
    "circ2" : {
        "type": "circ",
        "r": 50
    },
    "circ" : {
        "type": "circ",
        "fill": true,
        "r": 10,
        "sub": ["asd", 100, false]
    },
    "time" : {
        "type": "text",
        "text": "hello, {data.time.m}",
        "x": 20
    },
    "r" : {
        "type": "rect",
        "w": 10,
        "h": 20
    },
    "asd" : {
        "type": "bitmap",
        "data": [23, 67, 12],
        "w": 10,
        "h": 20
    }
})"""";