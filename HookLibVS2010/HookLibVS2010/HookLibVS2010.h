
// HookLibVS2010.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CHookLibVS2010App: 
// 有关此类的实现，请参阅 HookLibVS2010.cpp
//

class CHookLibVS2010App : public CWinApp
{
public:
	CHookLibVS2010App();

// 重写
public:
	virtual BOOL InitInstance();

private:
	void HookLoadlibrary();
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CHookLibVS2010App theApp;