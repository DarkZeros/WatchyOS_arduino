
/* This class hold backed JSON interface
 * 
 * It cannot be instaltiated by itself
 * and is the base class of NVS, SPIFFS LittleFS
*/

#pragma once

#include <string>
#include <vector>

#include "json/single_include/nlohmann/json.hpp"
using Json = nlohmann::json;
#include "optional.hpp"
#include "xxhash32.h"

class JsonBacked {
protected:
    Json mJson;
    bool mIgnoreChanges = false;
    mutable bool mNeedSave = false;
    mutable tl::optional<size_t> mPrevHash;

protected:
    static uint32_t getHash(const std::vector<uint8_t>& blob) {
        XXHash32 hash32(0);
        hash32.add(blob.data(), blob.size());
        return hash32.hash();
    }
public:
    explicit JsonBacked(const std::vector<uint8_t>& buffer, bool ignoreChanges, std::string_view def = {})
    : mJson(Json::from_msgpack(buffer, true, false))
    , mIgnoreChanges{ignoreChanges}
    {
        if (mJson.is_discarded()) {
            mJson = Json::parse(def, nullptr, false);
            mNeedSave = true;
        }
        // Compute hash if we are happy with previous content
        if (!mIgnoreChanges && !mNeedSave)
            mPrevHash = getHash(buffer);
    }
    explicit JsonBacked(Json&& json, bool needSave, bool ignoreChanges, tl::optional<size_t> hash)
    : mJson(std::move(json))
    , mIgnoreChanges{ignoreChanges}
    , mNeedSave{needSave}
    , mPrevHash{hash}
    {
    }

    // Interface that will be called before deep sleep
    bool backup() const {
        if (!mNeedSave)
            return true;
        if (mIgnoreChanges)
            return true;
        // Convert JSON to vector for saving
        std::vector<uint8_t> blob = Json::to_msgpack(mJson);
        // Compute hash
        auto hash = getHash(blob);
        if (mPrevHash && *mPrevHash == hash)
            return true;
        if (!save(blob))
            return false;
        mPrevHash = hash;
        mNeedSave = false;
        return true;
    }

    // Subclasses will implement the saving code
    virtual ~JsonBacked() {}
    virtual bool save(const std::vector<uint8_t>&) const = 0;
 
    const Json& get() const { return mJson; };
    Json& get() { mNeedSave = true; return mJson; };

    operator const Json&() const { return mJson; };
    operator Json&() { mNeedSave = true; return mJson; };
};