#include "ResourceManager.hpp"

namespace ray {
ResourceManager::ResourceManager()
{
    //ctor
}

bool ResourceManager::load(const string& path) {
    ifstream in(path);
    return this->load(in);
}

bool ResourceManager::load(const char* path) {
    ifstream in(path);
    return this->load(in);
}

bool ResourceManager::loadFromConfigFile(const string& path) {
    return this->loadFromConfigFile(ifstream(path));
}

bool ResourceManager::loadFromConfigFile(const char* path) {
    return this->loadFromConfigFile(ifstream(path));
}

}
