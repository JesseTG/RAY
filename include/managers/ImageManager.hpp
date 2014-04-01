#ifndef IMAGEMANAGER_HPP
#define IMAGEMANAGER_HPP

#include "ResourceManager.hpp"

#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>

namespace ray {
using sf::Image;
using thor::ResourceCache;
using thor::ResourceKey;

class ImageManager : public ResourceManager, public ResourceCache<Image>
{
    public:
        ImageManager();
        virtual ~ImageManager();
        bool loadConfigFile(const string& path) override;
    protected:
    private:
};
}

#endif // IMAGEMANAGER_HPP
