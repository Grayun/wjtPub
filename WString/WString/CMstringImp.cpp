#include "StdAfx.h"
#include "CMstringImp.h"
#include <iostream>
#include <fstream>
#include <stdarg.h>

CMStringImp::CMStringImp(void)
{
	m_vecData.resize(1, '\0');
}


CMStringImp::CMStringImp(LPCTSTR lpszstr)
{
	m_vecData.resize(1, '\0');
	Init(lpszstr);
}

CMStringImp::~CMStringImp(void)
{
}


CMStringImp& CMStringImp::operator += (LPCTSTR pStr)
{
	int nLen = GetLength();
	this->Insert(nLen, pStr);
	return *this;
}

CMStringImp& CMStringImp::operator += (TCHAR ch)
{
	int nLen = GetLength();
	this->Insert(nLen, ch);

	return *this;
}

bool CMStringImp::operator == (const CMStringImp& str) const
{
	return 0 == this->Compare(str);
}

bool CMStringImp::operator != (const CMStringImp& str) const
{
	if (*this == str)
	{
		return false;
	}

	return true;
}

CMStringImp::operator LPCTSTR() const
{
	return GetBuffer();
}

int CMStringImp::GetLength() const
{
	return _tcslen(GetBuffer());
}


int CMStringImp::Compare(LPCTSTR lpszstr) const
{
	return _tcscmp(*this, lpszstr);
}

int CMStringImp::CompareNoCase(LPCTSTR lpszstr) const
{
	return _tcsicmp(*this, lpszstr);
}

TCHAR CMStringImp::GetAt(int nIndex) const
{
	return m_vecData[nIndex];
}

TCHAR& CMStringImp::GetAt(int  nIndex)
{
	return m_vecData[nIndex];
}

LPCTSTR CMStringImp::GetBuffer() const
{
	return (TCHAR*)&m_vecData[0];
}

LPCTSTR CMStringImp::GetData(int nStart) const
{
	if (nStart < 0 || nStart > GetLength() - 1)
	{
		return _T("");
	}

	return (TCHAR*)&m_vecData[nStart];
}

LPCTSTR CMStringImp::GetBuffer(int nLen)
{
	m_vecData = std::vector<TCHAR>(nLen + 1, '\0');

	return GetBuffer();
}

void CMStringImp::Init(LPCTSTR lpszstr)
{
	if (lpszstr == GetBuffer())
	{
		return;
	}

	int nLen = 0;
	if (lpszstr != NULL)
	{
		nLen = _tcslen(lpszstr);
	}
	m_vecData = std::vector<TCHAR>(nLen + 1, '\0');
	int i = 0;
	for (;i < nLen;++i)
	{
		m_vecData[i] = lpszstr[i];
	}

	
}


int CMStringImp::Find(TCHAR ch, int nStart) const
{
	int nState = -1;
	for (int i = nStart;i < GetLength();++i)
	{
		TCHAR chTemp = GetAt(i);
		if (ch == chTemp)
		{
			nState = i;
			break;
		}
	}

	return nState;
}

int CMStringImp::Find(LPCTSTR pStr, int nStart) const
{
	if (nStart < 0)
	{
		nStart = 0;
	}

	CMStringImp imp(pStr);
	int nState = -1;

	int nLen = imp.GetLength();

	if (nLen == 0 || nLen > GetLength() - nStart)
	{
		return nState;
	}

	CMStringImp str;
	str.GetBuffer(nLen);

	int nCount = 0;
	for (int i = nStart;i < GetLength();++i)
	{
		TCHAR chTemp = GetAt(i);
		TCHAR& ch = str.GetAt(nCount);
		ch = chTemp;
		nCount++;

		if (nCount == nLen)
		{
			if (str == imp)
			{
				nState = i - nLen + 1;
				break;
			}
			else
			{
				for (int j = 0;j < nLen - 1;++j)
				{
					TCHAR& ch1 = str.GetAt(j);
					TCHAR& ch2 = str.GetAt(j + 1);
					ch1 = ch2;
				}
				nCount--;
			}
		}

	}

	return nState;

}
int CMStringImp::ReverseFind(TCHAR ch) const
{
	int nState = -1;
	for (int i = GetLength() - 1;i >= 0;--i)
	{
		TCHAR chTemp = GetAt(i);
		if (ch == chTemp)
		{
			nState = i;
			break;
		}
	}
	return nState;

}
int CMStringImp::ReverseFind(LPCTSTR pStr) const
{
	CMStringImp imp(pStr);
	int nState = -1;

	int nLen = imp.GetLength();

	if (nLen == 0 || nLen > GetLength())
	{
		return nState;
	}

	CMStringImp str;
	str.GetBuffer(nLen);

	int nCount = nLen - 1;
	for (int i = GetLength() - 1;i >= 0;--i)
	{
		TCHAR chTemp = GetAt(i);
		TCHAR& ch = str.GetAt(nCount);
		ch = chTemp;
		--nCount;

		if (nCount == -1)
		{
			if (str == imp)
			{
				nState = i;
				break;
			}
			else
			{
				for (int j = nLen - 1;j > 0;--j)
				{
					TCHAR& ch1 = str.GetAt(j);
					TCHAR& ch2 = str.GetAt(j - 1);
					ch1 = ch2;
				}
				++nCount;
			}
		}

	}

	return nState;

}
//
CMStringImp CMStringImp::Right(int nCount) const
{
	CMStringImp imp;
	if (nCount <= 0 || nCount > GetLength())
	{
		return imp;
	}

	imp.GetBuffer(nCount);
	for (int i = nCount - 1;i >=0;--i)
	{
		TCHAR& ch = imp.GetAt(i);
		ch = GetAt(i);
	}

	return imp;
}

CMStringImp CMStringImp::Left(int nCount) const
{
	CMStringImp imp;
	if (nCount <= 0 || nCount > GetLength())
	{
		return imp;
	}

	imp.GetBuffer(nCount);
	for (int i = 0;i < nCount;++i)
	{
		TCHAR& ch = imp.GetAt(i);
		ch = GetAt(i);
	}

	return imp;
}
//
//CMStringImp& CMStringImp::MakeLower()
//{
//
//}
//
//CMStringImp& CMStringImp::MakeUpper()
//{
//
//}
//
CMStringImp& CMStringImp::MakeReverse()
{
	int nLen = GetLength();

	CMStringImp imp(*this);

	int nCount = nLen - 1;
	for (int i = 0;i < nLen;++i)
	{
		TCHAR& ch = GetAt(nCount);
		ch = imp.GetAt(i);
		--nCount;
	}

	return *this;
}

CMStringImp CMStringImp::Reverse() const
{
	CMStringImp imp(*this);

	return imp.MakeReverse();
}
//
//int CMStringImp::Replace(TCHAR chOld, TCHAR chNew)
//{
//
//}
//
//int CMStringImp::Replace(LPCTSTR pszOld, LPCTSTR pszNew)
//{
//
//}
//
int CMStringImp::Insert(int iIndex, TCHAR ch)
{
	int nState = -1;

	if (iIndex < 0)
	{
		return nState;
	}

	m_vecData.insert(m_vecData.begin() + iIndex, ch);
	nState = 0;

	return nState;
}


int CMStringImp::Insert(int iIndex, LPCTSTR pStr)
{
	int nState = -1;
	if (iIndex < 0)
	{
		return nState;
	}
	CMStringImp imp(pStr);

	TCHAR ch = '\0';
	m_vecData.insert(m_vecData.begin() + iIndex, imp.GetLength(), ch);

	int nPos = 0;
	for (int i = iIndex;nPos < imp.GetLength();++i)
	{
		m_vecData[i] = imp.GetAt(nPos);
		nPos++;
	}
	nState = 0;

	return nState;
}