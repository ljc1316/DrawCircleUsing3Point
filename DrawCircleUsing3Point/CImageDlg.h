#pragma once
#include "afxdialogex.h"


class CDrawCircleUsing3PointDlg; 
struct StDrawInfo; 

// CImageDlg dialog

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CImageDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMAGE };
#endif

	void _initDlg(); 
	void _deinitDlg();

	void _resetImageData(byte byResetValue); 	
	void _updateImage(); 
	void _setPtInfoText(CDC& cDCRef, StDrawInfo* stDrInfoPtr);

	StDrawInfo* _stDrawInfoPtr = nullptr; 	

	void _setDrawInfo(StDrawInfo* stDrawInfoPtr); 	

	static RGBQUAD _stRgbQuadBpp8[RGB_CLR_TBL_SIZE_BPP_8]; 
	static bool _bInitBpp8;
	void _initColorTableBpp8(RGBQUAD stRgbQuad[]);

	CImage _cImage; 

	CDrawCircleUsing3PointDlg* _cMainDlgPtr = nullptr; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
