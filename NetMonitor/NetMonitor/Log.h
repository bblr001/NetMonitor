#pragma once
#include <string>

//该类实现各种运行日志的记录，将日志存储到指定的磁盘分区和主目录下，并在主目录下
//自动分成年、月、日二三四级子目录存储
//可设置年主日志、月主日志，日主日志和每天详细日志（每次写入单独生成）四种方式存储
enum LogFileType{logYear,logMonth,logDay,logDetail};
class CLog
{
public:
	static CLog* GetInstance(){return &m_LogInstance;}
	~CLog(void);
	bool SetLogFileInfo(char DiskPartition, LPCTSTR lpstrFolder, LPCTSTR lpstMainFileName=NULL);//初始化日志文件信息
	bool SetLogFileInfo(std::string path);	//初始化日志文件信息																	
	bool WriteLog(LPCTSTR lpcstDescription,LogFileType filetype=logYear);	//写日志
	LogFileType m_LogType;
private:
	CLog(void);
	bool m_bFileInfo;				//是否已完成初始化

	CString m_csPathMain;			//主路径
	CString m_csPathYear;			//年日志路径
	CString m_csPathMonth;			//月日志路径
	CString m_csPathDay;			//天日志路径

	CString m_csFileNameYear;		//年主日志文件名
	CString m_csFileNameMonth;		//月主日志文件名
	CString m_csFileNameDay;		//天主日志文件名
	CString m_csFileNameDetail;		//详细日志文件名

	bool CreatDir();				//创建日志目录
	int m_iCurYear;					//上一次更新的年时间
	int m_iCurMonth;				//上一次更新的月时间
	int m_iCurDay;					//上一次更新的日时间
	static CLog m_LogInstance;

	/*class GC
	{
	public:
		GC()
		{
			//cout<<"GC construct ..."<<endl;
		}
		~GC()
		{
			// We can destory all the resouce here, eg:db connector, file handle and so on
			if (m_pInstance != NULL)
			{
				//cout<<"Call the GC's destructor"<<endl;
				delete m_pInstance;
				m_pInstance = NULL;
			}
		}
	};
	static GC gc;*/
};

