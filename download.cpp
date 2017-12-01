#include "download.h"

long long DownloadData(char *ptr,long long size,long long nmemb,void *stream)
{
    long long written = fwrite(ptr,size,nmemb,(FILE*)stream);
    return written;
}

long long GetFileSize(void* ptr, long size, long nmemb, void* data)
{  
    return (long long)(size * nmemb);  
}

long long Download::GetFileTotalLen(const std::string url)
{
    double downloadFileTotalLen=0;
    CURL *lenHandle = curl_easy_init();
    CURLcode res;
    //SET EASY OPT
    curl_easy_setopt(lenHandle,CURLOPT_URL,url.c_str());
    //curl_easy_setopt(lenHandle,CURLOPT_HEADER,1);
    curl_easy_setopt(lenHandle,CURLOPT_NOBODY,1L);
    curl_easy_setopt(lenHandle,CURLOPT_HEADERFUNCTION,GetFileSize);
    curl_easy_setopt(lenHandle,CURLOPT_FOLLOWLOCATION,1L);


    res = curl_easy_perform(lenHandle);
    res = curl_easy_getinfo(lenHandle,CURLINFO_CONTENT_LENGTH_DOWNLOAD,&downloadFileTotalLen);
    if(res != CURLE_OK)
    {
        LOG_INFO("get file total failed");
        downloadFileTotalLen = -1;
    }
    curl_easy_cleanup(lenHandle);
    return downloadFileTotalLen;
}

int Download::StartDownloadFile(const std::string &url,const std::string &savePath,const std::string &fileName)
{
    CURL *downloadHandle = curl_easy_init();
    
    FILE *outFp = NULL;
    outFp = fopen(savePath.c_str(),"wb");
    if(NULL == outFp)
    {
        LOG_ERROR("fopen %s failed",savePath.c_str());
        return -1;
    }
    curl_easy_setopt(downloadHandle,CURLOPT_URL,url.c_str());
    curl_easy_setopt(downloadHandle,CURLOPT_WRITEFUNCTION,&DownloadData);
    curl_easy_setopt(downloadHandle,CURLOPT_WRITEDATA,outFp);
    
    CURLcode res = curl_easy_perform(downloadHandle);
    if(res != CURLE_OK)
    {
        LOG_ERROR("curl_easy_perform failed");
        return -2;
    }
    curl_easy_cleanup(downloadHandle);
    return 0;
}


