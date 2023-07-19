#include <iostream>
#include <string>
#include "Logger.h"

int main() {

    /* Informationログ出力 */
    std::string str = "Information Log"; 
    Logger::Info(str);
 
    /* Warningログ出力 */
    std::string warning_message = "warning text";
    Logger::Warn("Warning Log : %s", warning_message);

    /* Errorログ出力 */
    std::string error_message = "error text";
    int error_code = -1;
    Logger::Error("Error Log : %s %d", error_message, error_code);
    
    /* Informationログ出力 */
    Logger::Info("Information Log2"); 
}