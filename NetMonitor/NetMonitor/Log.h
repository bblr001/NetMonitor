#pragma once
#include <string>

//����ʵ�ָ���������־�ļ�¼������־�洢��ָ���Ĵ��̷�������Ŀ¼�£�������Ŀ¼��
//�Զ��ֳ��ꡢ�¡��ն����ļ���Ŀ¼�洢
//������������־��������־��������־��ÿ����ϸ��־��ÿ��д�뵥�����ɣ����ַ�ʽ�洢
enum LogFileType{logYear,logMonth,logDay,logDetail};
class CLog
{
public:
	static CLog* GetInstance(){return &m_LogInstance;}
	~CLog(void);
	bool SetLogFileInfo(char DiskPartition, LPCTSTR lpstrFolder, LPCTSTR lpstMainFileName=NULL);//��ʼ����־�ļ���Ϣ
	bool SetLogFileInfo(std::string path);	//��ʼ����־�ļ���Ϣ																	
	bool WriteLog(LPCTSTR lpcstDescription,LogFileType filetype=logYear);	//д��־
	LogFileType m_LogType;
private:
	CLog(void);
	bool m_bFileInfo;				//�Ƿ�����ɳ�ʼ��

	CString m_csPathMain;			//��·��
	CString m_csPathYear;			//����־·��
	CString m_csPathMonth;			//����־·��
	CString m_csPathDay;			//����־·��

	CString m_csFileNameYear;		//������־�ļ���
	CString m_csFileNameMonth;		//������־�ļ���
	CString m_csFileNameDay;		//������־�ļ���
	CString m_csFileNameDetail;		//��ϸ��־�ļ���

	bool CreatDir();				//������־Ŀ¼
	int m_iCurYear;					//��һ�θ��µ���ʱ��
	int m_iCurMonth;				//��һ�θ��µ���ʱ��
	int m_iCurDay;					//��һ�θ��µ���ʱ��
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

