#include "ResourceManager.hpp"

namespace ray {
ResourceManager::ResourceManager() {
    //ctor
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::reloadAll() {
    return this->loadConfigFile(this->config_file);
}

}
