#include "stdafx.h"
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperEdit

CSuperEdit::CSuperEdit()
{
	//brushes
	m_brInvalid.CreateSolidBrush(	RGBDEF_INVALID);
	m_brIncomplete.CreateSolidBrush(RGBDEF_INCOMPLETE);
	m_brEmpty.CreateSolidBrush(		RGBDEF_EMPTY);
	m_brValid.CreateSolidBrush(		RGBDEF_VALID);
	m_brCustom1.CreateSolidBrush(	RGBDEF_CUST1);
	m_brCustom2.CreateSolidBrush(	RGBDEF_CUST2);
	//boolean options
	m_bCustomValidation = FALSE;
	m_bNoValidation = FALSE;
	m_bEableLengthCheck = FALSE;
	m_bAllowEmpty = TRUE;
	m_bAllowAllCharEntry = FALSE;
	m_bCustomValidationCallOK = FALSE;
	m_bCustomErrorIsValid[1]  = FALSE;
	m_bCustomErrorIsValid[2]  = FALSE;
	m_bEnableRangeCheck = TRUE;
	//boolean states
	m_bValid = FALSE;
	m_bValidLength = FALSE;
	m_bValidSyntax = FALSE;
	m_bValidValue = TRUE;
	//default charset
	m_setCurrent = CSuperEdit::SET_ALLCHAR;

	//max min default\	CString m_szDefValue;
	m_bEableValueCheck = FALSE;
	m_dDefValue = 0;
	m_dMinValue = 0;
	m_dMaxValue = 0;
	m_dPrec = 0.0001;

	//TOOL TIP
	m_bEableTip = FALSE;
	m_szTip = _T("");
	m_bSetTipTextByUser = FALSE;
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	}

	//initialising others
	SetValidCharSet(m_setCurrent);
	m_iCurrentErr = BR_EMPTY;
	m_iMaxInputLen = VVE_DEF_MAXLEN;
	m_iMinInputLen = VVE_DEF_MINLEN;

}

CSuperEdit::~CSuperEdit()
{
	m_brInvalid.DeleteObject();
	m_brIncomplete.DeleteObject();
	m_brEmpty.DeleteObject();
	m_brValid.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSuperEdit, CEdit)
	//{{AFX_MSG_MAP(CSuperEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CSuperEdit message handlers

BOOL CSuperEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case ID_UNDO:
		Undo();
		break;
	case ID_CUT:
		Cut();
		break;
	case ID_COPY:
		Copy();
		break;
	case ID_PASTE:
		Paste();
		break;
	case ID_DELETE:
		Clear();
		break;
	case ID_SEL_ALL:
		SelectAll();
		break;
	}
	return CEdit::OnCommand(wParam, lParam);
}


BOOL CSuperEdit::SetBrushColour(VVEbrush brWhich, COLORREF NewColour)
{
	switch(brWhich)
	{
	case BR_ERROR:
	case BR_EMPTY:
		m_brEmpty.DeleteObject();
		m_brEmpty.CreateSolidBrush(NewColour);
		break;
	case BR_VALID:
		m_brValid.DeleteObject();
		m_brValid.CreateSolidBrush(NewColour);
		break;
	case BR_INVALIDSYNTAX:
	case BR_LITTE:
	case BR_BIG:
		m_brInvalid.DeleteObject();
		m_brInvalid.CreateSolidBrush(NewColour);
		break;
	case BR_INCOMPLETE:
	case BR_RANGEERR:
	case BR_LONG:
	case BR_SHORT:
		m_brIncomplete.DeleteObject();
		m_brIncomplete.CreateSolidBrush(NewColour);
		break;
	case BR_CUSTOM_ERR1:
		m_brCustom1.DeleteObject();
		m_brCustom1.CreateSolidBrush(NewColour);
		break;
	case BR_CUSTOM_ERR2:
		m_brCustom2.DeleteObject();
		m_brCustom2.CreateSolidBrush(NewColour);
		break;
	}
	return FALSE;

}

COLORREF CSuperEdit::GetBrushColour(VVEbrush brWhich)
{
	LOGBRUSH  Logbr;
	switch(brWhich)
	{
	case BR_ERROR:
	case BR_EMPTY:
		if( m_brEmpty.GetLogBrush(&Logbr))
			return Logbr.lbColor;
		else
			return RGBDEF_EMPTY;
	case BR_VALID:
		if( m_brValid.GetLogBrush(&Logbr))
			return Logbr.lbColor;
		else
			return RGBDEF_VALID;
	case BR_INVALIDSYNTAX:
	case BR_LITTE:
	case BR_BIG:
		if( m_brInvalid.GetLogBrush(&Logbr))
			return Logbr.lbColor;
		else
			return RGBDEF_INVALID;
	case BR_INCOMPLETE:
	case BR_LONG:
	case BR_SHORT:
	case BR_RANGEERR:
		if( m_brIncomplete.GetLogBrush(&Logbr))
			return Logbr.lbColor;
		else
			return RGBDEF_INCOMPLETE;
	case BR_CUSTOM_ERR1:
		if( m_brCustom1.GetLogBrush(&Logbr))
			return Logbr.lbColor;
		else
			return RGBDEF_CUST1;
	case BR_CUSTOM_ERR2:
		if( m_brCustom1.GetLogBrush(&Logbr))
			return Logbr.lbColor;
		else
			return RGBDEF_CUST2;
	}
	return ::GetSysColor(COLOR_WINDOW);
}



CBrush * CSuperEdit::CustomValidation()
{
	CBrush * brush = &m_brEmpty;
	CString s;
	CEdit::GetWindowText(s);
	DWORD_PTR dwResult = BR_ERROR;
	CWnd * pParent = GetParent();
	NMHDR nm;
	nm.idFrom = GetDlgCtrlID();
	nm.hwndFrom = m_hWnd;
	nm.code = m_iCurrentErr;
	if(::SendMessageTimeout(pParent->GetSafeHwnd(),UWM_VVE_DO_CUSTOMVALIDATION,
				(WPARAM)&nm,
				(LPARAM)(LPCTSTR)s,SMTO_BLOCK,1,&dwResult) == 0) //if == 0 then timeout or failure occured
	{
		m_iCurrentErr = BR_ERROR;
		m_bCustomValidationCallOK = FALSE;
		return &m_brEmpty;
	}
	m_bCustomValidationCallOK = TRUE;
	m_iCurrentErr = (VVEbrush)dwResult;
	switch(m_iCurrentErr)
	{
	case BR_ERROR:
		//SendMessageTimeout can still succeed if WM message is not processed. in this case
		//the return value dwResult will be 0 (BR_ERROR).
		m_bCustomValidationCallOK = FALSE;
		m_iCurrentErr = BR_ERROR;
		return &m_brEmpty;
	case BR_EMPTY:
		return &m_brEmpty;
	case BR_VALID:
		return &m_brValid;
	case BR_INCOMPLETE:
	case BR_SHORT:
	case BR_LONG:
	case BR_RANGEERR:
		return &m_brIncomplete;//long/short/large/small/incomplete are same brush - only context messages change
	case BR_INVALIDSYNTAX:
		return &m_brInvalid;
	case BR_CUSTOM_ERR1:
		return &m_brCustom1;
	case BR_CUSTOM_ERR2:
		return &m_brCustom2;
	default:
		m_iCurrentErr = BR_ERROR;
		return &m_brEmpty;
	}


	return brush;
}

//CheckFloat routine compliments of joseph - contact info at top of this file
CBrush * CSuperEdit::CheckFloat()
{
	CBrush * brush = &m_brEmpty;
	enum {S0, IPART, FPART, ESIGN, EPART};
	WORD state = S0;
	CString s;
	CEdit::GetWindowText(s);
	int iCheckLen = VVE_OK;
	int nCheckValue = VVE_OK;
	BOOL bNewValidOK;
	BOOL bNewValidLengthOK;
	BOOL bNewValidSyntaxOK;

	if(m_bCustomValidation)
	{
		brush = CustomValidation();
	}
	else
	{
		for(int i = 0; brush != &m_brInvalid && i < s.GetLength();)
		{ // scan string /
			TCHAR ch = s[i];
			switch(MAKELONG(state, ch))
			{
			case MAKELONG(S0, _T(' ')):
			case MAKELONG(S0, _T('\t')):
				i++;
				continue;
			case MAKELONG(S0, _T('+')):
			case MAKELONG(S0, _T('-')):
				i++;
				m_iCurrentErr = BR_INCOMPLETE;
				brush = &m_brIncomplete;
				state = IPART;
				continue;
			case MAKELONG(S0, _T('0')):
			case MAKELONG(S0, _T('1')):
			case MAKELONG(S0, _T('2')):
			case MAKELONG(S0, _T('3')):
			case MAKELONG(S0, _T('4')):
			case MAKELONG(S0, _T('5')):
			case MAKELONG(S0, _T('6')):
			case MAKELONG(S0, _T('7')):
			case MAKELONG(S0, _T('8')):
			case MAKELONG(S0, _T('9')):
				state = IPART;
				continue;
			case MAKELONG(S0, _T('.')):
				i++;
				state = FPART;
				m_iCurrentErr = BR_INCOMPLETE;
				brush = &m_brIncomplete;
				continue;
			case MAKELONG(S0, _T('E')):
			case MAKELONG(S0, _T('e')):
				i++;
				state = ESIGN;
				m_iCurrentErr = BR_INCOMPLETE;
				brush = &m_brIncomplete;
				continue;
			case MAKELONG(IPART, _T('0')):
			case MAKELONG(IPART, _T('1')):
			case MAKELONG(IPART, _T('2')):
			case MAKELONG(IPART, _T('3')):
			case MAKELONG(IPART, _T('4')):
			case MAKELONG(IPART, _T('5')):
			case MAKELONG(IPART, _T('6')):
			case MAKELONG(IPART, _T('7')):
			case MAKELONG(IPART, _T('8')):
			case MAKELONG(IPART, _T('9')):
				i++;
				m_iCurrentErr = BR_VALID;
				brush = &m_brValid;
				continue;
			case MAKELONG(IPART, _T('.')):
				i++;
				brush = &m_brValid;
				state = FPART;
				continue;
			case MAKELONG(IPART, _T('e')):
			case MAKELONG(IPART, _T('E')):
				i++;
				m_iCurrentErr = BR_INCOMPLETE;
				brush = &m_brIncomplete;
				state = ESIGN;
				continue;
			case MAKELONG(FPART, _T('0')):
			case MAKELONG(FPART, _T('1')):
			case MAKELONG(FPART, _T('2')):
			case MAKELONG(FPART, _T('3')):
			case MAKELONG(FPART, _T('4')):
			case MAKELONG(FPART, _T('5')):
			case MAKELONG(FPART, _T('6')):
			case MAKELONG(FPART, _T('7')):
			case MAKELONG(FPART, _T('8')):
			case MAKELONG(FPART, _T('9')):
				i++;
				m_iCurrentErr = BR_VALID;
				brush = &m_brValid;
				continue;
			case MAKELONG(FPART, _T('e')):
			case MAKELONG(FPART, _T('E')):
				i++;
				m_iCurrentErr = BR_INCOMPLETE;
				brush = &m_brIncomplete;
				state = ESIGN;
				continue;
			case MAKELONG(ESIGN, _T('+')):
			case MAKELONG(ESIGN, _T('-')):
				i++;
				m_iCurrentErr = BR_INCOMPLETE;
				brush = &m_brIncomplete;
				state = EPART;
				continue;
			case MAKELONG(ESIGN, _T('0')):
			case MAKELONG(ESIGN, _T('1')):
			case MAKELONG(ESIGN, _T('2')):
			case MAKELONG(ESIGN, _T('3')):
			case MAKELONG(ESIGN, _T('4')):
			case MAKELONG(ESIGN, _T('5')):
			case MAKELONG(ESIGN, _T('6')):
			case MAKELONG(ESIGN, _T('7')):
			case MAKELONG(ESIGN, _T('8')):
			case MAKELONG(ESIGN, _T('9')):
				state = EPART;
				continue;
			case MAKELONG(EPART, _T('0')):
			case MAKELONG(EPART, _T('1')):
			case MAKELONG(EPART, _T('2')):
			case MAKELONG(EPART, _T('3')):
			case MAKELONG(EPART, _T('4')):
			case MAKELONG(EPART, _T('5')):
			case MAKELONG(EPART, _T('6')):
			case MAKELONG(EPART, _T('7')):
			case MAKELONG(EPART, _T('8')):
			case MAKELONG(EPART, _T('9')):
				i++;
				m_iCurrentErr = BR_VALID;
				brush = &m_brValid;
				continue;
			default:
				m_iCurrentErr = BR_INVALIDSYNTAX;
				brush = &m_brInvalid;
				break;
			} // switch
		} // scan string /
	}

	//check contents/////////////////////////////////////////////////////

	//check range
	if(brush == &m_brValid && m_bEnableRangeCheck)//is input valid?
	{
		if (CheckRange(s) != VVE_OK)
		{
			m_iCurrentErr = BR_RANGEERR;
			brush = &m_brIncomplete;
		}
	}

	iCheckLen = CheckLen(s);
	nCheckValue = CheckValueOutRange();
	bNewValidSyntaxOK = brush == &m_brValid;
	bNewValidLengthOK = iCheckLen == VVE_OK;
	//if user wants to allow empty input, then override the checks
	if(m_bAllowEmpty && s.GetLength() == 0)
	{
		bNewValidLengthOK = TRUE;
		bNewValidSyntaxOK = TRUE;
	}
	//set the brush according to check results
	//note though, brush may have been left on m_brIncomplete from tests above
	if(bNewValidSyntaxOK)
	{
		if(iCheckLen == VVE_OK )//all fine
		{
			m_iCurrentErr = BR_VALID;
			brush = &m_brValid;
		}
		else //length ng
		{
			m_iCurrentErr = iCheckLen == VVE_LEN_LONG ? BR_LONG : BR_SHORT;
			brush = &m_brIncomplete;
		}
	}
	//now (almost final check) - as you can correct any syntax but still
	//be too long / short, length checking has greater priority
	if(iCheckLen != VVE_OK && m_bEableLengthCheck)
	{
		m_iCurrentErr = iCheckLen == VVE_LEN_LONG ? BR_LONG : BR_SHORT;
		brush = &m_brIncomplete;
	}
	if( (m_iMinInputLen == 0 || m_bAllowEmpty) && s.GetLength() == 0)
	{
		m_iCurrentErr = BR_EMPTY;
		brush = &m_brEmpty;//only set empty brush if its allowed to go empty
	}

	if((m_iMinInputLen > 0 || !m_bAllowEmpty) && s.GetLength() == 0)
	{
		m_iCurrentErr = BR_ERROR;
		brush = &m_brInvalid;
		bNewValidLengthOK =  FALSE;
	}

	if(m_bEableValueCheck)
	{
		if(nCheckValue != VVE_OK)
		{
			m_iCurrentErr = nCheckValue == VVE_VAL_LITTE ? BR_LITTE : BR_BIG;
			brush = &m_brInvalid;
		}

	}

	//log state
	m_bValidSyntax = bNewValidSyntaxOK;
	m_bValidLength = bNewValidLengthOK;
	bNewValidOK = (m_bValidLength && m_bValidSyntax && m_bValidValue);
	//determine whether or not to post Validation changed msg
	if(m_bValid != bNewValidOK)//has validation state changed
	{
		//log valid state before posting change so that calls to is valid succeed
		m_bValid = bNewValidOK;
		ValidationChanged(bNewValidOK);
	}
	else
		m_bValid = bNewValidOK;//log valid state
	return brush;

}

CBrush * CSuperEdit::Validate()
{
	if(m_setCurrent == SET_FLOAT && !m_bCustomValidation)
		return CheckFloat();

	CBrush * brush = &m_brValid;
	BOOL change = FALSE;
	CString s;
	CEdit::GetWindowText(s);
	int iTextLength = s.GetLength();
	BOOL bChanged = FALSE;
	BOOL bNewValidOK = FALSE;
	BOOL bNewValidSyntaxOK = FALSE;
	BOOL bNewValidLengthOK = FALSE;
	int  iCheckLen = VVE_OK;
	int nCheckValue = VVE_OK;
	//check contents
	if(m_bCustomValidation && ! m_bNoValidation)
	{
		brush = CustomValidation();
	}
	else
	{
		if(m_bAllCharValid == FALSE )
		{
			for(int i = 0; i < iTextLength && brush != &m_brInvalid ; i ++)
			{
				if(m_bValidChar[(UCHAR)s.GetAt(i)] == FALSE)
				{
					m_iCurrentErr = BR_INVALIDSYNTAX;
					brush = &m_brInvalid;
				}
			}
			//additional decimal check
			if(m_setCurrent == SET_SDECIMAL)
			{
				CString stemp(s);
				stemp.TrimLeft();
				stemp.TrimRight();
				if(stemp.GetLength())
				{
					BOOL bSigned = stemp.FindOneOf(_T("+-")) == 0;
					stemp = stemp.Right(stemp.GetLength()-1);
					if( stemp.FindOneOf(_T("+- ")) >= 0)
					{
						m_iCurrentErr = BR_INVALIDSYNTAX;
						brush = &m_brInvalid;
					}
					if(bSigned && stemp.GetLength() < 1)
					{
						m_iCurrentErr = BR_INCOMPLETE;
						brush = &m_brIncomplete;
					}
				}
			}
		}
	}
	//bCustErrorValid ...
	//used to determine if the custom error returned from a
	//custom validation routine is considered valid or not
	BOOL bCustErrorValid = FALSE;

	if(m_bEnableRangeCheck && brush == &m_brValid)//should we check range?
	{
		if(CheckRange(s) != VVE_OK)// is range ok?
		{
			//it is not!
			m_iCurrentErr = BR_RANGEERR;
			brush = &m_brIncomplete;
		}
	}

	//check contents
	iCheckLen = CheckLen(s);
	nCheckValue = CheckValueOutRange();
	if(m_bCustomValidation && m_bCustomValidationCallOK)
	{
		//this is a custom validation returned brush so lets see if
		//user has declared this errorvalue as a validvalue
		if((brush == &m_brCustom1 && m_bCustomErrorIsValid[1])
			|| (brush == & m_brCustom2 && m_bCustomErrorIsValid[2]))
		{
			bNewValidSyntaxOK = TRUE;//ok
			bCustErrorValid = TRUE;//prevent any futher changes to brush for the rest of this function
		}
		else
			bNewValidSyntaxOK = brush == &m_brValid;//ok
	}
	else
	{
		//this is a built in validation - set the the ValidOK var accordingly
		bNewValidSyntaxOK = brush == &m_brValid;//ok
	}
	bNewValidLengthOK = (iCheckLen == VVE_OK);

	//if user wants to allow empty input, then override the checks
	if(m_bAllowEmpty && s.GetLength() == 0)
	{
		bNewValidLengthOK = TRUE;
		bNewValidSyntaxOK = TRUE;
	}
	//default to OK syntax if a custom validation call failed (necessary as brush will == EMPTY)
	if(m_bCustomValidation == TRUE && m_bCustomValidationCallOK == FALSE)
		bNewValidSyntaxOK = TRUE;

	//set the brush according to check results
	if(bNewValidSyntaxOK != FALSE && !bCustErrorValid) //is this valid syntax
	{
		if(iCheckLen == VVE_OK )//yes, all fine
		{
			m_iCurrentErr = BR_VALID;
			brush = &m_brValid;
		}
		else //length ng
		{
			m_iCurrentErr = iCheckLen == VVE_LEN_LONG ? BR_LONG : BR_SHORT;
			brush = &m_brIncomplete;
		}
	}
	//now (almost final check) - as you can correct any syntax but still
	//be too long / short, length checking has greater priority
	if(iCheckLen != VVE_OK && m_bEableLengthCheck)
	{
		m_iCurrentErr = iCheckLen == VVE_LEN_LONG ? BR_LONG : BR_SHORT;
		brush = &m_brIncomplete;
	}
	if((m_iMinInputLen == 0 || m_bAllowEmpty) && s.GetLength() == 0)//nothing in text box ?
	{
		m_iCurrentErr = BR_EMPTY;
		brush =  &m_brEmpty ;
	}

	if((m_iMinInputLen > 0 || !m_bAllowEmpty) && s.GetLength() == 0)
	{
		m_iCurrentErr = BR_ERROR;
		brush = &m_brInvalid;
		bNewValidLengthOK =  FALSE;
	}

	if(m_bEableValueCheck)
	{
		if(nCheckValue != VVE_OK)
		{
			m_iCurrentErr = nCheckValue == VVE_VAL_LITTE ? BR_LITTE : BR_BIG;
			brush = &m_brInvalid;
		}

	}

	//log states
	m_bValidSyntax = bNewValidSyntaxOK;
	m_bValidLength = bNewValidLengthOK;
	bNewValidOK = (m_bValidLength && m_bValidSyntax && m_bValidValue);
	//determine whether or not to post Validation changed msg
	if(m_bValid != bNewValidOK)//has validation state changed
	{
		//log valid state before posting change so that calls to is valid succeed
		m_bValid = bNewValidOK;
		ValidationChanged(bNewValidOK);
	}
	else
		m_bValid = bNewValidOK;//log valid state
	return brush;
}

void CSuperEdit::DoValidation(BOOL bRedraw )//explicit request for validation & or update
{

	if(m_hWnd && bRedraw)
	{
		//cause ctlColor to be called (which will call Validate())
		InvalidateRect(NULL);
		UpdateWindow();
	}
	else
		Validate();
}

//returns VVE_LEN_SHORT, VVE_LEN_LONG or VVE_OK
int CSuperEdit::CheckLen()
{
	if(m_bEableLengthCheck == FALSE || m_bNoValidation == TRUE)
		return VVE_OK;
	CString s;
	CEdit::GetWindowText(s);
	int iLen = s.GetLength();
	if(iLen < m_iMinInputLen)
			return VVE_LEN_SHORT;
	if(iLen > m_iMaxInputLen)
			return VVE_LEN_LONG;
	return VVE_OK;
}

//returns VVE_LEN_SHORT, VVE_LEN_LONG or VVE_OK
int CSuperEdit::CheckLen( LPCTSTR str)
{
	if(m_bEableLengthCheck == FALSE || m_bNoValidation == TRUE)
		return VVE_OK;
	int iLen = _tcslen(str);
	if(iLen < m_iMinInputLen)
			return VVE_LEN_SHORT;
	if(iLen > m_iMaxInputLen)
			return VVE_LEN_LONG;
	return VVE_OK;
}


HBRUSH CSuperEdit::CtlColor(CDC * dc, UINT id)
{
	if(m_bNoValidation || IsWindowEnabled() == FALSE ||(GetStyle() & ES_READONLY))
		return NULL;
	CBrush * brush = Validate();

	LOGBRUSH br;
	brush->GetLogBrush(&br);
	dc->SetBkColor(br.lbColor);
	return (HBRUSH)*brush;
}



/****************************************************************************
*                           CSuperEdit::OnChange
*
*   1. Invalidates the entire control thru a call to DoValidation so colors
*   come out right. Otherwise the "optimizations" of the redraw will leave
*	the colors banded in odd ways and only update the area around the text,
*	not the entire	box.
*	2. Post notification that a change occured lParam being the new state
****************************************************************************/

BOOL CSuperEdit::OnChange()
{
    DoValidation();//to Validate content & redraw window
    return FALSE;
}

/***************************************************************************
*       Allow / disallow character entry
*		user can set warious options which effect the entry of chars
*		see SetAllCharEntry()
****************************************************************************/

void CSuperEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	//if this is a character to process
	if( ! m_bNoValidation  &&  ! m_bAllCharValid && ! m_bAllowAllCharEntry )
		if (   ! (    (nChar < VVE_MIN_CHAR) || (nChar > VVE_MAX_CHAR ))   )
		{
			if(m_bValidChar[nChar] == FALSE) //is this a valid char?
			{
				//its not
				MessageBeep(0);
				return;
			}
		}
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CSuperEdit::SetTextLenMinMax(int iMin, int iMax)
{
	m_iMaxInputLen = iMax;
	m_iMinInputLen = iMin;
	if(iMax > VVE_DEF_MAXLEN || iMax < VVE_DEF_MINLEN)
		m_iMaxInputLen = VVE_DEF_MAXLEN;
	if( iMin < VVE_DEF_MINLEN || iMin > VVE_DEF_MAXLEN)
		m_iMinInputLen = VVE_DEF_MINLEN;
	if(m_iMinInputLen > m_iMaxInputLen)
	{
		int temp = m_iMaxInputLen;
		m_iMaxInputLen = m_iMinInputLen;
		m_iMinInputLen = temp;
	}
	SetLimitText(m_iMaxInputLen);
}
void CSuperEdit::SetValidChars(LPCTSTR cstrCustomChar)
{
	DWORD dwFillLen = sizeof(BOOL) * UCHAR_MAX;
	FillMemory(m_bValidChar,dwFillLen,FALSE);
	m_bAllCharValid = FALSE;
	for (UINT i = 0; i < _tcslen(cstrCustomChar); i++)
		m_bValidChar[(UCHAR)cstrCustomChar[i]] = TRUE;
}

void CSuperEdit::SetValidCharSet(VVEset vveset, LPCTSTR cstrCustomChar)
{
	//set all to FALSE
	DWORD dwFillLen = sizeof(BOOL) * UCHAR_MAX;
	FillMemory(m_bValidChar,dwFillLen,FALSE);
	m_bAllCharValid = FALSE;
	UINT i;
	CString strValid;
	switch(vveset)
	{
	case SET_CUSTOM: //user choice
		if(cstrCustomChar == NULL)
			return ;
		for (i = 0; i < _tcslen(cstrCustomChar); i++)
			m_bValidChar[(UCHAR)cstrCustomChar[i]] = TRUE;
		m_bEnableRangeCheck = FALSE;
		break;
	case SET_ALPHANUM:
		for ( i = 0; i < _tcslen(VVE_DEFSETALPHANUM); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETALPHANUM[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETALPHANUM[i]] = TRUE;
		}
		m_bEnableRangeCheck = FALSE;
		break;
	case SET_ALPHA:
		for ( i = 0; i < _tcslen(VVE_DEFSETALPHA); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETALPHA[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETALPHA[i]] = TRUE;
		}
		m_bEnableRangeCheck = FALSE;
		break;
	case SET_DECIMAL:
		for ( i = 0; i < _tcslen(VVE_DEFSETDECIMAL); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETDECIMAL[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETDECIMAL[i]] = TRUE;
		}
		m_bEnableRangeCheck = TRUE;
		break;
	case SET_SDECIMAL:
		for ( i = 0; i < _tcslen(VVE_DEFSETSDECIMAL); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETSDECIMAL[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETSDECIMAL[i]] = TRUE;
		}
		m_bEnableRangeCheck = TRUE;
		break;
	case SET_OCTAL:
		for ( i = 0; i < _tcslen(VVE_DEFSETOCTAL); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETOCTAL[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETOCTAL[i]] = TRUE;
		}
		m_bEnableRangeCheck = TRUE;
		break;
	case SET_HEXADECIMAL:
		for ( i = 0; i < _tcslen(VVE_DEFSETHEXADECIMAL); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETHEXADECIMAL[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETHEXADECIMAL[i]] = TRUE;
		}
		m_bEnableRangeCheck = TRUE;
		break;
	case SET_BINARY:
		for ( i = 0; i < _tcslen(VVE_DEFSETBINARY); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETBINARY[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETBINARY[i]] = TRUE;
		}
		m_bEnableRangeCheck = TRUE;
		break;
	case SET_FLOAT:
		for ( i = 0; i < _tcslen(VVE_DEFSETFLOAT); i++)
		{
			ASSERT((UCHAR)VVE_DEFSETFLOAT[i] < UCHAR_MAX);
			m_bValidChar[(UCHAR)VVE_DEFSETFLOAT[i]] = TRUE;
		}
		m_bEnableRangeCheck = TRUE;
		break;
	case SET_ALLCHAR:
	default:
		m_bEnableRangeCheck = FALSE;
		m_bAllCharValid = TRUE;
	}
	//store the selected set
	m_setCurrent = vveset;

}

void CSuperEdit::GetValidChars(CString & strValidChars)
{
	int j = 0;
	// 1st check to see if all characters are valid
	if(m_bAllCharValid || m_bNoValidation)
	{
		strValidChars = VVE_DEFSETALL;
		return;
	}

	//if here then
	//reset string sent to all spaces
	strValidChars = CString(_T(' '), VVE_MAX_CHAR - VVE_MIN_CHAR +1);
	//fill it with currnet allowable chars
	for (int i = VVE_MIN_CHAR; i < VVE_MAX_CHAR ; i++)
	{
		if(m_bValidChar[i] != FALSE)
			strValidChars.SetAt(j++, TCHAR(i));
	}
	//trim str
	strValidChars.TrimRight();
}

void CSuperEdit::ValidationChanged(BOOL bNewState)
{
	CWnd *pParent = GetParent();
	ASSERT(pParent);
	DWORD_PTR dwResult;
	NMHDR nm;
	nm.idFrom = GetDlgCtrlID();
	nm.hwndFrom = m_hWnd;
	nm.code = (UINT) m_iCurrentErr;
	//inform parent of validation state change
	::SendMessageTimeout(pParent->GetSafeHwnd(),UWM_VVE_VALIDITY_CHANGED,
				(WPARAM)&nm,
				(LPARAM) bNewState ,SMTO_BLOCK,1,&dwResult) ;

}

double CSuperEdit::GetWindowValue()
{
	double dRet;
	if(GetWindowValue(dRet))
		return dRet;
	else
		return 0.0;
}

BOOL CSuperEdit::GetWindowValue(double &dVal)
{
	CString s, strVal;
	GetWindowText(s);
	int iRangeCheck = VVE_OK;
	BOOL bRet;
	TCHAR *pEnd;
	switch(m_setCurrent)
	{
	//asking for double if you call this function (bum bum) aaaaaany how
	//i handle it by calling the long implimentation - since
	//the user is requesting a double, a long/ulong wrapped
	//in a double will have to do (for now)!
	case SET_HEXADECIMAL:
	case SET_OCTAL:
	case SET_BINARY:
	case SET_DECIMAL:
		{
			iRangeCheck = CheckRange(m_setCurrent,s);
			unsigned long ul;
			bRet = GetWindowValue(ul);
			dVal = (double) ul;
			return bRet;
		}
		break;
	case SET_FLOAT:
		dVal = _tcstod(s,&pEnd);
		iRangeCheck = CheckRange(SET_FLOAT,s);
		break;
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_CUSTOM:
	case SET_SDECIMAL:
	default:
		{
			iRangeCheck = CheckRange(SET_SDECIMAL,s);
			long l;
			bRet = GetWindowValue(l);
			dVal = (double) l;
			return bRet;
		}
		break;
	}
	return (iRangeCheck == VVE_OK || m_bEnableRangeCheck == FALSE )
		&& (m_bValidSyntax || m_bNoValidation == TRUE);
}

//returns FALSE if convertion is unverified
//(this may happen if control contains invalid characters
//or the value is too large for container)
BOOL CSuperEdit::GetWindowValue(long &lVal)
{
	CString s, strVal;
	GetWindowText(s);
	int iRangeCheck = VVE_OK;
	TCHAR *pEnd = NULL;
	switch(m_setCurrent)
	{
	case SET_DECIMAL:
		iRangeCheck = CheckRange(s);
		lVal = (long)_tcstoul(s,&pEnd,10);
		break;
	case SET_OCTAL:
		iRangeCheck = CheckRange(s);
		lVal = (long)_tcstoul(s,&pEnd,8);
		break;
	case SET_HEXADECIMAL:
		iRangeCheck = CheckRange(s);
		lVal = (long)_tcstoul(s,&pEnd,16);
		break;
	case SET_BINARY:
		iRangeCheck = CheckRange(s);
		lVal = (long)_tcstoul(s,&pEnd,2);
		break;
	case SET_FLOAT:
		{
			//the check for float is done in
			//GetWindowValue(double)
			double d;
			BOOL bRet = GetWindowValue(d);
			lVal = (long) d;
			if(d > LONG_MAX || d < LONG_MIN)
				return FALSE;
			return bRet;
		}
	case SET_SDECIMAL:
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_CUSTOM:
		//as we are rerieving a value, check as a ULONG.
		//this way, a negitive value can be got from a
		//number > LONG_MAX but less than ULONG_MAX
		iRangeCheck = CheckRange(SET_DECIMAL,s);
		lVal = (long)_tcstoul(s,&pEnd,10);
		break;
	}
	return (iRangeCheck == VVE_OK || m_bEnableRangeCheck == FALSE )
		&& (m_bValidSyntax || m_bNoValidation == TRUE);
}

//returns FALSE if convertion is unverified
//(this may happen if control contains invalid characters
//or the value is too large for container)
BOOL CSuperEdit::GetWindowValue(unsigned long &lVal)
{
	CString s, strVal;
	GetWindowText(s);
	int iRangeCheck = VVE_NO_LARGE;
	TCHAR *pEnd = NULL;
	switch(m_setCurrent)
	{
	case SET_DECIMAL:
		iRangeCheck = CheckRange(SET_DECIMAL,s);
		lVal = _tcstoul(s,&pEnd,10);
		break;
	case SET_OCTAL:
		iRangeCheck = CheckRange(SET_OCTAL,s);
		lVal = _tcstoul(s,&pEnd,8);
		break;
	case SET_HEXADECIMAL:
		iRangeCheck = CheckRange(SET_HEXADECIMAL,s);
		lVal = _tcstoul(s,&pEnd,16);
		break;
	case SET_BINARY:
		iRangeCheck = CheckRange(SET_BINARY,s);
		lVal = _tcstoul(s,&pEnd,2);
		break;
	case SET_FLOAT:
		{
			double d;
			BOOL bRet = GetWindowValue(d);
			lVal = (unsigned long) d;
			if(d > ULONG_MAX)
				return FALSE;
			return bRet;
		}
	case SET_SDECIMAL:
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_CUSTOM:
		iRangeCheck = CheckRange(SET_DECIMAL,s);//force ULONG checking
		lVal = _tcstoul(s,&pEnd,10);
		break;
	}
	return (iRangeCheck == VVE_OK || m_bEnableRangeCheck == FALSE )
		&& (m_bValidSyntax || m_bNoValidation == TRUE);
}




//returns TRUE if the entered value is validated (according to settings & set selected)
//eg1, if mode is SET_ALPHA & you set any value, validation will fail
//eg2, if mode is SET_BINARY & MAX_LEN == 5, SetWindowValue(12345) would insert
//the test 11000000111001 into the edit but the return would fail
//eg3, if mode is SET_CUSTOM, SetWindowValue would default to decimal input & validation
//would depend on your character set
//retuns false otherwise
BOOL CSuperEdit::SetWindowValue(unsigned long lSetVal)
{
	CString strVal = _T("");
	TCHAR szTemp[LONG_STRMAX] = _T("");//long enough for representation of a long as a string
	switch(m_setCurrent)
	{
	case SET_DECIMAL:
		strVal = _ultot(lSetVal,szTemp,10);
		SetWindowText(strVal);
		break;
	case SET_SDECIMAL:
		strVal = _ltot(lSetVal,szTemp,10);
		SetWindowText(strVal);
		break;
	case SET_OCTAL:
		strVal = _ultot(lSetVal,szTemp,8);
		SetWindowText(strVal);
		break;
	case SET_HEXADECIMAL:
		strVal = _ultot(lSetVal,szTemp,16);
		SetWindowText(strVal);
		break;
	case SET_BINARY:
		strVal = _ultot(lSetVal,szTemp,2);
		SetWindowText(strVal);
		break;
	case SET_FLOAT:
		return SetWindowValue((double)lSetVal);
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_CUSTOM:
		strVal = _ultot(lSetVal,szTemp,10);
		SetWindowText(strVal);
	}
	//SetWindowText(strVal);
	//causes a validation thru OnChange
	//so m_bValidSyntax will be correct
	return m_bValidSyntax;

}
BOOL CSuperEdit::SetWindowValue(long lSetVal)
{
	CString strVal = _T("");
	TCHAR szTemp[LONG_STRMAX] = _T("");//long enough for representation of a long as a string
	switch(m_setCurrent)
	{
	case SET_DECIMAL:
		strVal = _ultot(lSetVal,szTemp,10);
		SetWindowText(strVal);
		break;
	case SET_SDECIMAL:
		strVal = _ltot(lSetVal,szTemp,10);
		SetWindowText(strVal);
		break;
	case SET_OCTAL:
		strVal = _ltot(lSetVal,szTemp,8);
		SetWindowText(strVal);
		break;
	case SET_HEXADECIMAL:
		strVal = _ltot(lSetVal,szTemp,16);
		SetWindowText(strVal);
		break;
	case SET_BINARY:
		strVal = _ltot(lSetVal,szTemp,2);
		SetWindowText(strVal);
		break;
	case SET_FLOAT:
		return SetWindowValue((double)lSetVal);
		break;
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_CUSTOM:
		strVal = _ltot(lSetVal,szTemp,10);
		SetWindowText(strVal);//causes a validation thru OnChange
	}
	//SetWindowText(strVal);
	//causes a validation thru OnChange
	//so m_bValidSyntax will be correct
	return m_bValidSyntax;
}




/************************************************************************
SetWindowValue(double lSetVal)
As this function is primaraly for use with SET_FLOAT, some conversions are
made when this is not the case

dSetVal is cast to a ULONG when setting an edit cotrol with one of
the following sets selected...
	SET_BINARY
	SET_HEXADECIMAL
	SET_OCTAL
	SET_DECIMAL
dSetVal is cast to a LONG when setting an edit cotrol with one of
the following sets selected...
	SET_ALPHA:
	SET_ALPHANUM:
	SET_ALLCHAR:
	SET_CUSTOM:
	SET_SDECIMAL:
/************************************************************************/
BOOL CSuperEdit::SetWindowValue(double dSetVal)
{
	CString strVal = _T("");
	TCHAR szTemp[LONG_STRMAX] = _T("");//long enough for representation of a long as a string
	BOOL bRangeOK = TRUE;
	switch(m_setCurrent)
	{
	case SET_DECIMAL:
		strVal.Format(_T("%u"),(unsigned long)dSetVal);
/*		if(dSetVal < 0)
			bRangeOK = (LONG_MIN < (__int64)dSetVal && LONG_MAX > (__int64)dSetVal);
		else
			bRangeOK = (ULONG_MAX > (unsigned __int64)dSetVal);  */
		SetWindowText(strVal);
		bRangeOK = CheckRange(m_setCurrent,strVal) ==VVE_OK ;
		break;
	case SET_OCTAL:
		strVal.Format(_T("%o"),(unsigned long)dSetVal);
/*		if(dSetVal < 0)
			bRangeOK = (LONG_MIN < (__int64)dSetVal && LONG_MAX > (__int64)dSetVal);
		else
			bRangeOK = (ULONG_MAX > (unsigned __int64)dSetVal);  */
		SetWindowText(strVal);
		bRangeOK = CheckRange(m_setCurrent,strVal) ==VVE_OK ;
		break;
	case SET_HEXADECIMAL:
		strVal.Format(_T("%x"),(unsigned long)dSetVal);
/*		if(dSetVal < 0)
			bRangeOK = (LONG_MIN < (__int64)dSetVal && LONG_MAX > (__int64)dSetVal);
		else
			bRangeOK = (ULONG_MAX > (unsigned __int64)dSetVal);  */
		SetWindowText(strVal);
		bRangeOK = CheckRange(m_setCurrent,strVal) ==VVE_OK ;
		break;
	case SET_BINARY:
		strVal = _ultot((unsigned long)dSetVal,szTemp,2);
/*		if(dSetVal < 0)
			bRangeOK = (LONG_MIN < (__int64)dSetVal && LONG_MAX > (__int64)dSetVal);
		else
			bRangeOK = (ULONG_MAX > (unsigned __int64)dSetVal);  */
		SetWindowText(strVal);
		bRangeOK = CheckRange(m_setCurrent,strVal) ==VVE_OK ;
		break;
	case SET_FLOAT:
		strVal.Format(_T("%0.8g"), dSetVal);
		try
		{
			//SetWindowText(strVal);
			::SetWindowText(this->m_hWnd,strVal);
		}
		catch (CException* e)
		{
			TCHAR strError[255]; 
			e->GetErrorMessage (strError, sizeof(strError));
			CString strTmp;
			strTmp.Format(_T("SetWindowText函数错误，错误原因：%s"),strError);
			AfxMessageBox(strTmp);
		}
		catch (...)
		{
			AfxMessageBox(_T("调用SetWindowText函数时出现未知错误！"));
		}
		bRangeOK = CheckRange(m_setCurrent,dSetVal) == VVE_OK;
		break;
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_CUSTOM:
	case SET_SDECIMAL:
	default:
		strVal.Format(_T("%d"),(long)dSetVal);
		SetWindowText(strVal);//causes Validate to be called thru OnChange
		bRangeOK = CheckRange(SET_SDECIMAL,dSetVal) ==VVE_OK ;
		break;
	}
	return m_bValidSyntax && bRangeOK;
}

//returns VVE_OK, VVE_NO_SMALL or VVE_NO_LARGE
int CSuperEdit::CheckRange()
{
	CString s;
	GetWindowText(s);
	return CheckRange(GetValidSet(),s);
}

//returns VVE_OK, VVE_NO_SMALL or VVE_NO_LARGE
int CSuperEdit::CheckRange(VVEset vveset, LPCTSTR lpszVal)
{

	CString s(lpszVal);
	switch(vveset)
	{
	case SET_DECIMAL:
		{
			s.TrimLeft(_T("0-+ "));
			//1st, if the char string is > 10 (+1 for sign) characters, then it is too large for
			//a long
			if(s.SpanIncluding(VVE_DEFSETSDECIMAL).GetLength() > 11)//include all valid chars (& sign)
				return VVE_NO_LARGE;
			__int64 i64 =  _ttoi64(lpszVal);//get the window value as a 64bit int
			if(i64 < 0)
				return VVE_NO_SMALL;
			else if(i64 > ULONG_MAX) //now a comparison to ULONG max can be made
				return VVE_NO_LARGE;
		}
		break;
	case SET_OCTAL:
		//as 37777777777 is largest octal val representable in an long/ulong
		//lengh if 1st verified, then string is compared to 37777777777 comparison
		//is simply returned as VVE_OK == 0, VVE_NO_LARGE = 1
		s.TrimLeft(_T("0"));
		s.SpanIncluding(VVE_DEFSETOCTAL);//remove rubbish after ok chars
		//if s > _T("37777777777") then 1 is returned (implicit VVE_NO_LARGE)
		if(s.GetLength() > 10)
			return s.Compare(_T("37777777777")) == 1;
		break;
	case SET_HEXADECIMAL://simple length check as FFFFFFFF is max val
		s.TrimLeft(_T("0 "));//remove leading spaces & 0s
		s.SpanIncluding(VVE_DEFSETHEXADECIMAL);//clean up
		if(s.GetLength() > 8)
			return VVE_NO_LARGE;
		break;
	case SET_BINARY://simple length check as 1111 1111 1111 1111 1111 1111 1111 1111 is max val
		s.TrimLeft(_T("0 "));//remove leading spaces & 0s
		s.SpanIncluding(VVE_DEFSETBINARY);//remove rubbish after ok chars
		if(s.GetLength() > 32)
			return VVE_NO_LARGE;
		break;
	case SET_FLOAT:
		{
			TCHAR *end;
			s.TrimLeft(_T("0 "));
			double d = _tcstod(s,&end);
			CString ErrTest(end);
			if(ErrTest.Find(_T("INF")) >= 0)
				return VVE_NO_LARGE;
			if( _isnan(d) || _finite(d) == FALSE || d == 0)
			{
				//s may be "+0" so check this - if any digit other than 0 is in the string,
				//then 0 s != a string == "0" signed or not!
				if(d == 0)
					if(s.FindOneOf(_T("123456789")) < 0)
						return VVE_OK;//must be zero string!
				return (d==0 ? VVE_NO_SMALL : VVE_NO_LARGE);
			}
		}
		break;
	case SET_CUSTOM:
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_SDECIMAL:
	default://all other sets default to signed decimal LONG for range check
		{
			s.TrimLeft(_T("0-+ "));
			//1st, if the char string is > 11 characters, then it is too large for
			//a long
			if(s.SpanIncluding(VVE_DEFSETSDECIMAL).GetLength() > 11)//include all valid chars
				return VVE_NO_LARGE;
			__int64 i64 =  _ttoi64(lpszVal);//get the window value as a 64bit int
			if(i64 < LONG_MIN)// compare to LONG min
				return VVE_NO_SMALL;
			else if(i64 > LONG_MAX) //now a comparison to LONG max can be made
				return VVE_NO_LARGE;
		}
		break;
	}
	return VVE_OK;
}

//returns VVE_OK, VVE_NO_SMALL or VVE_NO_LARGE
int CSuperEdit::CheckRange(VVEset vveset, double dVal)
{

	if( _isnan(dVal) || _finite(dVal) == FALSE)
		return VVE_NO_LARGE;//default check on double - just return NO_LARGE

	switch(vveset)
	{
	case SET_DECIMAL:
	case SET_OCTAL:
	case SET_HEXADECIMAL:
	case SET_BINARY:
			if(dVal < 0)
				return VVE_NO_SMALL;
			else if(dVal > ULONG_MAX) //now a comparison to ULONG max can be made
				return VVE_NO_LARGE;
		break;
	case SET_FLOAT:
			;//must be ok, it came in a double
		break;
	case SET_CUSTOM:
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	case SET_SDECIMAL:
	default://all other sets default to signed decimal LONG for range check
			if(dVal < LONG_MIN)// compare to LONG min
				return VVE_NO_SMALL;
			else if(dVal > LONG_MAX) //now a comparison to LONG max can be made
				return VVE_NO_LARGE;
		break;
	}
	return VVE_OK;
}

void CSuperEdit::SetAllValue(double dDefValue,double dMinValue,double dMaxValue)
{
	m_dDefValue = dDefValue;
	m_dMinValue = dMinValue;
	m_dMaxValue = dMaxValue;

	if(m_dMaxValue < m_dMinValue)
	{
		m_dMinValue = dMaxValue;
		m_dMaxValue = dMinValue;
	}

	if(m_dDefValue > m_dMaxValue || m_dDefValue < m_dMinValue)
		m_dDefValue = m_dMinValue;

	CString s;
	GetWindowText(s);

	if((!m_bAllowEmpty && s.IsEmpty())
		|| (!s.IsEmpty() && ((_tstof(s) > m_dMaxValue+m_dPrec) || (_tstof(s) < m_dMinValue-m_dPrec))))
		SetWindowValue(m_dDefValue);
}

int CSuperEdit::CheckValueOutRange()
{
	CString s;
	GetWindowText(s);
	if(s.IsEmpty())
	{
		if(m_bAllowEmpty)
		{
			m_bValidValue = TRUE;
			return VVE_OK;
		}
		else
		{
			m_bValidValue = FALSE;
			return VVE_VAL_LITTE;
		}
	}

	double d;
	GetWindowValue(d);
	int nRet = CheckValueOutRange(d);
	m_bValidValue = (nRet == VVE_OK) ? TRUE : FALSE;

	return nRet;
}

int CSuperEdit::CheckValueOutRange(double d)
{
	if(m_bEableValueCheck == FALSE || m_bNoValidation == TRUE)
		return VVE_OK;

	switch(m_setCurrent)
	{
	case SET_DECIMAL:
	case SET_SDECIMAL:
	case SET_OCTAL:
	case SET_HEXADECIMAL://simple length check as FFFFFFFF is max val
	case SET_BINARY://simple length check as 1111 1111 1111 1111 1111 1111 1111 1111 is max val
	case SET_FLOAT:
		if(d > m_dMaxValue+m_dPrec)
			return VVE_VAL_BIG;
		if(d < m_dMinValue-m_dPrec)
			return VVE_VAL_LITTE;
		break;
	case SET_CUSTOM:
	case SET_ALPHA:
	case SET_ALPHANUM:
	case SET_ALLCHAR:
	default:
		break;
	}
	return VVE_OK;
}

BOOL CSuperEdit::GetAllValue(double &dDefValue,double &dMinValue,double &dMaxValue)
{
	if(m_bEableValueCheck)
	{
		dDefValue = m_dDefValue;
		dMinValue = m_dMinValue;
		dMaxValue = m_dMaxValue;
		return TRUE;
	}
	else
		return FALSE;
}

void CSuperEdit::OnKillFocus (CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	if(m_bEableValueCheck)
		if(!IsValid())
			SetWindowValue(m_dDefValue);
}

void CSuperEdit::SetTipText(LPCTSTR szTip/* = NULL*/)
{
	if(szTip == NULL)
	{
		m_bSetTipTextByUser = FALSE;
		m_szTip = _T("");
		return;
	}
	m_bSetTipTextByUser = TRUE;
	m_szTip = szTip;

}

void CSuperEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	if((!m_bSetTipTextByUser || m_szTip == _T("")) && m_bEableTip && m_bEableValueCheck)
	{
		m_szTip.Format( _T("最小值:%g\n最大值:%g\n默认值:%g"),
			m_dMinValue, m_dMaxValue, m_dDefValue );
		// 如果当前已经增加了一个提示信息，则需要删除之
		if(m_ToolTip.GetToolCount() != 0) m_ToolTip.DelTool(this,1);
	}

	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn;
		GetClientRect(rectBtn);
		CString sDisplay;
		if( !m_ToolTip.AddTool(this, m_szTip, rectBtn, 1) )
			;//AfxMessageBox( _T("Create ToolTip Error!") );
	} // if
	m_ToolTip.Activate( TRUE );
	m_ToolTip.Update();
}

void CSuperEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	m_ToolTip.Update();

	CEdit::OnMouseMove(nFlags, point);
}

BOOL CSuperEdit::PreTranslateMessage(MSG* pMsg)
{
	if( m_bEableTip )
		m_ToolTip.RelayEvent( pMsg );

	return CEdit::PreTranslateMessage(pMsg);
}


//经常用的几种组合
void CSuperEdit::SetInputFloat(double dDef,double dMin/* = 0*/,double dMax /*= 1E30*/,BOOL bAllowEmpty /*= FALSE*/,BOOL bEableTip /*= TRUE*/,LPCTSTR szTip /*= NULL*/)//小数
{
	SetValidCharSet(SET_FLOAT);
	SetEmptyValid(bAllowEmpty);
	SetDisableValidation(FALSE);
	SetEnableRangeCheck();
	SetEnableLengthCheck();
	SetEableValueCheck();
	SetAllValue(dDef,dMin,dMax);
	SetEableTip(bEableTip);
	SetTipText(szTip);
}

void CSuperEdit::SetInputSInt(int nDef,int nMin /*= －1E5*/,int nMax/* = 1E5*/,BOOL bAllowEmpty /*= FALSE*/,BOOL bEableTip /*= TRUE*/,LPCTSTR szTip /*= NULL*/)//有符号整数
{
	SetValidCharSet(SET_SDECIMAL);
	SetEmptyValid(bAllowEmpty);
	SetDisableValidation(FALSE);
	SetEnableRangeCheck();
	SetEnableLengthCheck();
	SetEableValueCheck();
	SetAllValue(nDef,nMin,nMax);
	SetEableTip(bEableTip);
	SetTipText(szTip);

}

void CSuperEdit::SetInputInt(int nDef,int nMin /*= 0*/,int nMax /*= 1E5*/,BOOL bAllowEmpty /*= FALSE*/,BOOL bEableTip /*= TRUE*/,LPCTSTR szTip /*= NULL*/)//正整数
{
	SetValidCharSet(SET_DECIMAL);
	SetEmptyValid(bAllowEmpty);
	SetDisableValidation(FALSE);
	SetEnableRangeCheck();
	SetEnableLengthCheck();
	SetEableValueCheck();
	SetAllValue(nDef,nMin,nMax);
	SetEableTip(bEableTip);
	SetTipText(szTip);
}

void CSuperEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSuperEdit::SetPrecValue(double dPrec)
{
	m_dPrec = dPrec;
}

double CSuperEdit::PrecValue()
{
	return m_dPrec;
}
