#include "download.h"


/*
*   name:   callback_DownloadData()
*   func:   download process callback func,write data to file
*   param:  ptr:the buff to write  size:the data size  nmeme:items nums  stream:file descriptor
*   date:   2017.12.4
*   author: ali
*/
long long callback_DownloadData(char *ptr,long long size,long long nmemb,void *stream)
{
    long long written = fwrite(ptr,size,nmemb,(FILE*)stream);
    return written;
}

/*
*   func:call_back to get file len
*
long long GetFileSize(void* ptr, long size, long nmemb, void* data)
{  
    return (long long)(size * nmemb);  
}
*/


/*
*   name:   GetFileTotalLen()
*   func:   get the total of the download file
*   param:  url:the file url link
*   date:   2017.12.4
*   author: ali
*/
long long Download::GetFileTotalLen(const std::string &url)
{
    double downloadFileTotalLen=0;
    CURL *lenHandle = curl_easy_init();
    CURLcode res;
    //SET EASY OPT
    curl_easy_setopt(lenHandle,CURLOPT_URL,url.c_str());
    curl_easy_setopt(lenHandle,CURLOPT_HEADER,1);   //use header to get len 
    curl_easy_setopt(lenHandle,CURLOPT_NOBODY,1L);
    //curl_easy_setopt(lenHandle,CURLOPT_HEADERFUNCTION,GetFileSize);
    //curl_easy_setopt(lenHandle,CURLOPT_FOLLOWLOCATION,1L);


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

/*
*   name:   ThreadDownloadFile()
*   func:   run the download process
*   param:  url:the file url savePath:where to save the file include the file name filename:the name you want to save
*   date:   2017.12.4
*   author: ali
*/
int Download::ThreadDownloadFile(const std::string &url,const std::string &savePath,const std::string &fileName)
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
    curl_easy_setopt(downloadHandle,CURLOPT_WRITEFUNCTION,&callback_DownloadData);
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

/*
*   name:   StartDownloadFile()
*   func:   create a thread to download file
*   param:  url:the file url savePath:where to save the file include the file name filename:the name you want to save
*   date:   2017.12.4
*   author: ali
*/
void Download::StartDownloadFile(const std::string &url,const std::string &savePath,const std::string &fileName)
{
    std::thread downloadThread(Download::ThreadDownloadFile,url,savePath,fileName);
    downloadThread.detach();
}
