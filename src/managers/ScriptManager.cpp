#include "ScriptManager.hpp"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "config.hpp"

namespace ray {
ScriptManager::ScriptManager(const shared_ptr<LuaContext> lua) :
    ResourceManager(),
    _lua(lua)
{

}

ScriptManager::~ScriptManager()
{
    //dtor
}

bool ScriptManager::loadConfigFile(const string& path) {
    shared_ptr<LuaContext> lua = this->_lua.lock();

    if (!lua) {
        return false;
    }

    this->_scripts.clear();
    boost::property_tree::ptree config;
    boost::property_tree::read_json(path, config); // TODO: Handle exception


    for (const auto& i : config.get_child("files")) {
        // i.first is the key, i.second is the path
        this->_load_file(lua, i.second.data());
    }

    return true;
}

const string& ScriptManager::getScript(const string& key) {
    return this->_scripts[key];
}

bool ScriptManager::_load_file(shared_ptr<LuaContext> lua, const string& path) {
    ifstream script(path);
    if (!script) {
        return false;
    }
    else {
        ostringstream oss;
        oss << script.rdbuf();
        this->_scripts[path] = oss.str();
        lua->executeCode(this->_scripts[path]);
        return true;
    }
}

}
