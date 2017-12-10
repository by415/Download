#include<string>
using namespace std;

class Download
{
public:
	Download();
	~Download();

	//?????
    void Init(const std::string &url, const std::string &savePath);

	/*
	*   name:   StartDownloadFile()
	*   func:   create a thread to download file
	*   param:  url:the file url savePath:where to save the file include the file name filename:the name you want to save
	*   date:   2017.12.4
	*   author: ali
	*/
	void StartDownloadFile();

	//?????????§³
	size_t GetTotalBytes();
	//??????????§³
    void SetTotalBytes(size_t bytes);

	//??????§³
	size_t GetNowBytes();

	//??????§³
    void SetNowBytes(size_t bytes);

	//?????????
	FILE* GetFilePointer();

	//??????????¡¤??
	std::string GetSavePath();

public:

	/*
	*   name:   GetFileName()
	*   func:   get file name from url
	*   param:  url:the file url
	*   date:   2017.12.6
	*   author: ali
	*/
	static std::string GetFileName(const std::string &url);

	/*
	*   name:   GetFileTotalLen()
	*   func:   get the total of the download file
	*   param:  url:the file url link
	*   date:   2017.12.4
	*   author: ali
	*/
	static size_t GetFileTotalLen(const std::string &url);

	/*
	*   name:   GetFileExtensionName()
	*   func:   get file extension name from url
	*   param:  url:the file url
	*   date:   2017.12.6
	*   author: ali
	*/
	static std::string GetFileExtensionName(const std::string &url);

	/*
	*   name:   callback_DownloadData()
	*   func:   download process callback func,write data to file
	*   param:  ptr:the buff to write  size:the data size  nmeme:items nums  stream:file descriptor
	*   date:   2017.12.4
	*   author: ali
	*/
	static size_t CallBackDownloadData(char *buff, size_t size, size_t nitems, void *object);

private:
	/*
	*   name:   ThreadDownloadFile()
	*   func:   run the download process
	*   param:  url:the file url savePath:where to save the file include the file name filename:the name you want to save
	*   date:   2017.12.4
	*   author: ali
	*/
	bool ThreadDownloadFile();

private:
	volatile size_t _totalBytes;
	volatile size_t _nowBytes;

	std::string		_url;
	std::string		_savePath;
	FILE *			_outFp;
};
