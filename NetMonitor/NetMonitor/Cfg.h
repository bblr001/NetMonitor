/*
	读配置文件
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
		
	//配置
	// 获取系统运行模式
	int GetRunningMode() {return running_model;}
	int GetEtherNetID() {return ethernet_id;}
	string GetLogPath() {return log_path;}
	int GetCharMode() {return atoi(charset_mode.c_str());}		
	string GetFilterExpress() {return filter_exp;}
	
protected:		
	int running_model;			//系统运行模式：0 手动， 1 自动
	int ethernet_id;			//网卡序号
	string filter_exp;			//过滤表达式
	string log_path;			//日志路径
	string charset_mode;		//日志记录模式
	string mPath;				//程序路径

};

