#ifndef LEVELINFO_HPP
#define LEVELINFO_HPP

#include <string>

namespace ray {
using std::string;
struct LevelInfo : public util::GameStateArguments
{
    ~LevelInfo() {}
    string name;
};
}

#endif // LEVELINFO_HPP
