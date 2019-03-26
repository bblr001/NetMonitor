#include "StdAfx.h"
#include "Cfg.h"
#include <stdio.h>
//#include <Windows.h>
#ifndef MAX_PATH
#define MAX_PATH 255
#else
#endif

//Ccfg* Ccfg::m_Instance = NULL;
shared_ptr<Ccfg> Ccfg::m_Instance = NULL;
Ccfg::Ccfg(void)
{
	
}


Ccfg::~Ccfg(void)
{
	
}

bool Ccfg::CreateCfg(string filepath)
{

	
	return false;
}

int Ccfg::Save()
{
	//±£´æ

	
	return 0;
}

int Ccfg::Load(string filepath)
{	
	mPath = filepath;
	char buff[MAX_PATH] = {0};	
	running_model = GetPrivateProfileInt("BASE","mode",0, filepath.c_str());
	
	ethernet_id = GetPrivateProfileInt("BASE","ethernet_id",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),_T("logpath"),"",buff,MAX_PATH,filepath.c_str());
	log_path = buff;

	GetPrivateProfileString(_T("BASE"),_T("charsetmode"),"0",buff,MAX_PATH,filepath.c_str());
	charset_mode = buff;

	GetPrivateProfileString(_T("BASE"),_T("filter_express"),"",buff,MAX_PATH,filepath.c_str());
	filter_exp = buff;
	
	return true;
}

void Ccfg::Reload()
{
	Load(mPath);
}

shared_ptr<Ccfg> Ccfg::GetInstance()
{
	if (m_Instance == NULL)
		m_Instance = make_shared<Ccfg>();
	return m_Instance;
}

