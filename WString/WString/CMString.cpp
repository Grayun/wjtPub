#include "stdafx.h"
#include "CMString.h"
#include "CMstringImp.h"

CMStringImp* NewImp()
{
	return new CMStringImp;
}

CMstring::CMstring(void)
{
	m_pImp = NewImp();
	m_pImp->Init(NULL);
}

CMstring::~CMstring(void)
{
	if (NULL != m_pImp)
	{
		delete m_pImp;
		m_pImp = NULL;
	}
}

CMstring::CMstring(LPCTSTR lpszstr)
{
	m_pImp = NewImp();
	m_pImp->Init(lpszstr);
}

CMstring::CMstring(const CMstring& lpszstr)
{
	m_pImp = NewImp();
	m_pImp->Init(lpszstr);
}

CMstring& CMstring::operator = (const CMstring& lpszstr)
{
	return *this;
}

bool CMstring::operator == (const CMstring& str) const
{
	return 0 == this->Compare(str);
}

bool CMstring::operator != (const CMstring& str) const
{
	if (*this == str)
	{
		return false;
	}
	return true;
}

bool CMstring::operator < (const CMstring& str) const
{
	return Compare(str) < 0;
}


TCHAR CMstring::operator[] (int nIndex) const
{
	return GetAt(nIndex);
}

TCHAR& CMstring::operator[] (int nIndex)
{
	return GetAt(nIndex);
}

CMstring& CMstring::operator += (LPCTSTR pStr)
{
	(*this->m_pImp) += pStr;

	return *this;
}

CMstring& CMstring::operator += (TCHAR ch)
{
	(*this->m_pImp) += ch;
	return *this;
}


CMstring::operator LPCTSTR() const
{
	return *m_pImp;
}

CMstring operator+(const CMstring& str1, const CMstring& str2)
{
	CMstring str;
	str += str1;
	str += str2;

	return str;
}

CMstring operator+(const CMstring& str1, LPCTSTR lpszstr)
{
	CMstring str;
	str += str1;
	str += lpszstr;
	return str;	
}

CMstring operator+(const CMstring& str1, TCHAR ch)
{
	CMstring str;
	str += str1;
	str += ch;
	return str;	
}


CMstring operator+(TCHAR ch, const CMstring& str1)
{
	CMstring str;
	str += ch;
	str += str;
	return str;	

}

CMstring operator+(LPCTSTR lpszstr, const CMstring& str1)
{
	CMstring str;
	str += lpszstr;
	str += str1;
	return str;	
}

// wostream &operator <<(wostream &os,const CMstring &str)
// {
// 	os << str.m_pImp->GetBuffer();
// 	return os;
// }

#define MAX_FORMAT_BUFFER 1024
void CMstring::Format(LPCTSTR lpszFormat, ...)
{
	while (1)
	{
		TCHAR		*pBuffer = NULL;
		va_list      arg_ptr;
		pBuffer = (TCHAR*)m_pImp->GetBuffer(MAX_FORMAT_BUFFER);
		if (pBuffer == NULL)
		{
			m_pImp->Init(NULL);
		}

		va_start(arg_ptr,lpszFormat);
		_vsntprintf_s(pBuffer,MAX_FORMAT_BUFFER + 1,MAX_FORMAT_BUFFER,lpszFormat,arg_ptr);
		if (pBuffer[MAX_FORMAT_BUFFER-1] != '\0')
		{
			FILE *fpTempFile = NULL;
			tmpfile_s(&fpTempFile);
			if (fpTempFile == NULL)
			{
				break;
			}
			_vftprintf(fpTempFile,lpszFormat,arg_ptr);
			fseek(fpTempFile,0,SEEK_END);
			int iFileLength = ftell(fpTempFile);
			rewind(fpTempFile);

			pBuffer = (TCHAR*)m_pImp->GetBuffer(iFileLength + 1);;
			if (pBuffer == NULL)
			{
				m_pImp->Init(NULL);
			}
			fread(pBuffer,iFileLength,1,fpTempFile);
			pBuffer[iFileLength] = '\0';

			fclose(fpTempFile);
		}
		va_end(arg_ptr);
		m_pImp->Init(pBuffer);
		break;
	}
}

LPCTSTR CMstring::GetData() const
{
	return m_pImp->GetBuffer();
}

bool CMstring::IsEmpty() const
{
	return 0 == GetLength();
}

TCHAR CMstring::GetAt(int nIndex) const
{
	return m_pImp->GetAt(nIndex);
}

TCHAR& CMstring::GetAt(int nIndex)
{
	return m_pImp->GetAt(nIndex);
}

int CMstring::GetLength() const
{
	return m_pImp->GetLength();
}

int CMstring::Compare(const CMstring& str) const
{
	return m_pImp->Compare(str);
}

int CMstring::CompareNoCase(const CMstring& str) const
{
	return m_pImp->CompareNoCase(str);
}

int CMstring::Find(TCHAR ch, int nStart) const
{
	return m_pImp->Find(ch, nStart);
}

int CMstring::Find(LPCTSTR pStr, int nStart) const
{
	return m_pImp->Find(pStr, nStart);
}

int CMstring::ReverseFind(TCHAR ch) const
{
	return m_pImp->ReverseFind(ch);
}

int CMstring::ReverseFind(LPCTSTR pStr) const
{
	return m_pImp->ReverseFind(pStr);
}

CMstring CMstring::Right(int nCount) const
{
	return CMstring(m_pImp->Right(nCount));
}

CMstring CMstring::Left(int nCount ) const
{
	return CMstring(m_pImp->Left(nCount));
}