#pragma once
#include "StringFormat.h"
#include "FileLogger.h"
#include <string>
#include <mutex>
/* Loggerクラス宣言 */
class Logger
{
public:
    /* 書式指定Informationログ */   /* TODO:可変引数テンプレートについて調べる */
    template<typename ... Args>
    static void Info(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのInformationログ出力呼び出し */
        Logger::GetInstance().LogInfo(StringFormat(format, std::forward<Args>(args) ...));
    }
    /* 書式指定Warningログ */
    template<typename ... Args>
    static void Warn(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのWarningログ出力呼び出し */
        Logger::GetInstance().LogWarn(StringFormat(format, std::forward<Args>(args) ...));
    }
    /* 書式指定Errorログ */
    template<typename ... Args>
    static void Error(const std::string& format, Args& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのErrorログ出力呼び出し */
        Logger::GetInstance().LogError(StringFormat(format, std::forward<Args>(args)  ...));
    }
private:
    /* シングルトンインスタンス取得 */
    static Logger& GetInstance();
private:
    /* コンストラクタ */
    Logger();
    /* デストラクタ */
    ~Logger();
public:
    /* コピーコンストラクタを削除 */
    Logger(const Logger&) = delete;
    /* ムーブコンストラクタを削除 */
    Logger(Logger&&) = delete;
    /* コピー代入オペレータを削除 */
    Logger& operator=(const Logger&) = delete;
    /* ムーブ代入オペレータを削除 */
    Logger& operator=(Logger&&) = delete;
private:
    /* Informationログ出力 */
    void LogInfo(const std::string& message);
    /* Warningログ出力 */
    void LogWarn(const std::string& message);
    /* Errorログ出力 */
    void LogError(const std::string& message);
private:
    /* ミューテックス */
    std::mutex m_Mutex;
    /* File Loggerクラスインスタンス */
    FileLogger m_FileLogger;
};
