
#include "Database.h"

void Database::init() {

}

void Database::emplace(std::string_view str, NvsJson&& nvs) {
    mNvsJson.emplace(str, std::move(nvs));
}

JsonOpt Database::get(const std::string& str) {
    auto it = mNvsJson.find(str);
    if (it != mNvsJson.end())
        return {it->second.get()};
    return {};
}

bool Database::backup() {
    for(auto& [_, json] : mDB) {
        json.backup();
    }
    return true;
}

