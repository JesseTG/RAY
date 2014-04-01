#ifndef SCRIPTMANAGER_HPP
#define SCRIPTMANAGER_HPP

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include <LuaContext.hpp>

#include "ResourceManager.hpp"

namespace ray {
using std::ifstream;
using std::ostringstream;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::weak_ptr;
class ScriptManager : public ResourceManager
{
    public:
        ScriptManager(const shared_ptr<LuaContext> lua);
        virtual ~ScriptManager();

        bool loadConfigFile(const string& path) override;

        const string& getScript(const string& key);
    private:
        unordered_map<string, string> _scripts;
        weak_ptr<LuaContext> _lua;

        bool _load_file(shared_ptr<LuaContext> lua, const string& path);
};
}

#endif // SCRIPTMANAGER_HPP
