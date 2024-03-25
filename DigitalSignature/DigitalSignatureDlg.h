
// DigitalSignatureDlg.h : 头文件
//

#pragma once


// CDigitalSignatureDlg 对话框
class CDigitalSignatureDlg : public CDialogEx
{
// 构造
public:
	CDigitalSignatureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIGITALSIGNATURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	int bytesToInt(byte* bytes);
	bool GetDS(void);
	BYTE * m_byte;
	BYTE m_size[2];

};
