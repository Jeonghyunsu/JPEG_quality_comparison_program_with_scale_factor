
// MFC_JPEG.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFC_JPEGApp:
// �� Ŭ������ ������ ���ؼ��� MFC_JPEG.cpp�� �����Ͻʽÿ�.
//

class CMFC_JPEGApp : public CWinApp
{
public:
	CMFC_JPEGApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFC_JPEGApp theApp;
