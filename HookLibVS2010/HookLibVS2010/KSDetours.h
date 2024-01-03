#pragma once

#include <windows.h>
#include "include/detours.h"
#include <string>

//用于Hook LoadLibraryExW 方法，忽略部分 Satellite DLL相关的加载，解决 Satellite DLL Hijacking的情况
//详细问题见 http://www.hackdig.com/?05/hack-3316.htm
//这里主要根据进程名称做Dll的匹配，如果匹配上就不加载这个Dll


// 定义原始LoadLibrary函数指针类型
typedef HMODULE(WINAPI* LoadLibraryFunc)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);

// 原始LoadLibrary函数指针
LoadLibraryFunc originalLoadLibrary = NULL;


namespace KSDetours
{
	std::wstring exeName = L"";		//设置进程名字

	//从路径中找到文件名字
	std::wstring GetFileNameFromPath(const std::wstring& strFilePath)
	{
		std::wstring strFileName(L"");
		int nPos = strFilePath.find_last_of(L"\\");

		if (std::string::npos != nPos)
		{
			strFileName = strFilePath.substr(nPos + 1, strFilePath.length() - nPos - 1);
		}

		return strFileName;
	}

	// 重定义的LoadLibraryExw函数
	HMODULE WINAPI MyLoadLibrary(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
	{
		//// 检查是否是要拒绝加载的DLL
		if (GetFileNameFromPath(lpLibFileName).compare(L"") != 0  &&
			GetFileNameFromPath(lpLibFileName).find(exeName) != std::wstring::npos)
		
		/*if ( wcscmp(lpLibFileName, L"D:\\users\\kai.yin\\Documents\\Visual Studio 2015\\Projects\\HookLibVS2010\\Release\\HookLibVS2010LOC.dll") == 0
			|| wcscmp(lpLibFileName, L"D:\\users\\kai.yin\\Documents\\Visual Studio 2015\\Projects\\HookLibVS2010\\Release\\HookLibVS2010CHS.dll") == 0)*/
		{
			// 在这里可以添加拒绝加载的处理逻辑
			//MessageBox(NULL, L"Loading test.dll is not allowed!", L"Hooked LoadLibrary", MB_OK | MB_ICONERROR);
			hFile = NULL;
			return NULL; // 返回NULL表示加载失败
		}

		// 调用原始LoadLibrary函数
		return originalLoadLibrary(lpLibFileName, hFile, dwFlags);
	}

	void HookLoadlibrary()
	{
		//如果进程名没有设置就退出
		if (exeName.compare(L"") == 0)
		{
			return;
		}

		// DetourTransactionBegin/Commit/Abort用于事务性地进行挂钩
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		// 获取LoadLibrary地址
		originalLoadLibrary = (LoadLibraryFunc)GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryExW");

		// 挂钩LoadLibrary函数
		DetourAttach(&(PVOID&)originalLoadLibrary, MyLoadLibrary);

		// 提交挂钩事务
		DetourTransactionCommit();
	}
}
