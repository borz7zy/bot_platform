#ifndef MAIN_HXX
#define MAIN_HXX

#include <unordered_map>
#include <string>

namespace config_controller
{
    class ConfigManager
    {
    public:
        enum class AccessType
        {
            RO, // read-only
            RW  // read-write
        };

        static ConfigManager &getInstance();

        template <typename T>
        void setConfig(const std::string &key, const T &value, AccessType access);

        std::string getConfig(const std::string &key) const;

        ConfigManager(const ConfigManager &) = delete;
        ConfigManager &operator=(const ConfigManager &) = delete;

    private:
        ConfigManager() = default;

        struct ConfigEntry
        {
            std::string value;
            AccessType access;
        };

        std::unordered_map<std::string, ConfigEntry> configData;

        template <typename T>
        std::string toString(const T &value) const;
    };
};

#endif