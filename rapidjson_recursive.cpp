// Recursively map all the unique key names to lists of JSON paths 
// where those key names appear.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

void getKeyNamesMappedToPaths(
    std::map<std::string, std::vector<std::string>> *keyNamesMappedToPaths,
    const rapidjson::Value& val,
    const std::string keyPath = "")
{
    if (val.IsObject())
    {
        for (rapidjson::Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
        {
            std::string key = itr->name.GetString();
            std::string innerKeyPath = keyPath + "/" + key;
            auto keySearch = keyNamesMappedToPaths->find(key);
            if (keySearch == keyNamesMappedToPaths->end())
            {
                std::vector<std::string> vec;
                vec.push_back(innerKeyPath);
                keyNamesMappedToPaths->emplace(key, vec);
            }
            else
            {
                keyNamesMappedToPaths->at(key).push_back(innerKeyPath);
            }
            if (itr->value.IsObject())
            {
                const rapidjson::Value& val = itr->value.GetObject();
                getKeyNamesMappedToPaths(keyNamesMappedToPaths, val, innerKeyPath);
            }
        }
    }
}

int main(int, char**)
{
    std::ifstream ifs("rapidjson_recursive.json");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    std::map<std::string, std::vector<std::string>> keyNamesMappedToPaths;
    getKeyNamesMappedToPaths(&keyNamesMappedToPaths, doc.GetObject());
    for (std::map<std::string, std::vector<std::string>>::iterator itr = keyNamesMappedToPaths.begin(); itr != keyNamesMappedToPaths.end(); ++itr)
    {
        std::cout << itr->first << ": [" << std::endl;
        for (auto innerItr = itr->second.begin(); innerItr != itr->second.end(); ++innerItr)
        {
            std::cout << "    \"" << *innerItr << "\"," << std::endl;
        }
        std::cout << "]," << std::endl;
    }
    return 0;
}
