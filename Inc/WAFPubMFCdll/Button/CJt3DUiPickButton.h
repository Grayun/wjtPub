#pragma once

// --------------------------------------------------------------------------------
// Summary: 
//   ��ť��ʾһ�� ��ʰȡ��ͼƬ����ť��СΪ 24 x 24(AutoLoad֮���С)
//	 ���У�CJt3DUiPickButton m_btnPick;�����ʼ��ʱ��m_btnPick.AutoLoad()����
// --------------------------------------------------------------------------------
class WAFPUBMFCDLL_API CJt3DUiPickButton : public CButtonST
{
public:
	DECLARE_DYNAMIC(CJt3DUiPickButton)

	CJt3DUiPickButton();
	virtual ~CJt3DUiPickButton();

public:
	// --------------------------------------------------------------------------------
	// Summary: 
	//   ����ѡ��ͼƬ��������С��ѡ��ͼƬһ����λ�ò���
	// --------------------------------------------------------------------------------
	BOOL AutoLoad();
};
