#pragma once

//#include <Wimgapi.h>

#define DefHandle(ClassName,type,hInvalidHandle,TCloseHandle)\
class ClassName\
{\
	public:\
	type hHandle;\
	ClassName(type _hHandle = hInvalidHandle):hHandle(_hHandle){}\
	~ClassName(){ Close(); }\
	type* operator &(){	return &hHandle;  } \
	type operator=(type hHandle){ Close(); return this->hHandle = hHandle; }\
	operator type() {return hHandle;}\
	BOOL IsInvalid(){return hHandle == hInvalidHandle;	}\
	type Detach(){ type temp=hHandle; hHandle=hInvalidHandle;return temp; }\
	HRESULT Close(){ if(IsInvalid()) return ERROR_INVALID_HANDLE; HRESULT hr=##TCloseHandle; if(!hr) hHandle=hInvalidHandle; return hr;	}\
};

//�رվ����ʽ1��ֱ�ӷ��� HRESULT
#define TCloseHandle1(_TCloseHandle) _TCloseHandle(hHandle)

//�رվ����ʽ2����������BOOLֵ����ͨ��GetLastError���ش������
#define TCloseHandle2(_TCloseHandle) _TCloseHandle(hHandle) ? S_OK : GetLastError()


//���ڹر�RegOpenKey�򿪵ľ��
DefHandle(CHKEY, HKEY, NULL, TCloseHandle1(RegCloseKey))

//���ڹر�CreateFile�򿪵ľ��
DefHandle(CHFile, HANDLE, INVALID_HANDLE_VALUE, TCloseHandle2(CloseHandle))

//���ڹر�FindFirstFile�򿪵ľ��
DefHandle(CHFileFind, HANDLE, INVALID_HANDLE_VALUE, TCloseHandle2(FindClose))

//�Զ��ر�LoadLibrary�ľ��
DefHandle(CHModule, HMODULE, NULL, TCloseHandle2(FreeLibrary))

//�Զ��رջ������ľ��
DefHandle(CHMutex, HANDLE, NULL, TCloseHandle2(CloseHandle))


typedef CHMutex CHEvent;

typedef CHMutex CHProcess;

#ifdef _WIMGAPI_H_
DefHandle(CHWim, HANDLE, NULL, TCloseHandle2(WIMCloseHandle))
#endif