#ifndef CONFIGCENTER_H
#define CONFIGCENTER_H

#include "Singleton.hpp"
#include "rapidjson/document.h"

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <string>
#include <map>
#include <vector>


namespace nd
{
    typedef std::map<std::string, rapidjson::Value*> ParamMap;
    typedef std::vector<std::string> StrVector;
    typedef std::vector<int> IntVector;
    class ConfigCenter
    {
    public:
        ConfigCenter();
        ~ConfigCenter();

        void clear();
        void set(rapidjson::Document* doc, ParamMap& map);
        int get(const std::string& theKey, const int theDefault);
        const std::string get(const std::string& theKey, const std::string& theDefault);
        StrVector getStrVector(const std::string& theKey);
        IntVector getIntVector(const std::string& theKey);

        int load(const std::string& thePath);

    private:
        rapidjson::Document* documentM;
        std::shared_mutex mapMutexM;
        ParamMap paramMapM;
    };
    typedef DesignPattern::Singleton<ConfigCenter, 0> ConfigCenterSingleton;
}

#define g_cfg nd::ConfigCenterSingleton::instance()

#endif /* CONFIGCENTER_H */

