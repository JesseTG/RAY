#ifndef RESOURCELOADER_HPP
#define RESOURCELOADER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

namespace ray {
using std::istream;
using std::ifstream;
using std::string;
using std::unordered_map;
using std::wstring;

/**
 * An abstract class for loading and storing external resources (graphics,
 * sounds, scripts, etc.) of various kinds.
 */
class ResourceManager
{
    public:
        /**
         * Constructor. You may
         */
        ResourceManager();

        /**
         * Destructor. Frees any operating system resources and closes any open
         * input streams.
         */
        virtual ~ResourceManager();

        /**
         * Loads and validates a set of resources from a given configuration
         * file. The configuration file should list all the resources to be
         * loaded in some implementation-defined format.
         *
         * @param path The path on the file system of the config file to load
         * the desired resources from
         * @return True if the configuration file was found and parsed
         * successfully and if all of the resources loaded and validated
         * properly.
         */
        virtual bool loadConfigFile(const string& path) = 0;

        /**
         * Reloads all referenced resources. No configuration file is needed;
         * just reload the resources from the stored file paths.
         *
         * @return True if all files were reloaded and revalidated properly.
         */
        bool reloadAll();
    protected:
        string config_file;

};
}

#endif // RESOURCELOADER_HPP
