
// MFC_JPEGDlg.h : ��� ����
//

#pragma once

#include <atlimage.h>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

// CMFC_JPEGDlg ��ȭ ����
class CMFC_JPEGDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC_JPEGDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_JPEG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileopenMenuClick();
	afx_msg void OnStnClickedPic1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void QInputBtnClick();

	CString cstrImgPath;

	char oriFilePath[256];
	char oriFileName[256];
	char Ext[20];

	Mat srcMat, dstMat;
	boolean srcFlag = false, dstFlag = false; 
	int srcFileSize, dstFileSize;

	void DisplayImage(int IDC_PICTURE_CONTROL, Mat srcMat);
};
