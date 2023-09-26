#pragma once
#include <fstream>

/* File Loggerクラス宣言 */
class FileLogger
{
public:
    /* コンストラクタ */
    FileLogger();
    /* コンストラクタ */
    FileLogger(const std::string& file_path);
    /* デストラクタ */
    ~FileLogger();
    /* ファイルオープン */
    void Open(const std::string& file_path);
    /* ファイルクローズ */
    void Close();
    /* ファイルオープン確認 */
    bool IsOpened();
    /* ファイル書き込み */
    void Write(const std::string& log);
private:
    /* ログファイルパス */
    std::string m_FilePath;
    /* ファイル出力ストリーム */
    std::ofstream m_Stream;
};
