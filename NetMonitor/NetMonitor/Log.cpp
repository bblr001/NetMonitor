#include "StdAfx.h"
#include "Log.h"

CLog CLog:: m_LogInstance;
CLog::CLog(void)
{
	m_bFileInfo=false;
	m_iCurYear=0;
	m_iCurMonth=0;
	m_iCurDay=0;
	m_csPathMain.Empty();			
	m_csPathYear.Empty();			
	m_csPathMonth.Empty();			
	m_csPathDay.Empty();
	m_csFileNameYear.Empty();		
	m_csFileNameMonth.Empty();		
	m_csFileNameDay.Empty();
	m_csFileNameDetail.Empty();
}


CLog::~CLog(void)
{
}


bool CLog::SetLogFileInfo(char DiskPartition, LPCTSTR lpstrFolder,LPCTSTR lpstMainFileName)
{
	//创建主目录
	m_csPathMain=DiskPartition;
	m_csPathMain+=_T(":\\");
	m_csPathMain+=lpstrFolder;
	int retval= PathFileExists(m_csPathMain);
	if(retval == 0)
	{
		CreateDirectory(m_csPathMain,NULL);
	}

	m_bFileInfo=true;
	return true;
}

bool CLog::SetLogFileInfo(std::string path)
{
	if(!path.empty())
	{
		m_csPathMain.SetString(path.c_str()) ;
		m_bFileInfo=true;
		return true;
	}else{
		m_csPathMain = "";
		m_bFileInfo=false;
		return false;
	}
}
bool CLog::CreatDir()
{
	int nCurYear,nCurMonth,nCurDay,nCurHour,nCurMinute,nCurSecond;
	CTime curtime=CTime::GetCurrentTime();	
	nCurYear = curtime.GetYear();
	nCurMonth = curtime.GetMonth();
	nCurDay = curtime.GetDay();
	nCurHour = curtime.GetHour();
	nCurMinute = curtime.GetMinute();
	nCurSecond = curtime.GetSecond();

	CString cstemp;
	int retval;

	//换年，创建年份子目录
	if(m_iCurYear!=nCurYear)
	{
		m_iCurYear=nCurYear;
		m_iCurMonth=0;
		m_iCurDay=0;
		cstemp.Format(_T("%4d"),nCurYear);
		m_csPathYear=m_csPathMain+"\\"+cstemp;
		m_csFileNameYear=m_csPathYear+"\\"+cstemp+_T(".txt");
		cstemp=m_csPathYear;
		retval = PathFileExists(cstemp.GetBuffer(cstemp.GetLength()));
		if(retval == 0)
		{
			CreateDirectory(cstemp.GetBuffer(cstemp.GetLength()),NULL);
		}
	}
	//创建月份子目录
	if(m_iCurMonth!=nCurMonth)
	{
		m_iCurMonth=nCurMonth;
		m_iCurDay=0;
		cstemp.Format(_T("%04d_%02d"),nCurYear,nCurMonth);
		m_csPathMonth=m_csPathYear+"\\"+cstemp;
		m_csFileNameMonth=m_csPathMonth+"\\"+cstemp+_T(".txt");
		cstemp=m_csPathMonth;
		retval = PathFileExists(cstemp.GetBuffer(cstemp.GetLength()));
		if(retval == 0)
		{
			CreateDirectory(cstemp.GetBuffer(cstemp.GetLength()),NULL);
		}
	}
	//创建日期子目录
	if(m_iCurDay!=nCurDay)
	{
		m_iCurDay=nCurDay;
		cstemp.Format(_T("\\%04d_%02d_%02d"),nCurYear,nCurMonth,nCurDay);
		m_csPathDay=m_csPathMonth+"\\"+cstemp;
		m_csFileNameDay=m_csPathDay+"\\"+cstemp+_T(".txt");
		cstemp=m_csPathDay;
		m_csFileNameDetail=m_csPathDay;
		retval = PathFileExists(cstemp.GetBuffer(cstemp.GetLength()));
		if(retval == 0)
		{
			CreateDirectory(cstemp.GetBuffer(cstemp.GetLength()),NULL);
		}
	}
	
	
	return true;
}


bool CLog::WriteLog(LPCTSTR lpcstDescription,LogFileType filetype)
{
	if(!m_bFileInfo)
		return false;
	int nCurYear,nCurMonth,nCurDay,nCurHour,nCurMinute,nCurSecond;
	LPCTSTR strFileName;
	CTime curtime=CTime::GetCurrentTime();	
	nCurYear = curtime.GetYear();
	nCurMonth = curtime.GetMonth();
	nCurDay = curtime.GetDay();
	nCurHour = curtime.GetHour();
	nCurMinute = curtime.GetMinute();
	nCurSecond = curtime.GetSecond();

	CString csSystemTime ;//= curtime.Format("%Y-%m-%d %H:%M:%S\t");
	csSystemTime.Format(_T("%04d_%02d_%02d %02d_%02d_%02d"), 
		nCurYear, nCurMonth, nCurDay, 
		nCurHour, nCurMinute, nCurSecond);
	

	//判断是否因时间改变需要新建目录
	switch(filetype)
	{
		case logYear:
			if(m_iCurYear!=nCurYear)
			{
				CreatDir();
			}
			strFileName=m_csFileNameYear;
			break;
		case logMonth:
			if(m_iCurYear!=nCurYear || m_iCurMonth!=nCurMonth)
			{
				CreatDir();
			}
			strFileName=m_csFileNameMonth;
			break;
		case logDay:
			if(m_iCurYear!=nCurYear || m_iCurMonth!=nCurMonth || m_iCurDay!=nCurDay)
			{
				CreatDir();
			}
			strFileName=m_csFileNameDay;
			break;
		case logDetail:
			if(m_iCurYear!=nCurYear || m_iCurMonth!=nCurMonth || m_iCurDay!=nCurDay)
			{
				CreatDir();
			}
			m_csFileNameDetail=m_csFileNameDetail+"\\"+csSystemTime+_T(".txt");
			strFileName=m_csFileNameDetail;
			break;
	}
	CFile file;
	CString str;
	str = lpcstDescription;
	
	csSystemTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), 
		nCurYear, nCurMonth, nCurDay, 
		nCurHour, nCurMinute, nCurSecond);
	///str = csSystemTime + ":\r\n" + str + _T("\r\n");
	str = csSystemTime + ":\t" + str + _T("\r\n");

	if (!file.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		return FALSE;
	}
	file.SeekToEnd();
	file.Write(str, str.GetLength());
	file.Close();
	return true;
}
