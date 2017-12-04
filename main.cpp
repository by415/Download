#include "download.h"

#ifdef _WIN32 
#define SIGN "\\" 
#else 
#define SIGN "/" 
#endif






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



/*
void StartDownload()
{
    Download test;
    //std::string url = "http://sw.bos.baidu.com/sw-search-sp/software/a728d0427ab6f/npp_7.5.2_Installer.exe";

    //std::string url = "http://sw.bos.baidu.comhttp://sw.bos.baidu.com/sw-search-sp/software/10eafbd67f16b/QQ_8.9.6.22404_setup.exe";
    
    std::string url = "http://sw.bos.baidu.com/sw-search-sp/software/e25c4cc36a934/QQ_8.9.6.22427_setup.exe";
    //std::string url = "www.baidu.com";
    std::string path = "./";
    long long len = test.GetFileTotalLen(url);
    cout<<"total len="<<len<<endl;
    std::string name;
    GetFileName(url,name);
    int ret =-1;
    ret = test.StartDownloadFile(url,path+name,name);
    //test.Init(url,path);

}
*/


int main()
{

    LOG_INFO("begin download");
    //std::thread tid(StartDownload);
    //tid.join();
    //std::string url = "http://sw.bos.baidu.com/sw-search-sp/software/e25c4cc36a934/QQ_8.9.6.22427_setup.exe";
    Download download;
    std::string url = "http://sw.bos.baidu.com/sw-search-sp/software/a728d0427ab6f/npp_7.5.2_Installer.exe";
    std::string name;
    GetFileName(url,name);
    std::string path = "./";
    //download.ThreadDownloadFile(url,path+name);
    download.StartDownloadFile(url,path+name);
    while(1)
        ;
    LOG_INFO("over download");
    return 0;
}



