#include "Global.h"
#include <GMLIB/Files/JsonLanguage.h>
#include <GMLIB/Files/ResourceLanguage.h>
#include <GMLIB/Mod/CustomPacks.h>

namespace GMLIB::Files {

std::vector<std::string> readLinesFromFile(std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream            file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }
    return {};
}

std::string getKeyFromLine(std::string& line) {
    size_t equalPos = line.find("=");
    auto   res      = line.substr(0, equalPos);
    return res;
}

bool hasKey(std::string& key, std::vector<std::string> lines) {
    for (auto& line : lines) {
        if (getKeyFromLine(line) == key) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> splitStringByNewline(const std::string input) {
    std::vector<std::string> lines;
    size_t                   startPos = 0;
    while (startPos < input.size()) {
        size_t endPos = input.find_first_of("\n\r", startPos);
        if (endPos == std::string::npos) {
            lines.push_back(input.substr(startPos));
            break;
        }
        lines.push_back(input.substr(startPos, endPos - startPos));
        startPos = endPos + 1;
    }
    return lines;
}

std::string defaultManifest = R"({
    "format_version": 2,
    "header": {
        "description": "Plugin language pack, this pack is automatically generated by GMLIB.",
        "platform_locked" : false,
        "min_engine_version": [1, 20, 50]
    },
    "modules": [
        {
            "description": "Plugin language pack, this pack is automatically generated by GMLIB.",
            "type": "resources"
        }
    ]
})";

ResourceLanguage::ResourceLanguage(
    std::string directoryPath,
    std::string pluginName,
    ushort      versionMajor,
    ushort      versionMinor,
    ushort      versionPatch
)
: mPath(directoryPath),
  mPluginName(pluginName) {
    mLanguages = {};
    mVersion   = SemVersion(versionMajor, versionMinor, versionPatch, "", "");
}

bool buildManisest(SemVersion& version, std::string path, std::string name) {
    if (std::filesystem::exists(path)) {
        std::ifstream inputFile(path);
        std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        auto          oldJson    = nlohmann::ordered_json::parse(fileContent, nullptr, true, true);
        auto          oldVersion = oldJson["header"]["version"].get<std::vector<int>>();
        if (oldVersion[0] == version.getMajor() && oldVersion[1] == version.getMinor()
            && oldVersion[2] == version.getPatch()) {
            return false;
        }
    }
    std::vector<int> ver          = {version.getMajor(), version.getMinor(), version.getPatch()};
    auto             json         = nlohmann::json::parse(defaultManifest, nullptr, true, true);
    json["header"]["uuid"]        = mce::UUID::random();
    json["header"]["version"]     = ver;
    json["header"]["name"]        = name + " Language Pack";
    json["modules"][0]["uuid"]    = mce::UUID::random();
    json["modules"][0]["version"] = ver;
    std::ofstream manifestFile(path);
    manifestFile << json.dump(4);
    manifestFile.close();
    return true;
}

void ResourceLanguage::initLanguage() {
    Mod::CustomPacks::addCustomPackPath(mPath);
    auto path = mPath;
    if (!path.ends_with("/")) {
        path = path + "/";
    }
    std::filesystem::create_directories(path + "language_pack");
    std::string pathLangDir  = "language_pack/texts/";
    std::string pathManifest = "language_pack/manifest.json";
    buildManisest(mVersion, path + pathManifest, mPluginName);
    std::filesystem::create_directories(path + pathLangDir);
    auto langJson = nlohmann::json::array();
    for (auto& lang : mLanguages) {
        langJson.push_back(lang.first);
        auto          langPath = path + pathLangDir + lang.first + ".lang";
        std::ifstream oldFile(langPath);
        std::string   langFileData((std::istreambuf_iterator<char>(oldFile)), std::istreambuf_iterator<char>());
        while (langFileData.ends_with("\n") || langFileData.ends_with("\r")) {
            langFileData.erase(langFileData.size() - 1);
        }
        while (langFileData.starts_with("\n") || langFileData.starts_with("\r")) {
            langFileData.erase(0, 1);
        }
        if (std::filesystem::exists(langPath)) {
            std::vector<std::string> oldLangData = readLinesFromFile(langPath);
            std::vector<std::string> newLangData = splitStringByNewline(lang.second);
            for (auto& newLang : newLangData) {
                if (newLang.empty()) {
                    continue;
                }
                auto newKey = getKeyFromLine(newLang);
                if (!hasKey(newKey, oldLangData)) {
                    langFileData = langFileData + "\n" + newLang;
                }
            }
        }
        std::ofstream newLang(langPath);
        newLang << langFileData;
        newLang.close();
    }
    if (std::filesystem::exists(path + pathLangDir + "languages.json")) {
        auto oldData = GMLIB::Files::JsonLanguage::readFromFile(path + pathLangDir + "languages.json");
        langJson.merge_patch(oldData);
    }
    std::ofstream newLangJson(path + pathLangDir + "languages.json");
    newLangJson << langJson.dump(4);
    newLangJson.close();
}

void ResourceLanguage::addLanguage(std::string identifider, std::string language) {
    mLanguages[identifider] = language;
}

void ResourceLanguage::addLanguages(std::vector<std::pair<std::string, std::string>> languages) {
    for (auto& data : languages) {
        mLanguages[data.first] = data.second;
    }
}

} // namespace GMLIB::Files