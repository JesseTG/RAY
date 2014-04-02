#include "ImageManager.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace ray {
ImageManager::ImageManager()
{
    //ctor
}

ImageManager::~ImageManager()
{
    //dtor
}

bool ImageManager::loadConfigFile(const string& path) {
    boost::property_tree::ptree config;
    boost::property_tree::read_json(path, config); // TODO: Handle exception

    for (const auto& i : config.get_child("files")) {
        // i.first is the key, i.second is the path
        this->acquire(thor::Resources::fromFile<Image>(i.second.data()));
    }

    return true;
}
}
