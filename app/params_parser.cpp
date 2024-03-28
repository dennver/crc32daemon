#include <string>
#include <cstdlib>
#include <iostream>

#include "params_parser.h"

std::string GetShortOpts(int len, const std::vector<option>& options)
{
    std::string shortOpts;
    for (int i = 0; i < len; ++i)
    {
        shortOpts.push_back(options.at(i).val);
        shortOpts.push_back(':');
    }
    return shortOpts;
}

namespace CommandLine
{
    CommandLineArgsParser::CommandLineArgsParser(const std::unordered_map<std::string, Option>& options) : m_options(options) {}

    void CommandLineArgsParser::Parse(int argc, const char *argv[])
    {
        m_values.clear();
        for (int i = 1; i < argc; i++)
        {
            std::string arg = argv[i];
            const auto it = m_options.find(arg);
            if (it != m_options.end())
            {
                if (it->second.type != ValueType::NOVAL)
                {
                    if (i + 1 < argc && argv[i + 1][0] != '-')
                    {
                        m_values[arg] = argv[i + 1];
                        i++;
                    }
                    else
                    {
                        // Check if default value provided in environment
                        // If no argument provided in argv, take argument from environment
                        // For example: export MY_INT=42; export MY_BOOL=true; export MY_STRING=hello
                         std::cout << "Error: Argument not provided for option -" << arg << std::endl;
                        const char* envVal = std::getenv(arg.c_str());
                         std::cout << "Error: Argument not provided for option -" << std::string(envVal) << std::endl;
                        // if (envVal)
                            m_values[arg] = std::string(envVal);
                        // else
                        // {
                        //     std::cerr << "Error: Argument not provided for option -" << arg << std::endl;
                        //     throw std::invalid_argument("Missing argument for option -" + arg);
                        // }
                        
                    }
                }
            }
            else
                std::cerr << "Error: Unknown option -" << arg << std::endl;
        }
    }
}