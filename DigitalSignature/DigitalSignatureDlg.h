
// DigitalSignatureDlg.h : ͷ�ļ�
//

#pragma once


// CDigitalSignatureDlg �Ի���
class CDigitalSignatureDlg : public CDialogEx
{
// ����
public:
	CDigitalSignatureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DIGITALSIGNATURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
