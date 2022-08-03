
/* This class hold Json data in SPIFFS
 * 
 * Takes care of automatic saving on destruction
 * or on backup() call
*/

#pragma once 

#include "JsonBacked.h"

#include <SPIFFS.h>

class SpiffsJson : public JsonBacked {
    mutable fs::File mFile;

public:
    static tl::optional<SpiffsJson> create(const std::string& path, bool ignoreChanges, const Json& def = {})
    {
        if (!SPIFFS.begin(true))
            return {};
        auto f = SPIFFS.open(path.c_str(), ignoreChanges ? FILE_READ : FILE_WRITE, true);
        if (!f)
            return {};
        
        auto size = f.size();
        std::vector<uint8_t> buffer(size);
        f.read(buffer.data(), size);

        return SpiffsJson{std::move(f), buffer, ignoreChanges, def};
    }

    explicit SpiffsJson(fs::File&& file, const std::vector<uint8_t>& buffer, bool ignoreChanges, const Json& def = {})
    : JsonBacked(buffer, ignoreChanges, def)
    , mFile(std::move(file))
    {  
    }

    explicit SpiffsJson(fs::File&& file, Json&& json, bool needSave, bool ignoreChanges, tl::optional<uint32_t> hash)
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