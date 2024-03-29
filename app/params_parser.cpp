#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "params_parser.h"

/*CmdlineParser::CmdlineParser(int argc, char *argv[], const CmdParamsList& params)
{
    // const CmdParamsList& params) //: Options(params.size())

    std::vector<option> options;
    options.reserve(params.size());
    std::transform(params.cbegin(), params.cend(), std::back_inserter(options), [](const Option& opt)
    {
        return option{opt.GetKey().c_str(), opt.GetHasArg(), 0, opt.GetShortKey()};
    });
    options.push_back({0, 0, 0, 0});


    std::map<std::string, OptionValue> retVal;

    int opt   = 0;
    int index = 0;

    char str[params.size()*2 - 1];
    for (int i = 0; i < params.size(); ++i)
    {
        str[i++] = options.at(i).val;
        if (i%2 == 0)
            str[i++] = ':';
    }

    while ((opt = getopt_long(Argc, Argv, str, options.data(), &index)) != -1) {
    //         switch(opt) {
    //         // -d, --directory <directory name> dir path
    //         case 'd':
    //         {
    //             std::cout << optarg << std::endl;
    //             dir = optarg;
    //             params.insert({'d', optarg});
    //         }
    //             break;
    //         case 'v':

    //             std::cout << "Version: " << std::endl;
    //             params.insert({'d', optarg});

    //             return 0;
    //         case 0: // long option without a short arg
    //             break;
    //         default:
    //             std::cout << "Incorrect prompt parameters";
    //             return 1;
    //         }
        }
}
*/

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