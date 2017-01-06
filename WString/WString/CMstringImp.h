#pragma once
#include <iostream>
#include <vector>
#include <tchar.h>

class CMStringImp
{
public:
	CMStringImp(void);
	CMStringImp(LPCTSTR lpszstr);
	~CMStringImp(void);

	operator LPCTSTR() const;
	CMStringImp& operator += (LPCTSTR pStr);
	CMStringImp& operator += (TCHAR ch);
	bool operator == (const CMStringImp&) const;
	bool operator != (const CMStringImp&) const;
public:
	int GetLength() const;
	TCHAR GetAt(int) const;
	TCHAR& GetAt(int);
	LPCTSTR GetBuffer(int nLen);
	int Compare(LPCTSTR lpszstr) const;
	int CompareNoCase(LPCTSTR lpszstr) const;

	int Find(TCHAR ch, int nStart = 0) const;
	int Find(LPCTSTR pStr, int nStart = 0) const;
	int ReverseFind(TCHAR ch) const;
	int ReverseFind(LPCTSTR pStr) const;
	CMStringImp Right(int nCount) const;
	CMStringImp Left(int nCount ) const;
public:
	CMStringImp& MakeLower();
	CMStringImp& MakeUpper();
	CMStringImp& MakeReverse();
	CMStringImp Reverse() const;
	int Replace(TCHAR chOld, TCHAR chNew);
	int Replace(LPCTSTR pszOld, LPCTSTR pszNew);
	int Insert(int iIndex, TCHAR ch);
	int Insert(int iIndex, LPCTSTR pStr);
public:
	LPCTSTR GetBuffer() const;
	LPCTSTR GetData(int nStart = 0) const;
	void Init(LPCTSTR lpszstr);

private:
	std::vector<TCHAR> m_vecData;
};
