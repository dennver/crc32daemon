#include <unordered_map>
#include <vector>
#include <getopt.h> //?
// #include <tclap/CmdLine.h> ?

#pragma once

std::string GetShortOpts(int len, const std::vector<option>& options);

namespace CommandLine
{
    class CommandLineArgsParser
    {
    public:
        enum class ValueType {
            NOVAL,
            INT,
            BOOL,
            STRING
        };

        struct Option {
            ValueType type;
            void* value; // Pointer to store the value
        };

        CommandLineArgsParser(const std::unordered_map<std::string, Option>& options);
        ~CommandLineArgsParser(){}
        void Parse(int argc, const char *argv[]);

        template<typename T>
        T GetValue(const std::string& option) const {
            if (values.find(option) != values.end()) {
                return Convert<T>(values.at(option));
            } else {
                // Handle missing argument
                throw std::invalid_argument("Missing argument for option - " + option);
            }
        }
        size_t GetOptionsCount() const { return options.size(); }
        size_t GetValuesCount() const { return values.size(); }

    private:
        std::unordered_map<std::string, Option> options;
        std::unordered_map<std::string, std::string> values;

        template<typename T>
        T Convert(const std::string& str) const {
            // conversion according to ValueType
            if constexpr (std::is_same_v<T, int>)
                return std::stoi(str);
            if constexpr (std::is_same_v<T, bool>) 
                return str == "true";
            if constexpr (std::is_same_v<T, std::string>)
                return str;
            if constexpr (std::is_same_v<T, unsigned int>)
                return std::stoul(str);
            throw std::invalid_argument("Invalid option type - " + str);
        }
    };
}