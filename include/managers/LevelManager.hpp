#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <vector>
#include <string>

namespace ray {
using std::string;
using std::vector;
class LevelManager
{
    public:
        LevelManager(const string& path);
        virtual ~LevelManager();
        const string& getLevelName(const int) const;
        int getLevelNumber(const string&) const;
    protected:
    private:
        vector<string> _levels;

        void _load_config_file(const string& path);
};
}
#endif // LEVELMANAGER_HPP
