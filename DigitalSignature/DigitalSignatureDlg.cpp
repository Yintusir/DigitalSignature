
// DigitalSignatureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalSignature.h"
#include "DigitalSignatureDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalSignatureDlg �Ի���



CDigitalSignatureDlg::CDigitalSignatureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDigitalSignatureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDigitalSignatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDigitalSignatureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDigitalSignatureDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDigitalSignatureDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDigitalSignatureDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDigitalSignatureDlg ��Ϣ�������

BOOL CDigitalSignatureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDigitalSignatureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDigitalSignatureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDigitalSignatureDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog cfd(TRUE,_T("exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("��ִ���ļ���*.exe��|*.exe|"));
	cfd.DoModal();
	SetDlgItemText(IDC_EDIT1,cfd.GetPathName());
}


void CDigitalSignatureDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog cfd(TRUE,_T("exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("��ִ���ļ���*.exe��|*.exe|"));
	cfd.DoModal();
	SetDlgItemText(IDC_EDIT2,cfd.GetPathName());
}


void CDigitalSignatureDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!GetDS())
	{
		return;
	}
	CString strFile;
	GetDlgItemText(IDC_EDIT2,strFile);
	CFile file1(strFile,CFile::modeReadWrite);
	if (!file1.m_hFile)
	{
		SetDlgItemText(IDC_STATIC_LOG,_T("��Ŀ���ļ�ʧ�ܣ�"));
		return;
	}
	//��������ǩ����С
	int dsSize = m_size[0];
	dsSize |= ((m_size[1] << 8) & 0xFF00);

	DWORD dwSize = file1.GetLength();
	BYTE * pData = new BYTE[dwSize + dsSize];
	file1.Read(pData,dwSize);
	


	//�ж��Ƿ��ǿ�ִ���ļ�
	if(pData[0] != 'M' || pData[1] != 'Z')
	{
		MessageBox(_T("Ŀ���ļ�����һ����ִ���ļ���"));
		return;
	}
	//����PEƫ�ƾ���
	BYTE temp1[]={
		pData[63],
		pData[62],
		pData[61],
		pData[60]
	};
	int PE = bytesToInt(temp1);
	
	
	//�ж�ָ��λ���Ƿ���PEͷ
	if (pData[PE] != 'P' || pData[PE + 1] != 'E')
	{
		MessageBox(_T("û����Ŀ���ļ����ҵ�PEͷ��"));
		return;
	}

	
	//��������ǩ��λ��
	byte temp2[] = {
		pData[PE + 152 + 3],
		pData[PE + 152 + 2],
		pData[PE + 152 + 1],
		pData[PE + 152 + 0]
	};
	int ds = bytesToInt(temp2);
	if(ds != 0)
	{
		MessageBox(_T("Ŀ���ļ��Ѿ���������ǩ����"));
		return;
	}
	//��������ǩ������ִ���ļ����λ��
	for (int i = 0; i < dsSize; i++)
	{
		pData[dwSize + i] = m_byte[i];
	}
	//�޸���ִ���ļ�
	BYTE addr [4];
	memset(addr,0,sizeof(byte) *  4);
    addr[0] = (byte) (0xff & dwSize);
    addr[1] = (byte) ((0xff00 & dwSize) >> 8);
    addr[2] = (byte) ((0xff0000 & dwSize) >> 16);
    addr[3] = (byte) ((0xff000000 & dwSize) >> 24);

	pData[PE + 152 + 3] = addr[3];
	pData[PE + 152 + 2] = addr[2];
	pData[PE + 152 + 1] = addr[1];
	pData[PE + 152 + 0] = addr[0];

	pData[PE + 152 + 4] = m_size[0];
	pData[PE + 152 + 5] = m_size[1];
	file1.SetLength(0);
	file1.Write(pData,dwSize + dsSize);
	file1.Close();
	delete [] m_byte;
	MessageBox(_T("���Ƴɹ���"));
}


int CDigitalSignatureDlg::bytesToInt(byte* bytes) 
{
    int addr = bytes[3] & 0xFF;
    addr |= ((bytes[2] << 8) & 0xFF00);
    addr |= ((bytes[1] << 16) & 0xFF0000);
    addr |= ((bytes[0] << 24) & 0xFF000000);
    return addr;
}

bool CDigitalSignatureDlg::GetDS(void)
{

	//���ļ������������
	CString strFile;
	GetDlgItemText(IDC_EDIT1,strFile);
	CFile file1(strFile,CFile::modeRead);
	if (!file1.m_hFile)
	{
		SetDlgItemText(IDC_STATIC_LOG,_T("��ԭʼ�ļ�ʧ�ܣ�"));
		return FALSE;
	}
	DWORD dwSize = file1.GetLength();
	BYTE * pData = new BYTE[dwSize];
	file1.Read(pData,dwSize);
	file1.Close();

	
	//�ж��Ƿ��ǿ�ִ���ļ�
	if(pData[0] != 'M' || pData[1] != 'Z')
	{
		MessageBox(_T("ԭʼ�ļ�����һ����ִ���ļ���"));
		return FALSE;
	}
	//����PEƫ�ƾ���
	BYTE temp1[]={
		pData[63],
		pData[62],
		pData[61],
		pData[60]
	};
	int PE = bytesToInt(temp1);
	
	
	//�ж�ָ��λ���Ƿ���PEͷ
	if (pData[PE] != 'P' || pData[PE + 1] != 'E')
	{
		MessageBox(_T("û����ԭʼ�ļ����ҵ�PEͷ��"));
		return FALSE;
	}

	
	//��������ǩ��λ��
	byte temp2[] = {
		pData[PE + 152 + 3],
		pData[PE + 152 + 2],
		pData[PE + 152 + 1],
		pData[PE + 152 + 0]
	};
	int ds = bytesToInt(temp2);
	
	//��������ǩ����С
	int dsSize = pData[PE + 152 + 4];
	dsSize |= ((pData[PE + 152 + 5] << 8) & 0xFF00);
	if(dsSize == 0)
	{
		MessageBox(_T("û����ԭʼ�ļ����ҵ�����ǩ����"));
		return FALSE;
	}
	m_size[0] = pData[PE + 152 + 4];
	m_size[1] = pData[PE + 152 + 5];

	m_byte = new BYTE[dsSize];
	for (int i = 0; i < dsSize; i++)
	{
		m_byte[i] = pData[ds + i];
	}

	delete [] pData;
	return TRUE;
}
