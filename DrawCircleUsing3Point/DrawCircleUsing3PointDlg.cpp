
// drawCircleUsing3PointDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "drawCircleUsing3Point.h"
#include "drawCircleUsing3PointDlg.h"
#include "afxdialogex.h"

#include "CImageDlg.h"
#include "CImgDataHdr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CDrawCircleUsing3PointDlg dialog



CDrawCircleUsing3PointDlg::CDrawCircleUsing3PointDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWCIRCLEUSING3POINT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawCircleUsing3PointDlg::_initDlg()
{
	_initGuiCtrlValue(); 

	MoveWindow(0,0,MAIN_DLG_WIDTH, MAIN_DLG_HEIGHT); 

	_cImgDlgPtr = _createImageDlg(); 
	if(_cImgDlgPtr == nullptr)
	{ ASSERT(false); return; }
	
	_cImgDlgPtr->MoveWindow(0,0,_stDrawInfo._iImgDlgWidth, _stDrawInfo._iImgDlgHeight); 

	//init drawing info; 
	_initPointVec(_stDrawInfo, _stDrawInfo._iPtCnt); 

	_cImgDataHdrPtr = _createImgDataHdr(); 
	if(_cImgDataHdrPtr == nullptr)
	{ ASSERT(false); return; }

	return; 
}

void CDrawCircleUsing3PointDlg::_deinitDlg()
{
	
	_deleteImgDataHdr(_cImgDataHdrPtr); 
	_cImgDataHdrPtr = nullptr; 

	_deleteImageDlg(_cImgDlgPtr); 
	_cImgDlgPtr = nullptr; 

	_deinitPointVec(_stDrawInfo, 0); 

	return; 
}

void CDrawCircleUsing3PointDlg::_initGuiCtrlValue()
{
	CWnd* cwndPtr = nullptr;
	TCHAR tchTempValue[64] = { 0 };

	//point circle radius
	cwndPtr = GetDlgItem(IDC_EDIT_PT_CIRCLE_RADIUS);
	if (cwndPtr)
	{
		wsprintf(tchTempValue, _T("%d"), _stDrawInfo._iCircleRadius);
		cwndPtr->SetWindowText(tchTempValue);
	}

	//outer circle line size
	cwndPtr = GetDlgItem(IDC_EDIT_OUTER_CIRCLE_LINE_SIZE);
	if (cwndPtr)
	{
		wsprintf(tchTempValue, _T("%d"), _stDrawInfo._iOuterCircleLineSize);
		cwndPtr->SetWindowText(tchTempValue);
	}
	
	return; 
}

void CDrawCircleUsing3PointDlg::_getGuiCtrlValue()
{
	CWnd* cwndPtr = nullptr;
	TCHAR tchTempValue[128] = { 0 };
	int iTempValue = 0; 
	//point circle radius
	cwndPtr = GetDlgItem(IDC_EDIT_PT_CIRCLE_RADIUS);
	if (cwndPtr)
	{
		cwndPtr->GetWindowText(tchTempValue, 128);
		iTempValue = _ttoi(tchTempValue);
		if ((iTempValue >= CIRCLE_RADIUS_MIN) && (iTempValue <= CIRCLE_RADIUS_MAX))
			_stDrawInfo._iCircleRadius = iTempValue;
		else
		{
			wsprintf(tchTempValue, _T("Point Circle available range value is %d~%d\nPlease input again. "), 
				CIRCLE_RADIUS_MIN, CIRCLE_RADIUS_MAX);
			AfxMessageBox(tchTempValue);
			wsprintf(tchTempValue, _T("%d"), _stDrawInfo._iCircleRadius);
			cwndPtr->SetWindowText(tchTempValue);
		}
	}
	//outer circle line size
	cwndPtr = GetDlgItem(IDC_EDIT_OUTER_CIRCLE_LINE_SIZE);
	if (cwndPtr)
	{
		cwndPtr->GetWindowText(tchTempValue, 128);
		iTempValue = _ttoi(tchTempValue);
		if ((iTempValue >= OUTER_CIRCLE_LINE_SIZE_MIN) && (iTempValue <= OUTER_CIRCLE_LINE_SIZE_MAX))
			_stDrawInfo._iOuterCircleLineSize = iTempValue;
		else
		{
			wsprintf(tchTempValue, _T("Outer Circle Line Size available range value is %d~%d\nPlease input again. "), 
				OUTER_CIRCLE_LINE_SIZE_MIN, OUTER_CIRCLE_LINE_SIZE_MAX);
			AfxMessageBox(tchTempValue);
			wsprintf(tchTempValue, _T("%d"), _stDrawInfo._iOuterCircleLineSize);
			cwndPtr->SetWindowText(tchTempValue);
		}		
	}
	return;
}

CImageDlg* CDrawCircleUsing3PointDlg::_createImageDlg()
{
	CImageDlg* cImgDlgPtr = new CImageDlg(this); 
	if (cImgDlgPtr)
	{
		cImgDlgPtr->_setDrawInfo(&_stDrawInfo); 

		cImgDlgPtr->Create(IDD_DIALOG_IMAGE, this); 
		cImgDlgPtr->ShowWindow(SW_SHOW); 
	}	
	
	return cImgDlgPtr;
}

void CDrawCircleUsing3PointDlg::_deleteImageDlg(CImageDlg* cImgDlgPtr)
{
	if(cImgDlgPtr == nullptr)
		return; 

	cImgDlgPtr->OnBnClickedOk(); 

	SAFE_DELETE_PTR(cImgDlgPtr); 

	return; 
}

void CDrawCircleUsing3PointDlg::_initPointVec(StDrawInfo& stDrawInfoRef,  size_t szVecSize)
{
	std::vector<CPoint>& vecPointRef = stDrawInfoRef._vecPoint; 

	if (vecPointRef.size() != szVecSize)
		vecPointRef.resize(szVecSize);

	_clearPointInfo(vecPointRef); 

	return; 
}

void CDrawCircleUsing3PointDlg::_deinitPointVec(StDrawInfo& stDrawInfoRef, size_t szVecSize)
{
	std::vector<CPoint>& vecPointRef = stDrawInfoRef._vecPoint; 	

	if (vecPointRef.size() != szVecSize)
		vecPointRef.resize(szVecSize);

	return; 
}

void CDrawCircleUsing3PointDlg::_clearPointInfo(std::vector<CPoint>& vecPointRef)
{
	size_t szVecSize = vecPointRef.size(); 

	for (int i = 0; i < szVecSize; i++)
	{
		CPoint& cPointRef = vecPointRef[i]; 
		cPointRef = 0; 
	}

	return; 
}

bool CDrawCircleUsing3PointDlg::_addPointInfo(CPoint& cPoint)
{
	bool bRet = false;
	std::vector<CPoint>& vecPointRef = _stDrawInfo._vecPoint;

	_stDrawInfo._bUpdateImg = false;
	size_t szVecSize = vecPointRef.size(); 
	for (int i = 0; i < szVecSize; i++)
	{
		CPoint& cPtRef = vecPointRef[i]; 
		if ((cPtRef.x == 0) && (cPtRef.y == 0))
		{			
			cPtRef.SetPoint(cPoint.x, cPoint.y);		
			
			_cImgDataHdrPtr->_markPointCircle(cPtRef, IMAGE_DATA_SET_VALUE); 

			if (i == 2)
				_cImgDataHdrPtr->_mark3PointOuterCircle(IMAGE_DATA_SET_VALUE);

			bRet = true; 
			break; 
		}
	}

	_stDrawInfo._bUpdateImg = true;

	if(_cImgDlgPtr)
		_cImgDlgPtr->_updateImage(); 

	return bRet;
}

int CDrawCircleUsing3PointDlg::_getPtClicked(CPoint& cPoint)
{
	return _cImgDataHdrPtr->_getPtIdxClicked(cPoint); 
}

void CDrawCircleUsing3PointDlg::_uptPointInfo(CPoint& cPoint, int iSelPtIdx)
{
	if (iSelPtIdx == INVALID_SEL_PT_IDX)
		return;

	std::vector<CPoint>& vecPointRef = _stDrawInfo._vecPoint;
	CPoint& cPtRef = vecPointRef[iSelPtIdx];

	bool bUpdate = (cPtRef != cPoint) ? true : false;
	
	if (bUpdate)
	{
		_stDrawInfo._bUpdateImg = false;

		//erase image data
		_setImageData(_stDrawInfo, IMAGE_DATA_RESET_VALUE); 

		cPtRef = cPoint; 

		//update image data
		_setImageData(_stDrawInfo, IMAGE_DATA_SET_VALUE); 

		_stDrawInfo._bUpdateImg = true;

		//update image dlg
		if (_cImgDlgPtr)
			_cImgDlgPtr->_updateImage();
	}
	
	return;
}

void CDrawCircleUsing3PointDlg::_setImageData(StDrawInfo & stDrawInfoRef, byte byMarkValue)
{
	std::vector<CPoint>& vecPointRef = _stDrawInfo._vecPoint;

	size_t szVecSize = vecPointRef.size(); 
	for (int i = 0; i < szVecSize; i++)
	{
		CPoint& cPtRef = vecPointRef[i]; 
		if ((cPtRef.x != 0) || (cPtRef.y != 0))
		{		
			_cImgDataHdrPtr->_markPointCircle(cPtRef, byMarkValue); 
			
			if (i == 2)
				_cImgDataHdrPtr->_mark3PointOuterCircle(byMarkValue);
		}
	}

	return; 
}

void CDrawCircleUsing3PointDlg::_movePtRandomPos()
{
	_stDrawInfo._bUpdateImg = false;

	//erase image data
	_setImageData(_stDrawInfo, IMAGE_DATA_RESET_VALUE); 

	std::vector<CPoint>& vecPointRef = _stDrawInfo._vecPoint;
	size_t szVecSize = vecPointRef.size();
	
	int iRandomX = 0; 	
	int iRandomY = 0; 
		
	for (int i = 0; i < szVecSize; i++)
	{
		iRandomX = rand() % (_stDrawInfo._iImgDlgWidth - _stDrawInfo._iCircleRadius);
		iRandomY = rand() % (_stDrawInfo._iImgDlgHeight - _stDrawInfo._iCircleRadius);

		//Set point
		CPoint& cPtRef = vecPointRef[i];
		cPtRef.x = iRandomX;
		cPtRef.y = iRandomY;
	}
	
	//update image data
	_setImageData(_stDrawInfo, IMAGE_DATA_SET_VALUE); 

	_stDrawInfo._bUpdateImg = true;

	//update image dlg
	if (_cImgDlgPtr)
		_cImgDlgPtr->_updateImage();

	return;	
}

CImgDataHdr* CDrawCircleUsing3PointDlg::_createImgDataHdr()
{
	CImgDataHdr* cImgDataHdrPtr = new CImgDataHdr(); 

	if (cImgDataHdrPtr)
	{
		//Set draw info 
		cImgDataHdrPtr->_setDrawInfoPtr(&_stDrawInfo); 
	}	

	return cImgDataHdrPtr;
}

void CDrawCircleUsing3PointDlg::_deleteImgDataHdr(CImgDataHdr* cImgDataHdrPtr)
{
	SAFE_DELETE_PTR(cImgDataHdrPtr); 
	return; 
}

bool CDrawCircleUsing3PointDlg::_chk3PtInput()
{
	std::vector<CPoint>& vecPointRef = _stDrawInfo._vecPoint;
	size_t szVecSize = vecPointRef.size();

	if (szVecSize != _stDrawInfo._iPtCnt)
		return false;

	for (int i = 0; i < szVecSize; i++)
	{
		CPoint& cPtRef = vecPointRef[i];
		if ((cPtRef.x == 0) || (cPtRef.y == 0))
			return false; 
	}

	return true;
}

void CDrawCircleUsing3PointDlg::_ctrlGui(bool bEnable)
{
	CWnd* cwndPtr = nullptr; 

	//Refresh button
	cwndPtr = GetDlgItem(IDC_BUTTON_REFRESH);
	if (cwndPtr)
		cwndPtr->EnableWindow(bEnable);

	//point circle radius
	cwndPtr = GetDlgItem(IDC_EDIT_PT_CIRCLE_RADIUS);
	if (cwndPtr)
		cwndPtr->EnableWindow(bEnable);

	//outer circle line size
	cwndPtr = GetDlgItem(IDC_EDIT_OUTER_CIRCLE_LINE_SIZE);
	if (cwndPtr)
		cwndPtr->EnableWindow(bEnable);

	//Random move button
	cwndPtr = GetDlgItem(IDC_BUTTON_RANDOM_MOVE);
	if (cwndPtr)
		cwndPtr->EnableWindow(bEnable);

	return; 
}

void CDrawCircleUsing3PointDlg::_startThMoveRandomPos()
{
	_bRunTh = true; 

	_thMoveRandomPosPtr = new std::thread(&CDrawCircleUsing3PointDlg::_threadMoveRandomPos, this); 

	return; 
}

void CDrawCircleUsing3PointDlg::_stopThMoveRandomPos()
{
	_bRunTh = false;

	Sleep(100);

	if (_thMoveRandomPosPtr)
	{
		_thMoveRandomPosPtr->join();
		SAFE_DELETE_PTR(_thMoveRandomPosPtr);
	}

	return; 
}

void CDrawCircleUsing3PointDlg::_threadMoveRandomPos()
{
	printf("Thread start\n");
	
	_thProcMoveRandomPos();
	
	printf("Thread end\n");
	return; 
}

void CDrawCircleUsing3PointDlg::_thProcMoveRandomPos()
{
	int iRandomCnt = 0; 

	ULONGLONG ulJobPreStartTick = 0;
	ULONGLONG ulJobCurrentTick = 0;

	while (_bRunTh)
	{
		ulJobCurrentTick = GetTickCount64();
		if (ulJobCurrentTick - ulJobPreStartTick < 500)
		{
			Sleep(100); 
			continue;
		}

		ulJobPreStartTick = ulJobCurrentTick;
		_movePtRandomPos();
		iRandomCnt++;

		if(iRandomCnt >= 10)
			break; 
	}

	return; 
}

void CDrawCircleUsing3PointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawCircleUsing3PointDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDrawCircleUsing3PointDlg::OnBnClickedOk)	
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CDrawCircleUsing3PointDlg::OnBnClickedButtonRefresh)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_MOVE, &CDrawCircleUsing3PointDlg::OnBnClickedButtonRandomMove)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_MOVE_TH, &CDrawCircleUsing3PointDlg::OnBnClickedButtonRandomMoveTh)	
END_MESSAGE_MAP()


// CDrawCircleUsing3PointDlg message handlers

BOOL CDrawCircleUsing3PointDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	_initDlg(); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDrawCircleUsing3PointDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDrawCircleUsing3PointDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDrawCircleUsing3PointDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDrawCircleUsing3PointDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
 	ShowWindow(SW_HIDE); 

	_deinitDlg(); 
	CDialogEx::OnOK();
}

void CDrawCircleUsing3PointDlg::OnBnClickedButtonRefresh()
{
	// TODO: Add your control notification handler code here
	
	_stDrawInfo._bUpdateImg = false;

	//reset cimage's image data
	_cImgDlgPtr->_resetImageData(IMAGE_DATA_RESET_VALUE); 

	//reset point info
	_clearPointInfo(_stDrawInfo._vecPoint);

	_stDrawInfo._bUpdateImg = true;

	//invalidate image dlg 
	_cImgDlgPtr->_updateImage(); 

	_getGuiCtrlValue(); 

	return; 
}

void CDrawCircleUsing3PointDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE); 

	_deinitDlg(); 

	CDialogEx::OnClose();
}

BOOL CDrawCircleUsing3PointDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		// avoid exit
 		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			//case VK_DELETE:
		case VK_CANCEL:
		//case VK_SPACE:
			return TRUE;
			break;
		}
		break;
	}	

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{// avoid ALT F4
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDrawCircleUsing3PointDlg::OnBnClickedButtonRandomMove()
{
	// TODO: Add your control notification handler code here
	
	if (_chk3PtInput() == false)
		return;

	_movePtRandomPos();

	return; 
}

void CDrawCircleUsing3PointDlg::OnBnClickedButtonRandomMoveTh()
{
	// TODO: Add your control notification handler code here

	if (_chk3PtInput() == false)
		return;

	if (_bRunTh == false)
	{//start thread
		_startThMoveRandomPos();

		CWnd* cwndPtr = GetDlgItem(IDC_BUTTON_RANDOM_MOVE_TH);
		if (cwndPtr)
			cwndPtr->SetWindowText(_T("Stop Thread"));

		_ctrlGui(false); 
	}
	else
	{//stop thread
		_stopThMoveRandomPos(); 

		CWnd* cwndPtr = GetDlgItem(IDC_BUTTON_RANDOM_MOVE_TH);
		if (cwndPtr)
			cwndPtr->SetWindowText(_T("Run Thread Move Random Pos"));

		_ctrlGui(true); 
	}
	

	return; 
}
