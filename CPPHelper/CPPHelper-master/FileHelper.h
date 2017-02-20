#pragma once
#include <Windows.h>
#include <atlstr.h>
#include "km.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
//
//                       �ṩ�ļ���������
//
/////////////////////////////////////////////////////////////////////////////

//UTF16�ļ�ͷ
const BYTE BomUTF16[] = { 0xff,0xfe };

//UTF8�ļ�ͷ
const BYTE BomUTF8[] = { 0xEF ,0xBB ,0xBF };



//�ж����Ƿ���.�Լ�..
bool __fastcall IsDots(LPCWSTR FileName, DWORD cbFileName);
bool __fastcall IsDots(LPCWSTR FileName);
bool __fastcall IsDots(LPCSTR FileName);

#define _IsDots IsDots


NTSTATUS __fastcall GetFileId(LPCWSTR FilePath, FILE_STANDARD_INFORMATION* pFileStandardInfo, FILE_INTERNAL_INFORMATION* pFileInternalInfo = NULL);

NTSTATUS __fastcall NtGetFileId(POBJECT_ATTRIBUTES ObjectAttributes, FILE_STANDARD_INFORMATION* pFileStandardInfo, FILE_INTERNAL_INFORMATION* pFileInternalInfo);

/*����Ȩ��ɾ���ļ������ļ���
DeleteRootPathָʾ�Ƿ�ɾ����Ŀ¼����PathΪ�ļ�ʱ�˲�����Ч*/
NTSTATUS DeleteDirectory(LPCWSTR Path, BOOL DeleteRootPath = TRUE);

HRESULT UpdateFile(CString lpExistingFileName, CString lpNewFileName);

NTSTATUS CopyDirectory(LPCWSTR ExistingDirectoryPath, LPCWSTR NewDirectoryInfoPath);

//DWORD MoveFileEx2(LPCTSTR lpExistingFileName, LPCWSTR lpNewFileName, DWORD dwFlags = MOVEFILE_REPLACE_EXISTING);

//����Ȩ��ɾ���ļ����ߣ��գ��ļ���
NTSTATUS DeleteFile2(LPCWSTR FilePath);
NTSTATUS NtDeleteFile2(POBJECT_ATTRIBUTES ObjectAttributes);

DWORD GetFileAttributes2(LPCTSTR FilePath);
NTSTATUS NtSetFileAttributes(POBJECT_ATTRIBUTES ObjectAttributes, DWORD FileAttributes);

NTSTATUS SetFileAttributes2(LPCTSTR FilePath, DWORD FileAttributes);

UINT64 GetFileAllocationSize(LPCWSTR FilePath);

UINT64 GetDirectorySize(LPCWSTR FilePath);

//����NTFSѹ��һ���ļ�/�ļ���
HRESULT CompressFile(LPCWSTR FilePath);

ULONG64 GetDriverFreeSize(LPCWSTR Latter);

ULONG64 GetDriverUseSize(LPCWSTR Latter);


BOOL CheckUpdateFile(CString Src, CString Des, std::map<UINT64, DWORD>&FileMap, UINT64* pSize);

HRESULT GetFileVersion(HMODULE hFileMoudle, UINT16 Version[4]);

HRESULT GetFileVersion(LPCWSTR FilePath, UINT16 Version[4]);

UINT64 GetDirectoryAllocationSize(LPCWSTR FilePath, std::map<UINT64, DWORD>& FileMap);

HRESULT CrateDirectorHandLink(CString To, CString From);

HRESULT MoveDirectory(CString ExistingDirectoryPath, CString NewDirectoryInfoPath);


enum FilePathType
{
	PathIsUnkow = -1,
	PathNotExist,
	PathIsFile,
	PathIsDir,
};

FilePathType GetFileType(LPCWSTR FilePath);

FilePathType GetFileType(LPCSTR FilePath);