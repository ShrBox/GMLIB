#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Server {

class PlaceholderAPI {
private:
    std::string                                                                       mValue;
    std::string                                                                       mPlaceholder;
    std::string                                                                       mPluginName;
    int                                                                               mUpdateInterval;
    bool                                                                              mAutoUpdate;
    bool                                                                              mProcessPlayer;
    bool                                                                              mProcessParameters;
    std::function<std::string(Player*)>                                               mCallback;
    std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> mCallbackWithParameters;

public:
    PlaceholderAPI() = default;

    PlaceholderAPI(
        std::string                                                                       Name,
        int                                                                               UpdateInterval,
        bool                                                                              AutoUpdate,
        bool                                                                              mProcessPlayer,
        bool                                                                              mProcessParameters,
        std::string                                                                       PluginName,
        std::function<std::string(Player*)>                                               callback,
        std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> CallbackWithParameters
    );

public:
    virtual ~PlaceholderAPI() = default;

public:
    GMLIB_API static std::string getValue(std::string placeholder, Player* player);

    GMLIB_API static std::string getValue(std::string placeholder);

    GMLIB_API static void translate(std::string& value, Player* sp = nullptr);

    GMLIB_API static std::string translate(std::string_view value, Player* sp = nullptr);

    GMLIB_API static bool
    registerStaticPlaceholder(std::string placeholder, std::string value, std::string PluginName = "");

    GMLIB_API static bool
    registerStaticPlaceholder(std::string placeholder, std::string (*Func)(), std::string PluginName = "");

    GMLIB_API static bool registerStaticPlaceholder(
        std::string                  placeholder,
        std::function<std::string()> callback,
        std::string                  PluginName = ""
    );

    GMLIB_API static bool registerStaticPlaceholder(
        std::string placeholder,
        int         UpdateInterval,
        std::string (*Func)(),
        std::string PluginName
    );

    GMLIB_API static bool registerStaticPlaceholder(
        std::string                  name,
        int                          UpdateInterval,
        std::function<std::string()> callback,
        std::string                  PluginName = ""
    );

    GMLIB_API static bool registerPlayerPlaceholder(
        std::string                         name,
        std::function<std::string(Player*)> callback,
        std::string                         PluginName = ""
    );

    GMLIB_API static bool registerPlayerPlaceholder(
        std::string                                                                       name,
        std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> callback,
        std::string                                                                       PluginName = ""
    );

    GMLIB_API static bool registerServerPlaceholder(
        std::string                  placeholder,
        std::function<std::string()> callback,
        std::string                  PluginName = ""
    );

    GMLIB_API static bool registerServerPlaceholder(
        std::string                                                              name,
        std::function<std::string(std::unordered_map<std::string, std::string>)> callback,
        std::string                                                              PluginName = ""
    );

    GMLIB_API static bool unRegisterPlaceholder(std::string placeholder);

    GMLIB_API static void update(PlaceholderAPI papi);

    GMLIB_API static std::unordered_set<std::string> getPAPIPluginsList();

    GMLIB_API static std::vector<PlaceholderAPI> getPAPIInfoList();

    GMLIB_API static std::vector<std::string> getAllPAPI();

public:
    GMLIB_API inline std::string getName() { return mPlaceholder; }

    GMLIB_API inline std::string getValue() { return mValue; }

    GMLIB_API inline std::string getPluginName() { return mPluginName; }

private:
    static bool registerPlaceholder(PlaceholderAPI papi);
};

} // namespace GMLIB::Server