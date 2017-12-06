#include "download.h"



int main()
{

    LOG_INFO("begin download");
    Download download;
    std::string url = "http://sw.bos.baidu.com/sw-search-sp/software/a728d0427ab6f/npp_7.5.2_Installer.exe";
    std::string name;
    std::string path = "./";
    name = download.GetFileName(url);
    //cout<<download.GetFileExtensionName(url)<<endl;
    download.StartDownloadFile(url,path+name);
    while(1)
        ;
    LOG_INFO("over download");
    return 0;
}



