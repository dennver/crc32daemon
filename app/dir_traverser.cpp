#include <filesystem>
#include <thread>

#include "dir_traverser.h"

namespace Traverser
{
    std::list<std::string> DirectoryTraversal::Traverse()
    {
        std::list<std::string> filesList;
        namespace fs = std::filesystem;

        for (const auto& entry : fs::recursive_directory_iterator(rootPath))
        {
            if (fs::is_regular_file(entry.path()))
                filesList.push_back(entry.path().string());
        }
        return filesList;
    }
}