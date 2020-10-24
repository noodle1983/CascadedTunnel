#include "FileSink.h"
#include "Log.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm> 
#include <cstring>

using namespace nd;
using namespace std;
namespace fs = std::filesystem;

static inline std::tm localtime_nd(std::time_t timer)
{
    std::tm bt;
#if defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    localtime_r(&timer, &bt);
#endif
    return bt;
}
//-----------------------------------------------------------------------------

FileSink::FileSink(std::string& prefix, Severity severity)
    : prefixM(prefix)
    , timeFormatM("%Y-%m-%d_%H-%M-%S")
    , severityM(severity)
    , keepHisNoM(0)
    , switchDaysM(1)
    , curDaysM(0)
{
}

//-----------------------------------------------------------------------------

void FileSink::log(const LogMeta* theMeta)
{
    if (theMeta->severityM < severityM) {return ;}

    std::time_t now = std::chrono::system_clock::to_time_t(theMeta->timepointM);
    int64_t curDays = now / (24 * 3600);
    struct ::tm nowTm = localtime_nd(now);
    char fileTimeStr[128];
    strftime(fileTimeStr, sizeof(fileTimeStr), timeFormatM.c_str(), &nowTm);

    if (curDaysM == 0){curDaysM = curDays;}
    int64_t daysDiff = curDays - curDaysM;
    if (switchDaysM > 0 && daysDiff >= switchDaysM && fileHandleM.is_open()){
        fileHandleM.close();
        curDaysM = curDays;
    }

    if (!fileHandleM.is_open())
    {
        checkDelHisFile();
        string filename = prefixM + "_" + fileTimeStr + ".log";
        fileHandleM.open(filename.c_str(), std::ios_base::out|std::ios_base::app);
        if (!fileHandleM.good()){
            cerr << "failed to open file[" << filename << "] errno:" << errno << endl;
            abort();
        }
    }
    int ms = chrono::time_point_cast<chrono::milliseconds>(theMeta->timepointM).time_since_epoch().count() % 1000;
    char logTimeStr[256];
    snprintf(logTimeStr, sizeof(logTimeStr), "%s.%03d", fileTimeStr, ms);

    // log format: time severity (file:no) msg
    fileHandleM << logTimeStr << severityToStr((Severity)theMeta->severityM) 
        << theMeta->streamM.str() << endl << flush;

}

//-----------------------------------------------------------------------------

void FileSink::checkDelHisFile()
{
    size_t found = prefixM.find_last_of("/\\");
    string dirname("."); 
    string filePrefix(prefixM); 
    if (found != string::npos){
        dirname = prefixM.substr(0,found);
        filePrefix = prefixM.substr(found+1);
    }

    vector<fs::path> all_log_files;
    for(auto& p: fs::directory_iterator(dirname)){
        if (!p.is_regular_file()) {continue;}
        
        const fs::path& filepath = p.path(); 
        const string& filename = filepath.filename();
        if (memcmp(filename.c_str(), filePrefix.c_str(), filePrefix.length()) == 0 //begin with prefix
                && memcmp(filename.c_str() + filename.length() - 4, ".log", 4) == 0)
        {
            all_log_files.push_back(filepath);
        }
    }
    if ((int)all_log_files.size() <= keepHisNoM){return;}

    sort(all_log_files.begin(), all_log_files.end());
    int delNumber = all_log_files.size() - keepHisNoM;
    for(int i = 0; i < delNumber && i < (int)all_log_files.size(); i++){
        CFG_DEBUG("remove file:" << all_log_files[i] << ", cur day:" << curDaysM);
        remove(all_log_files[i]);
    }
}
