#pragma once
#include<iostream>
#include<stdlib.h>
#include<mutex>
#include<stdio.h>
#define LOG_BUFF 1024


extern char msg[LOG_BUFF];

#define __LOG_SWITCH
#ifdef __LOG_SWITCH
#define LOG_INFO(...)\
    memset(msg,0,sizeof(msg));\
    sprintf(msg,__VA_ARGS__);\
    CodeLog::PrintLog("INFO",__FILE__,__func__,__LINE__,msg);

#define LOG_ERROR(...)\
    memset(msg,0,sizeof(msg));\
    sprintf(msg,__VA_ARGS__);\
    CodeLog::PrintLog("ERROR",__FILE__,__func__,__LINE__,msg);
    
#define LOG_WARN(...)\
    memset(msg,0,sizeof(msg));\
    sprintf(msg,__VA_ARGS__);\
    CodeLog::PrintLog("WARN",__FILE__,__func__,__LINE__,msg);
#else
#define LOG_INFO(...)
#define LOG_ERROR(...)
#define LOG_WARN(...)

#endif  //____LOG_SWITCH



class CodeLog
{
    public:
        static void PrintLog(const char* level,const char* fileName,const char* funcName,const long long line,const char* msg);
};
