#include "MusicManager.hpp"

namespace ray {
MusicManager::MusicManager(const string& path) : _global_volume(0.0)
{
    this->loadConfigFile(path);
}

MusicManager::~MusicManager()
{
    //dtor
}

bool MusicManager::loadConfigFile(const string& path) {
    boost::property_tree::ptree config;
    boost::property_tree::read_json(path, config); // TODO: Handle exception

    for (const auto& i : config.get_child("files")) {
        // i.first is the key, i.second is the path
        this->_music[i.first] = i.second.data();
    }

    return true;
}

void MusicManager::setGlobalVolume(const float globalVolume) {
    this->_global_volume = globalVolume;
}

float MusicManager::getGlobalVolume() const {
    return this->_global_volume;
}


}
