
/*
 * Database where all the Json is stored
 *
 * The database handles some data being
 * stored in NVS, some in RTC, some in 
 * other storage formats, but all have
 * same accessors.
 * It also handles saving them back
 * Some objects like the interpreter will
 * refer to the Database for data
*/

#pragma once

#include "NvsJson.h"

#include <map>

struct JsonOpt {
    /*std::map<>
    static const Json& mConfig, mFace;
    static Json& mData;*/

    tl::optional<const Json&> mOpt;

    using Section = std::pair<size_t, size_t>;
    using SectionVec = std::vector<Section>;
    
    void resolveJsonPointer() const {
        // Check if the object refers to another to replace it here
        if (!is_string())
            return;
        auto& str = mOpt->get_ref<const std::string&>();
        if (auto s = findSections(str)) {
            if (s || s->size() == 1 ||
                    (s->front() != Section{0, str.size()-1}) ||
                    str[1] != '$')
                return;
            // Check if the string contained is a valid JSON pointer

            return;// {{asd}};
            //
        }
    }

    JsonOpt operator[](const char * str) const {
        if (!mOpt.has_value())
            return {tl::nullopt};
        auto& v = *mOpt;
        auto it = v.find(str);
        if (it == v.end())
            return {tl::nullopt};
        auto res = JsonOpt{{*it}};
        res.resolveJsonPointer();
        return res;
    }

    static tl::optional<SectionVec> findSections(const std::string& str) {
        auto find = [&](char key) {
            size_t last = 0, now;
            std::vector<size_t> matches;
            while ((now = str.find(key, last)) != str.npos) {
                if (now == str.size() || str[now+1] != key) {
                    matches.emplace_back(now);
                }
                last = now + 2;
            }
            return matches;
        };

        // Get all start/end points
        auto st = find('{');
        auto en = find('}');

        if (st.size() != en.size())
            return tl::nullopt;
        
        SectionVec sc;
        for (auto i=0; i<st.size(); i++){
            if (st[i] >= en[i])
                return tl::nullopt;
            sc.emplace_back(st[i], en[i]);
        }
        return sc;
        
    }

    JsonOpt interpret(const std::string& str) {
        findSections(str);
        return {tl::nullopt};
    }

    const bool& bool_or(const bool& def) const {
        if (!mOpt.has_value())
            return def;
        return mOpt->get_ref<const bool&>();
    }
    int int_or(const int& def) const {
        if (!mOpt.has_value())
            return def;
        return mOpt->get<int>();
    }
    template<class T>
    bool operator==(const T& other) const {
        if (!mOpt.has_value())
            return false;
        return *mOpt == other;
    }
    operator bool() const {
        return mOpt.operator bool();
    }
    bool is_string() const {
        return mOpt.map_or(&Json::is_string, false);
    }
};

class Database {
public:
    std::map<std::string, NvsJson> mNvsJson;

    std::map<std::string, JsonBacked&> mDB;
    //std::map<std::string, const JsonBacked&> mDBbc;
public:

    void init();

    void emplace(std::string_view str, NvsJson&& nvs);
    
    JsonOpt get(const std::string& str);


    bool backup();

    //void render(const Json& config, const Json& data,  const Json& render);

    
};

