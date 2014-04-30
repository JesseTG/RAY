#ifndef MUSICMANAGER_HPP
#define MUSICMANAGER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Resources.hpp>

#include "ResourceManager.hpp"

namespace ray {
using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::make_shared;
using std::move;
using sf::Music;
class MusicManager : public ResourceManager
{
    public:
        MusicManager(const string& path);
        virtual ~MusicManager();
        bool loadConfigFile(const string& path) override;

        void setGlobalVolume(const float globalVolume);
        float getGlobalVolume() const;

        void setSong(const string& id);
        Music& getCurrentSong();
    private:
        unordered_map<string, string> _paths;
        Music _music;
        float _global_volume;
};
}

#endif // MUSICMANAGER_HPP
