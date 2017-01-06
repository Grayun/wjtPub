#pragma once
#include <iostream>

class CMStringImp;
class CMstring
{
public:
	explicit CMstring(void);
	~CMstring(void);
	CMstring(LPCTSTR lpszstr);
	CMstring(const CMstring& lpszstr);
	CMstring& operator = (const CMstring& lpszstr);
	operator LPCTSTR() const;
	bool operator == (const CMstring&) const;
	bool operator != (const CMstring&) const;
	bool operator < (const CMstring&) const;
	TCHAR operator[] (int nIndex) const;
	TCHAR& operator[] (int nIndex);
	CMstring& operator += (LPCTSTR pStr);
	CMstring& operator += (TCHAR ch);
	friend CMstring operator+(const CMstring& str1, const CMstring& str2);
	friend CMstring operator+(const CMstring& str1, LPCTSTR lpszstr);
	friend CMstring operator+(const CMstring& str1, TCHAR ch);
	friend CMstring operator+(TCHAR ch, const CMstring& str1);
	friend CMstring operator+(LPCTSTR lpszstr, const CMstring& str1);
	// friend wostream operator <<(wostream &is,const CMstring &str);

public:
	LPCTSTR GetData() const;
	bool IsEmpty() const;
	TCHAR GetAt(int) const;
	TCHAR& GetAt(int);
	int GetLength() const;
	int Compare(const CMstring&) const;
	int CompareNoCase(const CMstring&) const;
	int Find(TCHAR ch, int nStart = 0) const;
	int Find(LPCTSTR pStr, int nStart = 0) const;
	int ReverseFind(TCHAR ch) const;
	int ReverseFind(LPCTSTR pStr) const;
	CMstring Right(int nCount) const;
	CMstring Left(int nCount ) const;
public:
	CMstring& MakeLower();
	CMstring& MakeUpper();
	CMstring& MakeReverse();
	int Replace(TCHAR chOld, TCHAR chNew);
	int Replace(LPCTSTR pszOld, LPCTSTR pszNew);
	int Insert(int iIndex, TCHAR ch);
	void Format(LPCTSTR lpszFormat, ...);
private:
	CMStringImp* m_pImp;
};