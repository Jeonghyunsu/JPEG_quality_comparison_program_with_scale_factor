
// MFC_JPEGDlg.h : 헤더 파일
//

#pragma once

#include <atlimage.h>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

// CMFC_JPEGDlg 대화 상자
class CMFC_JPEGDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFC_JPEGDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_JPEG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
