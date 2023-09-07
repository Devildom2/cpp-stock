#include "Logger.h"
#include <iostream>

/* コンソール出力文字色制御コード */
constexpr const char* ConsoleColorReset     = "\x1b[0m";
constexpr const char* ConsoleColorBlack     = "\x1b[30m";
constexpr const char* ConsoleColorRed       = "\x1b[31m";
constexpr const char* ConsoleColorGreen     = "\x1b[32m";
constexpr const char* ConsoleColorYellow    = "\x1b[33m";
constexpr const char* ConsoleColorBlue      = "\x1b[34m";
constexpr const char* ConsoleColorMagenta   = "\x1b[35m";
constexpr const char* ConsoleColorCyan      = "\x1b[36m";
constexpr const char* ConsoleColorWhite     = "\x1b[37m";
/* シングルトンインスタンス取得 */
Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}
/* コンストラクタ */
Logger::Logger()
    : m_Mutex()
    , m_FileLogger()
{
    /* ログファイルオープン */
    this->m_FileLogger.Open("log.txt");
}
/* デストラクタ */
Logger::~Logger()
{
    /* ログファイルクローズ */
    this->m_FileLogger.Close();
}
/* Informationログ出力 */
void Logger::LogInfo(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Informationログを生成 */
    std::string log_message = StringFormat("[INFO] %s", message);
    /* Informationログを標準出力に出力(シアン) */
    std::cout << ConsoleColorCyan << log_message << ConsoleColorReset << std::endl;
    /* Informationログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
/* Warningログ出力 */
void Logger::LogWarn(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Warningログを生成 */
    std::string log_message = StringFormat("[WARN] %s", message);
    /* Warningログを標準出力に出力(黄色) */
    std::cerr << ConsoleColorYellow << log_message << ConsoleColorReset << std::endl;
    /* Warningログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
/* Errorログ出力 */
void Logger::LogError(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Errorログを生成 */
    std::string log_message = StringFormat("[ERROR] %s", message);
    /* Errorログを標準出力に出力(赤色) */
    std::cerr << ConsoleColorRed << log_message << ConsoleColorReset << std::endl;
    /* Errorログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
