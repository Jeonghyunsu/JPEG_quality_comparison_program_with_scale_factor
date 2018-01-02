
// MFC_JPEGDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC_JPEG.h"
#include "MFC_JPEGDlg.h"
#include "afxdialogex.h"

extern "C" double JPEG_TEST(const char* oriFilePath, char* oriFileName, char* Ext, int scale_factor);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_JPEGDlg ��ȭ ����



CMFC_JPEGDlg::CMFC_JPEGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_JPEG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_JPEGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC_JPEGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILEOPEN, &CMFC_JPEGDlg::OnFileopenMenuClick)
	ON_STN_CLICKED(IDC_PIC1, &CMFC_JPEGDlg::OnStnClickedPic1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC_JPEGDlg::QInputBtnClick)
END_MESSAGE_MAP()


// CMFC_JPEGDlg �޽��� ó����

BOOL CMFC_JPEGDlg::OnInitDialog()
{
	this->ModifyStyle(NULL, WS_THICKFRAME);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetLimitText(3);
	memset(oriFilePath, '\0', 256);

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMFC_JPEGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFC_JPEGDlg::OnPaint()
{
	if (IsIconic())
	{

		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialogEx::OnPaint(); 
		if (srcFlag) {
			DisplayImage(IDC_PIC1, srcMat);
		}if (dstFlag) {
			DisplayImage(IDC_PIC2, dstMat);
		}
		
	}
}

HCURSOR CMFC_JPEGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_JPEGDlg::OnFileopenMenuClick()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	TCHAR szFilter[] = _T("Image (*.BMP, *.JPG) | *.BMP;*.JPG; | All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		cstrImgPath = dlg.GetPathName();
		CString cstrImgName = dlg.GetFileName();
		CString cstrExt = dlg.GetFileExt();

		String _cstrImgPath = (LPSTR)(LPCTSTR)cstrImgPath;
		String _cstrImgName = (LPSTR)(LPCTSTR)cstrImgName;
		String _cstrExt = (LPSTR)(LPCTSTR)cstrExt;

		srcMat = imread(_cstrImgPath);
		strcpy(oriFilePath, _cstrImgPath.c_str());
		strcpy(oriFileName, _cstrImgName.c_str());
		strcpy(Ext, _cstrExt.c_str());

		srcFlag = true;
		dstFlag = false;
		dstFileSize = 0;

		Invalidate(true);
	}
}

void CMFC_JPEGDlg::DisplayImage(int IDC_PIC_CONTROL, Mat srcMat)
{
	if (cstrImgPath != "") {
		IplImage* srcIplImage = new IplImage(srcMat);

		float size = 0.f;
		
		CWnd* pWnd_pictureControl = GetDlgItem(IDC_PIC_CONTROL);
		CClientDC dcImageTraget(pWnd_pictureControl);
		RECT rcPicControl;	
		pWnd_pictureControl->GetClientRect(&rcPicControl);

		float fImageRatio = float(srcMat.cols) / float(srcMat.rows);
		float fRectRatio = float(rcPicControl.right) / float(rcPicControl.bottom);
		float fScaleFactor;
		if (fImageRatio < fRectRatio) {
			fScaleFactor = float(rcPicControl.bottom) / float(srcMat.rows);	
			int rightWithRatio = srcMat.cols * fScaleFactor;
			float halfOfDif = ((float)rcPicControl.right - (float)rightWithRatio) / (float)2;
			rcPicControl.left = halfOfDif;
			rcPicControl.right = rightWithRatio;
		}
		else {
			fScaleFactor = float(rcPicControl.right) / float(srcMat.cols);
			int bottomWithRatio = srcMat.rows * fScaleFactor;
			float halfOfDif = ((float)rcPicControl.bottom - (float)bottomWithRatio) / (float)2;
			rcPicControl.top = halfOfDif;
			rcPicControl.bottom = bottomWithRatio;
		}

		BITMAPINFO bitmapInfo;
		memset(&bitmapInfo, 0, sizeof(bitmapInfo));
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = srcIplImage->width;
		bitmapInfo.bmiHeader.biHeight = -srcIplImage->height;

		IplImage *tempImage = nullptr;

		if (srcIplImage->nChannels == 1)
		{
			tempImage = cvCreateImage(cvSize(srcIplImage->width, srcIplImage->height), IPL_DEPTH_8U, 3);
			cvCvtColor(srcIplImage, tempImage, CV_GRAY2BGR);
		}
		else if (srcIplImage->nChannels == 3)
		{
			tempImage = cvCloneImage(srcIplImage);
		}

		bitmapInfo.bmiHeader.biBitCount = tempImage->depth * tempImage->nChannels;

		
		dcImageTraget.SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(dcImageTraget.GetSafeHdc(),
			rcPicControl.left, rcPicControl.top, rcPicControl.right, rcPicControl.bottom,
			0, 0, tempImage->width, tempImage->height, tempImage->imageData,
			&bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
			
		cvReleaseImage(&tempImage);
	}
}

void CMFC_JPEGDlg::OnStnClickedPic1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMFC_JPEGDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}


void CMFC_JPEGDlg::QInputBtnClick()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (oriFilePath[0] == '\0') {
		MessageBox(_T("�Է� ������ �־��ּ���."), _T("Warning"), MB_ICONWARNING);
		return;
	}
	UpdateData(TRUE);
	CString scaleStr;
	GetDlgItemText(IDC_EDIT1, scaleStr);

	int scale_factor = atoi(scaleStr);
	if (scale_factor <= 0 || scaleStr=="") {
		MessageBox(_T("������ �´� Q�� �־��ּ���."), _T("Warning"), MB_ICONWARNING);
		return;
	}

	if (!strcmp(Ext, "bmp")) {
		char strFirstFileName[256] = "./";
		char strFirstDecFileName[256] = "./";

		strncat(strFirstFileName, oriFileName, strlen(oriFileName) - 3);
		strncat(strFirstFileName, "jpg", 3);

		strncat(strFirstDecFileName, oriFileName, strlen(oriFileName) - 4);
		strncat(strFirstDecFileName, "_Dec.bmp", 8);

		double PSNR = JPEG_TEST(oriFilePath, oriFileName, Ext, scale_factor);
		char PSNR_str[20];
		sprintf(PSNR_str, "%.2f db", PSNR);
		SetDlgItemText(IDC_PSNR, PSNR_str);

		dstMat = imread(strFirstFileName);

		FILE* fp;
		fopen_s(&fp, cstrImgPath, "r");
		fseek(fp, 0, SEEK_END);
		int nFileSize1 = ftell(fp);
		fclose(fp);

		fopen_s(&fp, strFirstFileName, "r");
		fseek(fp, 0, SEEK_END);
		int nFileSIze2 = ftell(fp);
		fclose(fp);

		CString strTemp1, strTemp2;
		strTemp1.Format("%d", nFileSize1);
		strTemp2.Format("%d", nFileSIze2);

		SetDlgItemText(IDC_FILESIZE1, strTemp1);
		SetDlgItemText(IDC_FILESIZE2, strTemp2);

		printf("Original Image Size : %d\n", nFileSize1);
		printf("Compression Image Size : %d\n", nFileSIze2);
		printf("PSNR : %.2f db\n\n", PSNR);


		dstFlag = true;
		srcFlag = false;
		Invalidate(false);
	}
	if (!strcmp(Ext, "jpg")) {
		char strFirstDecFileName[256] = "./";
		char strFirstFileName[256] = "./";
		char strSecondDecFileName[256] = "./";

		strncat(strFirstDecFileName, oriFileName, strlen(oriFileName) - 4);
		strncat(strFirstDecFileName, "_Dec.bmp", 8);

		strncat(strFirstFileName, strFirstDecFileName, strlen(strFirstDecFileName) - 7);
		strncat(strFirstFileName, "Re.jpg", 6);

		strncat(strSecondDecFileName, strFirstDecFileName, strlen(strFirstDecFileName) - 4);
		strncat(strSecondDecFileName, "2.bmp", 5);


		double PSNR = JPEG_TEST(oriFilePath, oriFileName, Ext, scale_factor);
		char PSNR_str[20];
		sprintf(PSNR_str, "%.2f db", PSNR);
		SetDlgItemText(IDC_PSNR, PSNR_str);


		dstMat = imread(strSecondDecFileName);

		FILE* fp;
		fopen_s(&fp, cstrImgPath, "r");
		fseek(fp, 0, SEEK_END);
		int nFileSize1 = ftell(fp);
		fclose(fp);

		fopen_s(&fp, strFirstFileName, "r");
		fseek(fp, 0, SEEK_END);
		int nFileSIze2 = ftell(fp);
		fclose(fp);

		CString strTemp1, strTemp2;
		strTemp1.Format("%d bytes", nFileSize1);
		strTemp2.Format("%d bytes", nFileSIze2);

		SetDlgItemText(IDC_FILESIZE1, strTemp1);
		SetDlgItemText(IDC_FILESIZE2, strTemp2);

		printf("Original Image Size : %d bytes\n", nFileSize1);
		printf("Compression Image Size : %d bytes\n", nFileSIze2);
		printf("PSNR : %.2f db\n\n", PSNR);


		dstFlag = true;
		srcFlag = false;
		Invalidate(true);

	}
}


