
// HookLibVS2010.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHookLibVS2010App: 
// �йش����ʵ�֣������ HookLibVS2010.cpp
//

class CHookLibVS2010App : public CWinApp
{
public:
	CHookLibVS2010App();

// ��д
public:
	virtual BOOL InitInstance();

private:
	void HookLoadlibrary();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHookLibVS2010App theApp;