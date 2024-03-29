#include <list>
#include <string>

#pragma once

namespace Traverser
{
    class DirectoryTraversal
    {
    public:
        DirectoryTraversal(const std::string& path) : rootPath(path) {}
        std::list<std::string> Traverse();
        
    private:
        std::string rootPath;
    };
} // namespace name