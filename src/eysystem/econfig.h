#pragma once
#include "../osstd.h"
using namespace osstd;

namespace econfig{
    class EyConfig{
        struct debugMode{
            bool lex_TokenGroupMsg;
            bool lex_TokenContent;
            bool ast_StatMsg;
            bool ast_SaveAnyTimesAstTreeToLog;
            bool cgen_OutputGeneratoringCode;
        };
        debugMode DebugMode;
        bool ExitTip;
        string DependOnConfigFile;
    public:
        EyConfig()=default;

        friend class ConfigReader;
        friend class ConfigWriter;
    };

    class ConfigReader{
        EyConfig Config;
        CJsonObject Json;
        string Path;
    public:
        ConfigReader(string path);
        EyConfig Get(); // 返回Config
        bool IsExist();
        bool Read();
    };

    class ConfigWriter{
        EyConfig Config;
        CJsonObject Json;
        string path;
    public:
        ConfigWriter(string path);
        EyConfig Get();
        bool IsExist();

        bool AddConfig(const string& key, const string& content);
        bool AddConfig(const string& key, bool content);
        bool AddConfig(const string& key, int content);
        bool AddConfig(const string& key, CJsonObject object);

        bool ModiftyConfig(const string& key, const string& content);
        bool ModiftyConfig(const string& key, bool content);
        bool ModiftyConfig(const string& key, CJsonObject object);
    };
}