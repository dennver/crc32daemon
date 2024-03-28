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
            if (m_values.find(option) != m_values.end()) {
                return Convert<T>(m_values.at(option));
            } else {
                // Handle missing argument
                throw std::invalid_argument("Missing argument for option - " + option);
            }
        }
        size_t GetOptionsCount() const { return m_options.size(); }
        size_t GetValuesCount() const { return m_values.size(); }

    private:
        std::unordered_map<std::string, Option> m_options;
        std::unordered_map<std::string, std::string> m_values;

        template<typename T>
        T Convert(const std::string& str) const {
            // conversion according to ValueType
            if constexpr (std::is_same_v<T, int>)
                return std::stoi(str);
            if constexpr (std::is_same_v<T, bool>) 
                return str == "true";
            if constexpr (std::is_same_v<T, std::string>)
                return str;
            throw std::invalid_argument("Invalid option type - " + str);
        }
    };
}