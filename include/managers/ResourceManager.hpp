#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

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
 *
 * It is implementation-defined whether or not any file objects are kept open.
 */
class ResourceManager
{
    public:
        ResourceManager();

        /**
         * Destructor. Frees any operating system resources and closes any open
         * input streams.
         */
        virtual ~ResourceManager();

        /**
         * Loads and validates a resource from the given path. By default, just
         * calls @c this->load(std::ifstream(path)).
         *
         * @param path The path on the file system to load the requested
         * resource from
         * @return True if the requested resource exists, was loaded properly,
         * and passed validation.
         */
        virtual bool load(const char* path);

        /**
         * @copydoc ResourceManager::load(const char*)
         */
        virtual bool load(const string& path);

        /**
         * Loads and validates a resource from the given input stream. This
         * should be implemented in subclasses.
         */
        virtual bool load(istream& file) = 0;

        /**
         * Loads and validates a set of resources from a given configuration
         * file. The configuration file should list all the resources to be
         * loaded in some implementation-defined format. By default, calls
         * @c this->loadFromConfigFile(std::ifstream(path)).
         *
         * @param path The path on the file system of the config file to load
         * the desired resources from
         * @return True if the configuration file was found and parsed
         * successfully and if all of the resources loaded and validated
         * properly.
         */
        virtual bool loadFromConfigFile(const char* path);

        /**
         * @copydoc ResourceManager::loadFromConfigFile(const char*)
         */
        virtual bool loadFromConfigFile(const string& path);

        /**
         * Loads and validates a set of resources from the given @c std::istream.
         */
        virtual bool loadFromConfigFile(const istream& file) = 0;

        /**
         * Reloads all referenced resources. No configuration file is needed;
         * just reload the resources from the stored file paths.
         *
         * @return True if all files were reloaded and revalidated properly.
         */
        virtual bool reloadAll() = 0;
};
}

#endif // RESOURCEMANAGER_HPP
