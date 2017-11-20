#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<curl/curl.h>

using namespace std;



#ifdef _WIN32 
#define SIGN "\\" 
#else 
#define SIGN "/" 
#endif

class Download
{
    public:
        Download()
        {
            _totalBytes=0;
            _nowBytes=0;
        }
        ~Download()
        {}

        void Init(const std::string& url,const std::string& path);
        void Start();
        float GetProcess();
        int GetRemainingTime(); 
    private:
        volatile long long _totalBytes;
        volatile long long _nowBytes;
};


void Download::Start()
{}

float Download::GetProcess()
{
    return 0;
}

int GetRemainingTime()
{
    return 0;
}

long long DownloadData(char *ptr,long long size,long long nmemb,void *stream)
{
    long long written = fwrite(ptr,size,nmemb,(FILE*)stream);
    return written;
}




void GetFileName(const std::string& url,std::string& fileName)
{
    //std::string fileName;
    if(!url.empty())
    {
        size_t index;
        index = url.rfind("/");
        fileName = url.substr(index);


        cout << "name = "<< fileName <<endl;

        if(!fileName.empty())
        {
            if(fileName.size()>1)
                fileName.erase(0,1);
            else
                fileName.clear();
        }
        //return fileName;
    }
    //return "";
}

void Download::Init(const std::string& url,const std::string& path)
{

    cout << url <<endl; 

    CURL *curl;
    std::string fileName;
    std::string pathAndName;
    FILE *outFp;
    GetFileName(url,fileName);
    if(fileName.empty())
    {
        printf("invalid url,we can get the file name\n");
        exit(-1);
    }
    cout<<fileName<<endl;
    pathAndName = path + SIGN + fileName;

    cout<<pathAndName<<endl;
    outFp = fopen(pathAndName.c_str(),"wb");
    if(NULL == outFp)
    {
        printf("open file failure\n");
        exit(-1);
    }
    
    // set curl args
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    curl_easy_setopt(curl,CURLOPT_URL,url.data());
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,DownloadData);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,outFp);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
}   






int main()
{

    std::string url = "http://sw.bos.baidu.comhttp://sw.bos.baidu.com/sw-search-sp/software/10eafbd67f16b/QQ_8.9.6.22404_setup.exe";
    std::string path = ".";
    Download test;
    test.Init(url,path);
    return 0;
}



