#include "CLogger.h"

void CLogger::logger(string logMsg)
{
	string file = "log.out";
    string now = CUtils::dateToString(time(0), "%Y-%m-%d %X");
    ofstream ofs(file.c_str(), std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}
