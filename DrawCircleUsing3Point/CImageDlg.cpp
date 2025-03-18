// CImageDlg.cpp : implementation file
//

#include "pch.h"
#include "DrawCircleUsing3Point.h"
#include "afxdialogex.h"
#include "CImageDlg.h"

#include "DrawCircleUsing3PointDlg.h"


bool CImageDlg::_bInitBpp8 = false; 
RGBQUAD CImageDlg::_stRgbQuadBpp8[RGB_CLR_TBL_SIZE_BPP_8]; 

// CImageDlg dialog

IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_IMAGE, pParent)
{
	_cMainDlgPtr = (CDrawCircleUsing3PointDlg*)pParent; 
}

CImageDlg::~CImageDlg()
{
}

void CImageDlg::_initDlg()
{ 
	_cImage.Create(_stDrawInfoPtr->_iImgDlgWidth, -_stDrawInfoPtr->_iImgDlgHeight,
		_stDrawInfoPtr->_iImgBpp); 

	//init color table 
	if (_stDrawInfoPtr->_iImgBpp == 8)
	{
		_initColorTableBpp8(_stRgbQuadBpp8);
		_cImage.SetColorTable(0,RGB_CLR_TBL_SIZE_BPP_8,_stRgbQuadBpp8);
		_resetImageData(IMAGE_DATA_RESET_VALUE); 
		if(_stDrawInfoPtr)
		{
			_stDrawInfoPtr->_chImgDataPtr = static_cast<PUCHAR>(_cImage.GetBits()); 
			_stDrawInfoPtr->_iPitch = _cImage.GetPitch(); 
		}
	}

	return; 
}

void CImageDlg::_deinitDlg()
{
}

void CImageDlg::_resetImageData(byte byResetValue)
{
	PUCHAR chFrmDataPtr = static_cast<PUCHAR>(_cImage.GetBits()); 

	memset(chFrmDataPtr, byResetValue, 
		_stDrawInfoPtr->_iImgDlgWidth*_stDrawInfoPtr->_iImgDlgHeight*sizeof(byte)); 

	return; 
}

void CImageDlg::_updateImage()
{
	Invalidate(); 
	return; 
}

void CImageDlg::_setPtInfoText(CDC& cDCRef, StDrawInfo* stDrInfoPtr)
{
	if (stDrInfoPtr == nullptr)
		return;

	CString strPtInfo;
	CPoint cPt;
	std::vector<CPoint>& vecPtRef = stDrInfoPtr->_vecPoint; 

	for (int i = 0; i < stDrInfoPtr->_iPtCnt; i++)
	{
		cPt = vecPtRef[i];
		if ((cPt.x != 0) || (cPt.y != 0))
		{
			strPtInfo.Format(_T("Pt%d : (%d, %d)"), i + 1, cPt.x, cPt.y);
			cDCRef.TextOutW(cPt.x + stDrInfoPtr->_iCircleRadius, cPt.y - 5 , strPtInfo);
		}		
	}
	return;
}

void CImageDlg::_setDrawInfo(StDrawInfo* stDrawInfoPtr)
{
	if (stDrawInfoPtr == nullptr)
	{ ASSERT(false); return; }

	_stDrawInfoPtr = stDrawInfoPtr; 
	return; 
}

void CImageDlg::_initColorTableBpp8(RGBQUAD stRgbQuad [])
{
	if(_bInitBpp8)
		return; 

	_bInitBpp8 = true; 

	for (int i = 0; i < RGB_CLR_TBL_SIZE_BPP_8; i++)
		stRgbQuad[i].rgbRed = stRgbQuad[i].rgbGreen = stRgbQuad[i].rgbBlue = i; 

	return; 
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImageDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CImageDlg message handlers

void CImageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	_initDlg(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	if(_stDrawInfoPtr->_bUpdateImg)
	{
		_cImage.Draw(dc, 0, 0); 	
		_setPtInfoText(dc, _stDrawInfoPtr);
	}

}

void CImageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (_cMainDlgPtr->_bRunTh)
	{
		CDialogEx::OnLButtonDown(nFlags, point);
		return; 
	}

	int iPtSelIdx = _cMainDlgPtr->_getPtClicked(point); 

	if (iPtSelIdx == INVALID_SEL_PT_IDX)
		_cMainDlgPtr->_addPointInfo(point); 
	else
	{
		_stDrawInfoPtr->_iSelPtIdx = iPtSelIdx; 
		_stDrawInfoPtr->_bDragging = true; 
		SetCapture(); 			
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CImageDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (_cMainDlgPtr->_bRunTh)
	{
		CDialogEx::OnLButtonUp(nFlags, point);
		return; 
	}

	if (_stDrawInfoPtr->_bDragging)
	{
		ReleaseCapture();
		_stDrawInfoPtr->_bDragging = false;
		if (_stDrawInfoPtr->_iSelPtIdx != INVALID_SEL_PT_IDX)
		{
			if ((point.x < (_stDrawInfoPtr->_iImgDlgWidth - _stDrawInfoPtr->_iCircleRadius )) &&
				(point.y < (_stDrawInfoPtr->_iImgDlgHeight - _stDrawInfoPtr->_iCircleRadius )))
			{
				if ((point.x > _stDrawInfoPtr->_iCircleRadius) &&
					(point.y > _stDrawInfoPtr->_iCircleRadius))
				{
					if(_cMainDlgPtr)
						_cMainDlgPtr->_uptPointInfo(point, _stDrawInfoPtr->_iSelPtIdx);
				}				
			}					
		}

		_stDrawInfoPtr->_iSelPtIdx = INVALID_SEL_PT_IDX; 
		
		TRACE(_T("OnLButtonUp x = %d, y = %d\n"), point.x, point.y);

	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CImageDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (_cMainDlgPtr->_bRunTh)
	{
		CDialogEx::OnMouseMove(nFlags, point);
		return; 
	}

	if(_stDrawInfoPtr->_bDragging)
	{
		TRACE(_T("OnMouseMove x = %d, y = %d\n"), point.x, point.y);

		if (_stDrawInfoPtr->_iSelPtIdx != INVALID_SEL_PT_IDX)
		{
			if ((point.x < (_stDrawInfoPtr->_iImgDlgWidth - _stDrawInfoPtr->_iCircleRadius )) &&
				(point.y < (_stDrawInfoPtr->_iImgDlgHeight - _stDrawInfoPtr->_iCircleRadius )))
			{
				if ((point.x > _stDrawInfoPtr->_iCircleRadius ) &&
					(point.y > _stDrawInfoPtr->_iCircleRadius ))
				{
					if(_cMainDlgPtr)
						_cMainDlgPtr->_uptPointInfo(point, _stDrawInfoPtr->_iSelPtIdx);
				}
			}			
		}
	}


	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CImageDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE; 	

	return CDialogEx::OnEraseBkgnd(pDC);
}
