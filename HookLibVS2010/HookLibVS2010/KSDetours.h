#pragma once

#include <windows.h>
#include "include/detours.h"
#include <string>

//����Hook LoadLibraryExW ���������Բ��� Satellite DLL��صļ��أ���� Satellite DLL Hijacking�����
//��ϸ����� http://www.hackdig.com/?05/hack-3316.htm
//������Ҫ���ݽ���������Dll��ƥ�䣬���ƥ���ϾͲ��������Dll


// ����ԭʼLoadLibrary����ָ������
typedef HMODULE(WINAPI* LoadLibraryFunc)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);

// ԭʼLoadLibrary����ָ��
LoadLibraryFunc originalLoadLibrary = NULL;


namespace KSDetours
{
	std::wstring exeName = L"";		//���ý�������

	//��·�����ҵ��ļ�����
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

	// �ض����LoadLibraryExw����
	HMODULE WINAPI MyLoadLibrary(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
	{
		//// ����Ƿ���Ҫ�ܾ����ص�DLL
		if (GetFileNameFromPath(lpLibFileName).compare(L"") != 0  &&
			GetFileNameFromPath(lpLibFileName).find(exeName) != std::wstring::npos)
		
		/*if ( wcscmp(lpLibFileName, L"D:\\users\\kai.yin\\Documents\\Visual Studio 2015\\Projects\\HookLibVS2010\\Release\\HookLibVS2010LOC.dll") == 0
			|| wcscmp(lpLibFileName, L"D:\\users\\kai.yin\\Documents\\Visual Studio 2015\\Projects\\HookLibVS2010\\Release\\HookLibVS2010CHS.dll") == 0)*/
		{
			// �����������Ӿܾ����صĴ����߼�
			//MessageBox(NULL, L"Loading test.dll is not allowed!", L"Hooked LoadLibrary", MB_OK | MB_ICONERROR);
			hFile = NULL;
			return NULL; // ����NULL��ʾ����ʧ��
		}

		// ����ԭʼLoadLibrary����
		return originalLoadLibrary(lpLibFileName, hFile, dwFlags);
	}

	void HookLoadlibrary()
	{
		//���������û�����þ��˳�
		if (exeName.compare(L"") == 0)
		{
			return;
		}

		// DetourTransactionBegin/Commit/Abort���������Եؽ��йҹ�
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		// ��ȡLoadLibrary��ַ
		originalLoadLibrary = (LoadLibraryFunc)GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryExW");

		// �ҹ�LoadLibrary����
		DetourAttach(&(PVOID&)originalLoadLibrary, MyLoadLibrary);

		// �ύ�ҹ�����
		DetourTransactionCommit();
	}
}
