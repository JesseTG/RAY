#ifndef LEVELINFO_HPP
#define LEVELINFO_HPP

#include <string>

namespace ray {
using std::string;
struct LevelInfo : public util::GameStateArguments
{
    LevelInfo(const string& name="", const int lives=0) : GameStateArguments(), name(name), lives(lives) {}
    ~LevelInfo() {}
    string name;
    int lives;
};
}

#endif // LEVELINFO_HPP
