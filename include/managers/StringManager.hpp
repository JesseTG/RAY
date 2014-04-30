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
        bool loadConfigFile(const string& path) override {return false;}

        const string& getScript(const string& key);
};
}

#endif // STRINGMANAGER_HPP
