#include "LevelManager.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

namespace ray {
using std::find;
using boost::property_tree::ptree;

LevelManager::LevelManager(const string& path)
{
    this->_load_config_file(path);
}

LevelManager::~LevelManager()
{
    //dtor
}

int LevelManager::getLevelNumber(const string& name) const {
    for (int i = 0; i < this->_levels.size(); ++i) {
        if (this->_levels[i] == name) {
            return i;
        }
    }
    return -1;
}

const string& LevelManager::getLevelName(const int index) const {
    #ifdef DEBUG
    return this->_levels.at(index);
    #else
    return this->_levels[index];
    #endif // DEBUG
}

void LevelManager::_load_config_file(const string& path) {
    ptree config;
    boost::property_tree::read_json(path, config); // TODO: Handle exception

    for (const auto& i : config.get_child("levels")) {
        this->_levels.push_back(i.second.get_child("id").data());
    }
}


}
