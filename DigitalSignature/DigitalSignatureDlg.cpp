
// DigitalSignatureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalSignature.h"
#include "DigitalSignatureDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalSignatureDlg 对话框



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


// CDigitalSignatureDlg 消息处理程序

BOOL CDigitalSignatureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDigitalSignatureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDigitalSignatureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDigitalSignatureDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog cfd(TRUE,_T("exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("可执行文件（*.exe）|*.exe|"));
	cfd.DoModal();
	SetDlgItemText(IDC_EDIT1,cfd.GetPathName());
}


void CDigitalSignatureDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog cfd(TRUE,_T("exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("可执行文件（*.exe）|*.exe|"));
	cfd.DoModal();
	SetDlgItemText(IDC_EDIT2,cfd.GetPathName());
}


void CDigitalSignatureDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!GetDS())
	{
		return;
	}
	CString strFile;
	GetDlgItemText(IDC_EDIT2,strFile);
	CFile file1(strFile,CFile::modeReadWrite);
	if (!file1.m_hFile)
	{
		SetDlgItemText(IDC_STATIC_LOG,_T("打开目标文件失败！"));
		return;
	}
	//计算数字签名大小
	int dsSize = m_size[0];
	dsSize |= ((m_size[1] << 8) & 0xFF00);

	DWORD dwSize = file1.GetLength();
	BYTE * pData = new BYTE[dwSize + dsSize];
	file1.Read(pData,dwSize);
	


	//判断是否是可执行文件
	if(pData[0] != 'M' || pData[1] != 'Z')
	{
		MessageBox(_T("目标文件不是一个可执行文件！"));
		return;
	}
	//计算PE偏移距离
	BYTE temp1[]={
		pData[63],
		pData[62],
		pData[61],
		pData[60]
	};
	int PE = bytesToInt(temp1);
	
	
	//判断指定位置是否有PE头
	if (pData[PE] != 'P' || pData[PE + 1] != 'E')
	{
		MessageBox(_T("没有在目标文件中找到PE头！"));
		return;
	}

	
	//计算数字签名位置
	byte temp2[] = {
		pData[PE + 152 + 3],
		pData[PE + 152 + 2],
		pData[PE + 152 + 1],
		pData[PE + 152 + 0]
	};
	int ds = bytesToInt(temp2);
	if(ds != 0)
	{
		MessageBox(_T("目标文件已经存在数字签名！"));
		return;
	}
	//复制数字签名到可执行文件最后位置
	for (int i = 0; i < dsSize; i++)
	{
		pData[dwSize + i] = m_byte[i];
	}
	//修复可执行文件
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
	MessageBox(_T("复制成功！"));
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

	//打开文件，并获得数据
	CString strFile;
	GetDlgItemText(IDC_EDIT1,strFile);
	CFile file1(strFile,CFile::modeRead);
	if (!file1.m_hFile)
	{
		SetDlgItemText(IDC_STATIC_LOG,_T("打开原始文件失败！"));
		return FALSE;
	}
	DWORD dwSize = file1.GetLength();
	BYTE * pData = new BYTE[dwSize];
	file1.Read(pData,dwSize);
	file1.Close();

	
	//判断是否是可执行文件
	if(pData[0] != 'M' || pData[1] != 'Z')
	{
		MessageBox(_T("原始文件不是一个可执行文件！"));
		return FALSE;
	}
	//计算PE偏移距离
	BYTE temp1[]={
		pData[63],
		pData[62],
		pData[61],
		pData[60]
	};
	int PE = bytesToInt(temp1);
	
	
	//判断指定位置是否有PE头
	if (pData[PE] != 'P' || pData[PE + 1] != 'E')
	{
		MessageBox(_T("没有在原始文件中找到PE头！"));
		return FALSE;
	}

	
	//计算数字签名位置
	byte temp2[] = {
		pData[PE + 152 + 3],
		pData[PE + 152 + 2],
		pData[PE + 152 + 1],
		pData[PE + 152 + 0]
	};
	int ds = bytesToInt(temp2);
	
	//计算数字签名大小
	int dsSize = pData[PE + 152 + 4];
	dsSize |= ((pData[PE + 152 + 5] << 8) & 0xFF00);
	if(dsSize == 0)
	{
		MessageBox(_T("没有在原始文件中找到数字签名！"));
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
