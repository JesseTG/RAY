#ifndef LEVELINFO_HPP
#define LEVELINFO_HPP

#include <string>

namespace ray {
using std::string;
struct LevelInfo : public util::GameStateArguments
{
    LevelInfo(const string& name="",
              const int lives = 0,
              const int quota = 0) :
        GameStateArguments(),
        name(name),
        lives(lives),
        quota(quota) {}
    ~LevelInfo() {}
    string name;
    int lives;
    int quota;
};
}

#endif // LEVELINFO_HPP
