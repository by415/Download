#include "log.h"


#define LOG_PATH "log.txt"

using namespace std;

char msg[LOG_BUFF];
std::mutex mtx;


void GetNowTime(char* buf,const int len)
{
    time_t timeValue;
    struct tm* timeInfo;
    time(&timeValue);
    timeInfo=localtime(&timeValue);
    strftime(buf,len,"%Y-%m-%d %I:%M:%S",timeInfo);
}

void CodeLog::PrintLog(const char* level,const char* fileName,const char* funcName,const long long line,const char* msg)
{
    if(mtx.try_lock())
    {
        FILE* fp;
        fp = fopen(LOG_PATH,"a+");
        if(NULL == fp)
        {
            cout<<"log file open failure"<<endl;
            exit(-1);
        }
        char timeBuf[50]={'0'};
        GetNowTime(timeBuf,sizeof(timeBuf));
        fprintf(fp,"%s %s %s->%s->%lld:%s\n",level,timeBuf,fileName,funcName,line,msg);
        fflush(fp);
        fclose(fp);
    }
    mtx.unlock();
}
