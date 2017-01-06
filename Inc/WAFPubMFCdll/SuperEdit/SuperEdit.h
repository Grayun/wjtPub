#ifndef NOUSESUPEREDIT
#if !defined(SUPEREDIT_LKJPOKOPOFLKJGORFO__59875949_2008___)
#define SUPEREDIT_LKJPOKOPOFLKJGORFO__59875949_2008___


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSuperEdit window

// Predefined sets
#define VVE_DEFSETALPHANUM		_T(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")//×ÖÄ¸ºÍÊý×Ö
#define VVE_DEFSETALPHA			_T(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")//×ÖÄ¸
#define VVE_DEFSETDECIMAL		_T("0123456789")//Êý×Ö
#define VVE_DEFSETSDECIMAL		_T("-+0123456789")//Õý¸ººÅºÍÊý×Ö
#define VVE_DEFSETHEXADECIMAL	_T("abcdefABCDEF0123456789")//Ê®Áù½øÖÆµÄÓÐÐ§Êý¾Ý
#define VVE_DEFSETOCTAL			_T("01234567")//°Ë½øÖÆµÄÓÐÐ§Êý¾Ý
#define VVE_DEFSETBINARY		_T("01")//¶þ½øÖÆµÄÓÐÐ§Êý¾Ý
#define VVE_DEFSETFLOAT			_T("0123456789+-.Ee")//¸¡µãÐÍµÄÓÐÐ§×ÖÄ¸
#define VVE_DEFSETALL			_T("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ")//ËùÓÐµÄ×Ö·û

// Default colors
#define RGBDEF_EMPTY			::GetSysColor(COLOR_WINDOW)
#define RGBDEF_VALID			::GetSysColor(COLOR_WINDOW)/*RGB(192,255,192) //green*/
#define RGBDEF_INVALID			RGB(255,0,0) //red
#define RGBDEF_INCOMPLETE		RGB(255,255,128) //yellow
#define RGBDEF_CUST1			RGB(255,196,196) //ltred
#define RGBDEF_CUST2			RGB(190,190,255) //µ­×ÏÉ«

// user messages
#define UWM_VVE_DO_CUSTOMVALIDATION (WM_APP + 1)
#define UWM_VVE_VALIDITY_CHANGED (WM_APP + 2)

//context constants
#define ID_UNDO (WM_APP + 5)
#define ID_CUT (WM_APP + 6)
#define ID_COPY (WM_APP + 7)
#define ID_PASTE (WM_APP + 8)
#define ID_DELETE (WM_APP + 9)
#define ID_SEL_ALL (WM_APP + 10)

//other constants
#define LONG_STRMAX 33 /*enough for a string containing a binary/hexadecimal/octal equiv of a long/ulong*/

class WAFPUBMFCDLL_API CSuperEdit : public CEdit
{
// Construction & enums
public:
	//return constants - each has a matching brush (BR_INCOMPLETE, BR_SHORT, BR_LONG, BR_RANGEERR are same brush)
	enum VVEbrush {BR_ERROR = 0, BR_EMPTY, BR_VALID, BR_INVALIDSYNTAX, BR_INCOMPLETE, BR_SHORT, BR_LONG, BR_RANGEERR,BR_LITTE,BR_BIG, BR_CUSTOM_ERR1, BR_CUSTOM_ERR2};
	//built in validation sets
	enum VVEset {SET_CUSTOM, SET_ALLCHAR, SET_ALPHANUM, SET_ALPHA, SET_DECIMAL, SET_SDECIMAL, SET_HEXADECIMAL, SET_OCTAL, SET_BINARY, SET_FLOAT };
	//internal constants
	enum {VVE_LEN_SHORT = -1,VVE_VAL_LITTE = -1 , VVE_NO_SMALL = -1, VVE_OK, VVE_LEN_LONG, VVE_VAL_BIG, VVE_NO_LARGE = 1, VVE_MIN_CHAR = 32, VVE_MAX_CHAR = 255};
	//internal defaults
	enum {VVE_DEF_MINLEN = 0 , VVE_DEF_MAXLEN = 0xffff};
	CSuperEdit();

// Attributes
public:
	BOOL IsValid() 
	{return m_bNoValidation ? TRUE : m_bValid; };
	BOOL IsValidLength() 
	{return (m_bNoValidation || m_bAllowEmpty || !m_bEableLengthCheck) ? TRUE : m_bValidLength; };
	BOOL IsValidSyntax() 
	{return m_bNoValidation ? TRUE : m_bValidSyntax; };
	BOOL IsValidValue()
	{return (m_bNoValidation || !m_bEableValueCheck) ? TRUE : m_bValidValue; };


	//¾­³£ÓÃµÄ¼¸ÖÖ×éºÏ
	void SetInputFloat(double dDef,double dMin = 0,double dMax = 1E30,BOOL bAllowEmpty = FALSE, BOOL bEableTip = TRUE,LPCTSTR szTip = NULL);//Ð¡Êý
	void SetInputFloatSim(double dDef,BOOL bAllowEmpty = FALSE, BOOL bEableTip = FALSE,LPCTSTR szTip = NULL){SetInputFloat(dDef,0,1E30,bAllowEmpty,bEableTip,szTip);};
	void SetInputSInt(int nDef,int nMin = -1E5,int nMax = 1E5,BOOL bAllowEmpty = FALSE,BOOL bEableTip = TRUE,LPCTSTR szTip = NULL);//ÓÐ·ûºÅÕûÊý
	void SetInputSIntSim(int dDef,BOOL bAllowEmpty = FALSE, BOOL bEableTip = FALSE,LPCTSTR szTip = NULL){SetInputSInt(dDef,(int)-1E5,(int)1E5,bAllowEmpty,bEableTip,szTip);};
	void SetInputInt(int nDef,int nMin = 0,int nMax = 1E5,BOOL bAllowEmpty = FALSE,BOOL bEableTip = TRUE,LPCTSTR szTip = NULL);//ÕýÕûÊý
	void SetInputIntSim(int dDef,BOOL bAllowEmpty = FALSE, BOOL bEableTip = FALSE,LPCTSTR szTip = NULL){SetInputInt(dDef,0,(int)1E5,bAllowEmpty,bEableTip,szTip);};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	int  CheckRange();
	int  CheckRange(LPCTSTR lpszVal)
	{
		return CheckRange(m_setCurrent,lpszVal);
	};
	static int CheckRange(VVEset vveset, LPCTSTR lpszVal);
	static int CheckRange(VVEset vveset, double dVal);

	void SetEnableRangeCheck(BOOL bEnable = TRUE){m_bEnableRangeCheck = bEnable;};//ÉèÖÃ·¶Î§¼ì²é×´Ì¬
	BOOL GetEnableRangeCheck(){return m_bEnableRangeCheck;};//µÃµ½·¶Î§¼ì²é×´Ì¬

	void SetCustomErrorIsValid(int iCustomWhich, BOOL bSetIsValid = TRUE)
	{	if(iCustomWhich > 2 || iCustomWhich < 1)
			return ;
		m_bCustomErrorIsValid[iCustomWhich] = bSetIsValid;
	};
	BOOL GetCustomErrorIsValid(int iCustomWhich, BOOL &bCustomIsValid )
	{	if(iCustomWhich > 2 || iCustomWhich < 1)
			return FALSE;
		bCustomIsValid = m_bCustomErrorIsValid[iCustomWhich];
		return TRUE;;
	};
	
	void DoValidation(BOOL bRedraw = TRUE);//explicit request

	void SetEnableLengthCheck(BOOL bEnable = TRUE){m_bEableLengthCheck = bEnable;};//ÉèÖÃ³¤¶È¼ì²é
	BOOL GetEnableLengthCheck(){return m_bEableLengthCheck;};

	void SetEableValueCheck(BOOL bEnable = TRUE){m_bEableValueCheck = bEnable;};
	BOOL GetEableValueCheck(){return m_bEableValueCheck;};

	BOOL SetBrushColour(VVEbrush brWhich, COLORREF NewColour);//ÉèÖÃ¸÷ÖÖ´íÎóÊ±±à¼­¿òµÄÑÕÉ«
	COLORREF GetBrushColour(VVEbrush brWhich);

	BOOL SetWindowValue(double fSetVal);
	BOOL SetWindowValue(long lSetVal);
	BOOL SetWindowValue(unsigned long lSetVal);
	BOOL GetWindowValue(double &fVal);//returns FALSE if string is too large for double
	double GetWindowValue();
	BOOL GetWindowValue(long &lVal);//returns FALSE if string is too small/large for long
	BOOL GetWindowValue(unsigned long &lVal);//returns FALSE if string is too large for ulong

	BOOL GetAllCharEntry(){return m_bAllowAllCharEntry;};
	void SetAllCharEntry(BOOL bAllowAllCharsIn = TRUE){m_bAllowAllCharEntry = bAllowAllCharsIn;};

	BOOL GetEmptyValid(){return m_bAllowEmpty;};
	void SetEmptyValid(BOOL bEmptyOK = TRUE){m_bAllowEmpty = bEmptyOK;};

	void SetDisableValidation(BOOL bNoValidation = TRUE){m_bNoValidation = bNoValidation;};
	BOOL GetDisableValidation(){return m_bNoValidation;};

	void SetCustomValidation(BOOL bUseCustom = TRUE){m_bCustomValidation = bUseCustom;};
	BOOL GetCustomValidation(){return m_bCustomValidation;};

	void SetValidCharSet(VVEset vveset, LPCTSTR cstrCustomChar = NULL);//ÉèÖÃÓÐÐ§µÄ×Ö·û´®¼¯
	
	void SetValidSet(VVEset vveset){m_setCurrent = vveset;};
	VVEset GetValidSet(){return m_setCurrent;};
	
	void SetValidChars(LPCTSTR cstrCustomChar);
	void GetValidChars(CString & strValidChars);

	void SetTextLenMinMax(int iMin = VVE_DEF_MINLEN, int iMax = VVE_DEF_MAXLEN);//ÉèÖÃÎÄ±¾µÄ×îÐ¡³¤¶ÈºÍ×î´ó³¤¶È
	int	 GetTextMinLen(){return m_iMinInputLen;};
	int  GetTextMaxLen(){return m_iMaxInputLen;};

	//ÉèÖÃÄ¬ÈÏÖµ
	void SetAllValue(double dDefValue,double dMinValue = 0,double dMaxValue = 1E30);
	BOOL GetAllValue(double &dDefValue,double &dMinValue,double &dMaxValue);
	
	//ToolTip
	void SetEableTip(BOOL bEableTip = TRUE){m_bEableTip = bEableTip;};
	BOOL GetEableTip(){return m_bEableTip;}; 

	void SetTipText(LPCTSTR szTip = NULL);
	CString GetTipText(){return m_szTip;};
	void SetPrecValue(double dPrec);	//ÉèÖÃÊµÊý±È½Ï¾«¶È
	double PrecValue();					//²éÑ¯ÊµÊý±È½Ï¾«¶È
	
	virtual ~CSuperEdit();

	// Generated message map functions
protected:
	void   SelectAll(){  if(m_hWnd){SetFocus();	SetSel(0,-1);}  };
	void   ValidationChanged(BOOL bNewState);//post new state to parent
	CBrush m_brEmpty;
 	CBrush m_brValid; 
	CBrush m_brInvalid;
	CBrush m_brIncomplete;
	CBrush m_brCustom1;
	CBrush m_brCustom2;
	CBrush * Validate();
	CBrush * CheckAlphaNum()	{return Validate();};
	CBrush * CheckDecimal()		{return Validate();};
	CBrush * CheckAlpha()		{return Validate();};
	CBrush * CheckHexadecimal() {return Validate();};
	CBrush * CheckOctal()		{return Validate();};
	CBrush * CheckBinary()		{return Validate();};      
	CBrush * CheckFloat();        
	CBrush * CustomValidation();

	int    CheckLen();//returns VVE_LEN_SHORT , VVE_LEN_LONG or VVE_OK
	int    CheckLen(LPCTSTR str);//returns VVE_LEN_SHORT , VVE_LEN_LONG or VVE_OK
	
	int    CheckValueOutRange();//returns VVE_OK  VVE_VAL_LITTE VVE_VAL_BIG
	int    CheckValueOutRange(double d);//returns VVE_OK  VVE_VAL_LITTE VVE_VAL_BIG
	
	//monitoring
	BOOL m_bValid;
	BOOL m_bValidSyntax;
	BOOL m_bValidLength;
	BOOL m_bValidValue;
	BOOL m_bCustomValidationCallOK;
	VVEbrush m_iCurrentErr;//used for context info
	//allwable characters
	BOOL m_bValidChar[UCHAR_MAX];
	BOOL m_bAllCharValid;
	//input lenth
	int m_iMinInputLen;
	int m_iMaxInputLen;
	//currently selected set / brush
	VVEset m_setCurrent;
	//Options
	BOOL m_bCustomValidation;
	BOOL m_bNoValidation;
	BOOL m_bEableLengthCheck;
	BOOL m_bAllowEmpty;
	BOOL m_bAllowAllCharEntry;//allows invalid chars into control 
	BOOL m_bCustomErrorIsValid[3];
	BOOL m_bEnableRangeCheck;
	//min max default
	BOOL m_bEableValueCheck;
	double m_dDefValue;
	double m_dMinValue;
	double m_dMaxValue;
	double m_dPrec;//Ôö¼Ó¾«¶È¿ØÖÆ
	//tooltip text
	CString m_szTip;
	BOOL    m_bEableTip;
	BOOL    m_bSetTipTextByUser;
	CToolTipCtrl m_ToolTip;

	//{{AFX_MSG(CSuperEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg HBRUSH CtlColor(CDC * dc, UINT id);
	afx_msg BOOL OnChange();
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(SUPEREDIT_LKJPOKOPOFLKJGORFO__59875949_2008___)
#endif
