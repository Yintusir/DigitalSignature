
// DigitalSignature.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDigitalSignatureApp:
// �йش����ʵ�֣������ DigitalSignature.cpp
//

class CDigitalSignatureApp : public CWinApp
{
public:
	CDigitalSignatureApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDigitalSignatureApp theApp;