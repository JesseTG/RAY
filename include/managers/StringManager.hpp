#ifndef STRINGMANAGER_HPP
#define STRINGMANAGER_HPP

#include "ResourceManager.hpp"

namespace ray {
class StringManager : public ResourceManager
{
    public:
        StringManager();
        virtual ~StringManager();
    private:
        bool load(const string& path) override;
        bool loadFromConfigFile(const string& path) override;
        bool reloadAll() override;

        const string& getScript(const string& key);
};
}

#endif // STRINGMANAGER_HPP
