#include <algorithm>
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
    CommandLineArgsParser::CommandLineArgsParser(const std::unordered_map<std::string, Option>& options) : options(options) {}

    void CommandLineArgsParser::Parse(int argc, const char *argv[])
    {
        values.clear();
        std::vector<std::string> argVec(argv+1, argv + argc);

        for (auto it = options.cbegin(); it != options.cend(); ++it)
        {
            std::string option = it->first;
            if (auto argIt = std::find(argVec.begin(), argVec.end(), "-" + option); argIt != argVec.end())
            {
                auto nx = std::next(argIt, 1);
                if (it->second.type != CommandLine::CommandLineArgsParser::ValueType::NOVAL)
                    if (nx != argVec.end() && nx->at(0) != '-')
                        values[option] = *nx;
                    else
                       throw std::invalid_argument("Missing argument for option -" + option);
            }
            else
            {
                const char* envVal = std::getenv(option.c_str());
                if (envVal)
                    values[option] = std::string(envVal);
                else
                    throw std::invalid_argument("Missing argument for option -" + option);
            }
        }
    }
}