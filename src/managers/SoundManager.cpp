#include "SoundManager.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace ray {
SoundManager::SoundManager(const string& path) : _global_volume(0.0)
{
    this->setReleaseStrategy(thor::Resources::ExplicitRelease);
    this->loadConfigFile(path);
}

SoundManager::~SoundManager()
{
}

bool SoundManager::loadConfigFile(const string& path) {
    boost::property_tree::ptree config;
    boost::property_tree::read_json(path, config); // TODO: Handle exception


    for (const auto& i : config.get_child("files")) {
        // i.first is the key, i.second is the path
        this->_sounds[i.first] =
            Sound(
                *(this->acquire(thor::Resources::fromFile<SoundBuffer>(i.second.data())))
            );
    }

    return true;
}

Sound& SoundManager::getSound(const string& id) {
    return this->_sounds[id];
}

void SoundManager::setGlobalVolume(const float globalVolume) {
    this->_global_volume = globalVolume;
    for (auto& i : this->_sounds) {
        i.second.setVolume(globalVolume);
    }
}

float SoundManager::getGlobalVolume() const {
    return this->_global_volume;
}

}
