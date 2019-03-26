/*
	�������ļ�
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Noncopyable.h"
using namespace std;


class Ccfg: 
	public noncopyable
{
protected:
	
	//static Ccfg *m_Instance;
	static shared_ptr<Ccfg> m_Instance;
public:
	Ccfg(void);
	virtual ~Ccfg(void);
	static shared_ptr<Ccfg> GetInstance();
	int Load(string filepath);
	void Reload();
	int Save();
	bool CreateCfg(string filepath);
		
	//����
	// ��ȡϵͳ����ģʽ
	int GetRunningMode() {return running_model;}
	int GetEtherNetID() {return ethernet_id;}
	string GetLogPath() {return log_path;}
	int GetCharMode() {return atoi(charset_mode.c_str());}		
	string GetFilterExpress() {return filter_exp;}
	
protected:		
	int running_model;			//ϵͳ����ģʽ��0 �ֶ��� 1 �Զ�
	int ethernet_id;			//�������
	string filter_exp;			//���˱��ʽ
	string log_path;			//��־·��
	string charset_mode;		//��־��¼ģʽ
	string mPath;				//����·��

};

