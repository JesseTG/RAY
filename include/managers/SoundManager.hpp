#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <unordered_map>

#include <SFML/Audio.hpp>
#include <Thor/Resources.hpp>
#include "ResourceManager.hpp"

namespace ray {
using thor::ResourceCache;
using sf::Sound;
using sf::SoundBuffer;
class SoundManager : public ResourceManager, public ResourceCache<SoundBuffer>
{
    public:
        SoundManager(const string& path);
        virtual ~SoundManager();
        bool loadConfigFile(const string& path) override;
        Sound& getSound(const string& id);

        void setGlobalVolume(const float globalVolume);
        float getGlobalVolume() const;
    private:
        float _global_volume;
        unordered_map<string, Sound> _sounds;
};
}

#endif // SOUNDMANAGER_HPP
