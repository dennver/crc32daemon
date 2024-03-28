#include <getopt.h>
#include <gtest/gtest.h>

#include "../app/params_parser.h"

TEST(CmdLineParser, shortOpts)
{
    std::vector<option> options = {
        {"timeout", 0, 0, 't'},
        {"directory", 0, 0, 'd'},
        {"version",   0, 0, 'v'},
        {0, 0, 0, 0}
    };
    std::string str = GetShortOpts(3, options);
    EXPECT_STREQ(str.data(), "t:d:v:");
}

TEST(CmdLineParser, Parse)
{
    using namespace CommandLine;
    CommandLineArgsParser::Option opt1{CommandLineArgsParser::ValueType::INT};//, "10"};
    CommandLineArgsParser::Option opt2{CommandLineArgsParser::ValueType::BOOL};//, "false"};
    CommandLineArgsParser::Option opt3{CommandLineArgsParser::ValueType::STRING};//, "default"};
    std::unordered_map<std::string, CommandLine::CommandLineArgsParser::Option> options{
        {"--int", opt1},
        {"--bool", opt2},
        {"--string", opt3}
    };

    const char* argv[]={"app", "--int", "5", "--bool", "true", "--string", "default"};
    const int argc = sizeof(argv)/sizeof(char*);;
    CommandLineArgsParser parser(options);
    ASSERT_EQ(parser.GetOptionsCount(), 3);

    parser.Parse(argc, argv);
    ASSERT_EQ(parser.GetValuesCount(), 3);
    int i = parser.GetValue<int>("--int");
    EXPECT_EQ(i, 5);
    bool b = parser.GetValue<bool>("--bool");
    EXPECT_EQ(b, true);
    std::string s = parser.GetValue<std::string>("--string");
    EXPECT_EQ(s, "default");
}

TEST(CmdLineParser, ParseNoVal)
{
    using namespace CommandLine;
    CommandLineArgsParser::Option opt1{CommandLineArgsParser::ValueType::NOVAL};//, "10"};
    CommandLineArgsParser::Option opt2{CommandLineArgsParser::ValueType::BOOL};//, "false"};
    CommandLineArgsParser::Option opt3{CommandLineArgsParser::ValueType::STRING};//, "default"};
    std::unordered_map<std::string, CommandLine::CommandLineArgsParser::Option> options{
        {"--flag", opt1},
        {"--bool", opt2},
        {"--string", opt3}
    };

    const char* argv[]={"app", "--flag", "--bool", "true", "--string", "default"};
    const int argc = sizeof(argv)/sizeof(char*);
    CommandLineArgsParser parser(options);
    ASSERT_EQ(parser.GetOptionsCount(), 3);

    parser.Parse(argc, argv);
    ASSERT_EQ(parser.GetValuesCount(), 2);
    bool b = parser.GetValue<bool>("--bool");
    EXPECT_EQ(b, true);
    std::string s = parser.GetValue<std::string>("--string");
    EXPECT_EQ(s, "default");

}

TEST(CmdLineParser, ParseEnvVar)
{
    using namespace CommandLine;
    CommandLineArgsParser::Option opt1{CommandLineArgsParser::ValueType::INT};//, "10"};
    CommandLineArgsParser::Option opt2{CommandLineArgsParser::ValueType::BOOL};//, "false"};
    CommandLineArgsParser::Option opt3{CommandLineArgsParser::ValueType::STRING};//, "default"};
    std::unordered_map<std::string, CommandLine::CommandLineArgsParser::Option> options{
        {"--int", opt1},
        {"--bool", opt2},
        {"STRING", opt3}
    };
    ASSERT_TRUE(!setenv("STRING", "default", 1));

    const char* argv[]={"app", "--int", "5", "--bool", "true", "STRING"};
    const int argc = sizeof(argv)/sizeof(char*);
    CommandLineArgsParser parser(options);
    EXPECT_EQ(parser.GetOptionsCount(), 3);

    parser.Parse(argc, argv);
    ASSERT_TRUE(!unsetenv("STRING"));

    ASSERT_EQ(parser.GetValuesCount(), 3);
    int i = parser.GetValue<int>("--int");
    EXPECT_EQ(i, 5);
    bool b = parser.GetValue<bool>("--bool");
    EXPECT_EQ(b, true);
    std::string s = parser.GetValue<std::string>("STRING");
    EXPECT_EQ(s, "default");

}