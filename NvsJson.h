
/* This class hold Json data in SPIFFS
 * 
 * Takes care of automatic saving on destruction
 * or on backup() call
*/

#pragma once

#include "JsonBacked.h"
#include "ArduinoNvs.h"

class NvsJson : public JsonBacked {
    std::string mPath;

public:
    static tl::optional<NvsJson> create(const std::string& path, bool ignoreChanges, std::string_view def = {})
    {
        NVS.begin("watchy");
        std::vector<uint8_t> buffer = NVS.getBlob(path);

        return NvsJson(path, buffer, ignoreChanges, def);
    }

    explicit NvsJson(const std::string& path, const std::vector<uint8_t>& buffer, bool ignoreChanges, std::string_view def = {})
    : JsonBacked(buffer, ignoreChanges, def)
    , mPath(std::move(path))
    {  
    }
    explicit NvsJson(const std::string& path, Json&& json, bool needSave, bool ignoreChanges, tl::optional<uint32_t> hash)
    : JsonBacked(std::move(json), needSave, ignoreChanges, hash)
    , mPath(std::move(path))
    {  
    }

    bool save(const std::vector<uint8_t>& buffer) const override {
        return NVS.setBlob(mPath, buffer, true);
    };
};
