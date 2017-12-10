#include "download.h"

#include<thread>
#include"./log/log.h"
#include<curl/curl.h>

size_t Download::CallBackDownloadData(char *buff, size_t size, size_t nitems, void *object)
{
	Download* pDownload = (Download *)object;

	//§Õ?????
	size_t written = fwrite(buff, size, nitems, pDownload->GetFilePointer());
	//????????????
    pDownload->SetNowBytes(pDownload->GetNowBytes() + size*written);
	return written;
}

Download::Download()
	:_outFp(NULL)
	, _totalBytes(0)
	, _nowBytes(0)
{}

Download::~Download()
{}

void Download::Init(const std::string &url, const std::string &savePath)
{
	_url = url;
	//????????§³
	_totalBytes = GetFileTotalLen(_url);
	_savePath = savePath;
}

size_t Download::GetFileTotalLen(const std::string &url)
{
	double downloadFileTotalLen = 0.0;
	CURL *lenHandle = curl_easy_init();
	CURLcode res;
	//SET EASY OPT
	curl_easy_setopt(lenHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(lenHandle, CURLOPT_HEADER, 1L);   //use header to get len
	curl_easy_setopt(lenHandle, CURLOPT_NOBODY, 1L);

	res = curl_easy_perform(lenHandle);
	if (res != CURLE_OK)
	{
        //LOG_INFO("curl_easy_perform failed res = %u", res);
	}
	else
	{
		res = curl_easy_getinfo(lenHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileTotalLen);
		if (res != CURLE_OK)
		{
            //LOG_INFO("get file total failed res = %u", res);
			downloadFileTotalLen = -1;
		}
	}

	curl_easy_cleanup(lenHandle);
    //LOG_INFO("get file total len = %lu", (size_t)downloadFileTotalLen);
	return downloadFileTotalLen;
}

bool Download::ThreadDownloadFile()
{
	//???????
	_outFp = fopen(_savePath.c_str(), "wb");
	if (NULL == _outFp)
	{
        //LOG_ERROR("fopen %s failed", _savePath.c_str());
		return false;
	}

	//????curl
	CURL *downloadHandle = curl_easy_init();
	curl_easy_setopt(downloadHandle, CURLOPT_URL, _url.c_str());
	curl_easy_setopt(downloadHandle, CURLOPT_WRITEFUNCTION, &Download::CallBackDownloadData);
	curl_easy_setopt(downloadHandle, CURLOPT_WRITEDATA, this);

	CURLcode res = curl_easy_perform(downloadHandle);
	if (res != CURLE_OK)
	{
        //LOG_ERROR("curl_easy_perform failed return val = %u", res);
		return false;
	}
	curl_easy_cleanup(downloadHandle);	//clean up the curl handle

	fclose(_outFp);	//close the file
	_outFp = nullptr;

	return true;
}

void Download::StartDownloadFile()
{
    std::thread(&Download::ThreadDownloadFile, this).detach();
}

std::string Download::GetFileName(const std::string &url)
{
#ifdef __WIN32
	std::string pathSeparator = "\\";
#else
	std::string pathSeparator = "/";
#endif
	std::string fileName = "";
	if (!url.empty())
	{
		size_t index = url.rfind(pathSeparator);
		if (index != std::string::npos)
		{
			fileName = url.substr(index + pathSeparator.size());
		}
	}
	return fileName;
}

std::string Download::GetFileExtensionName(const std::string &url)
{
	std::string fileExtensionName = "";
	std::string extensionSeparator = ".";
	if (!url.empty())
	{
		size_t index = url.rfind(extensionSeparator);
		if (index != std::string::npos)
		{
			fileExtensionName = url.substr(index + extensionSeparator.length());
		}
	}
	return fileExtensionName;
}

size_t Download::GetTotalBytes()
{
	return _totalBytes;
}

size_t Download::GetNowBytes()
{
	return _nowBytes;
}

void Download::SetTotalBytes(size_t bytes)
{
	_totalBytes = bytes;
}

void Download::SetNowBytes(size_t bytes)
{
	_nowBytes = bytes;
}

FILE* Download::GetFilePointer()
{
	return _outFp;
}

std::string Download::GetSavePath()
{
	return _savePath;
}
