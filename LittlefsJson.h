
/* This class hold Json data in SPIFFS
 * 
 * Takes care of automatic saving on destruction
 * or on backup() call
*/

#pragma once

#include "JsonBacked.h"

#include <LittleFS.h>

class LittlefsJson : public JsonBacked {
    mutable fs::File mFile;

public:
    static tl::optional<LittlefsJson> create(const std::string& path, bool ignoreChanges, std::string_view = {})
    {
        if (!LittleFS.begin(true))
            return {};
        auto f = LittleFS.open(path.c_str(), ignoreChanges ? FILE_READ : FILE_WRITE, true);
        if (!f)
            return {};
        
        auto size = f.size();
        std::vector<uint8_t> buffer(size);
        f.read(buffer.data(), size);

        return LittlefsJson{std::move(f), buffer, ignoreChanges, def};
    }

    explicit LittlefsJson(fs::File&& file, const std::vector<uint8_t>& buffer, bool ignoreChanges, std::string_view = {})
    : JsonBacked(buffer, ignoreChanges, def)
    , mFile(std::move(file))
    {  
    }

    explicit LittlefsJson(fs::File&& file, Json&& json, bool needSave, bool ignoreChanges, tl::optional<uint32_t> hash)
    : JsonBacked(std::move(json), needSave, ignoreChanges, hash)
    , mFile(std::move(file))
    {  
    }

    bool save(const std::vector<uint8_t>& buffer) const override {
        if (!mFile.seek(0, SeekSet))
            return false;
        if (mFile.write(buffer.data(), buffer.size()) != buffer.size())
            return false;
        return true;
    };
};