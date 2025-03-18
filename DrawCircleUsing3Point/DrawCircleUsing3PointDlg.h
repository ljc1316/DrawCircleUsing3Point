
// drawCircleUsing3PointDlg.h : header file
//

#pragma once

#include <thread>

class CImageDlg; 
class CImgDataHdr; 

struct StDrawInfo
{
	int _iImgDlgWidth = IMAGE_DLG_WIDTH; 
	int _iImgDlgHeight = IMAGE_DLG_HEIGHT; 
	int _iImgBpp = IMAGE_BPP; 

	int _iPtCnt = POINT_CNT; 
	std::vector<CPoint> _vecPoint; 

	int _iCircleRadius = CIRCLE_RADIUS; 

	PUCHAR _chImgDataPtr = nullptr; 
	int _iPitch = 0; 

	int _iOuterCircleLineSize = OUTER_CIRCLE_LINE_SIZE; 

	int _iOuterCenterX = 0; 
	int _iOuterCenterY = 0;
	int _iOuterRadius = 0;

	//Drag 
	int _iSelPtIdx = INVALID_SEL_PT_IDX; 
	bool _bDragging = false; 

	//image update flag
	std::atomic_bool _bUpdateImg = true;
};

// CDrawCircleUsing3PointDlg dialog
class CDrawCircleUsing3PointDlg : public CDialogEx
{
// Construction
public:
	CDrawCircleUsing3PointDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWCIRCLEUSING3POINT_DIALOG };
#endif
	
	void _initDlg(); 
	void _deinitDlg(); 
	void _initGuiCtrlValue(); 
	void _getGuiCtrlValue();

	//CImageDlg
	CImageDlg* _createImageDlg(); 
	void _deleteImageDlg(CImageDlg* cImgDlgPtr); 
	CImageDlg* _cImgDlgPtr = nullptr;  	


	//StDrawInfo
	StDrawInfo _stDrawInfo;		
	void _initPointVec(StDrawInfo & stDrawInfoRef, size_t szVecSize); 
	void _deinitPointVec(StDrawInfo & stDrawInfoRef, size_t szVecSize); 
	void _clearPointInfo(std::vector<CPoint>& vecPointRef); 
	bool _addPointInfo(CPoint& cPoint); 
	int _getPtClicked(CPoint& cPoint);
	void _uptPointInfo(CPoint& cPoint, int iSelPtIdx);
	void _setImageData(StDrawInfo & stDrawInfoRef, byte byMarkValue);
	void _movePtRandomPos();

	//Image data handler 
	CImgDataHdr* _createImgDataHdr(); 
	void _deleteImgDataHdr(CImgDataHdr* cImgDataHdrPtr); 
	CImgDataHdr* _cImgDataHdrPtr = nullptr; 

	bool _chk3PtInput(); 

	bool _bRunTh = false; 
	void _ctrlGui(bool bEnable); 
	void _startThMoveRandomPos(); 
	void _stopThMoveRandomPos();

	std::thread* _thMoveRandomPosPtr = nullptr;
	void _threadMoveRandomPos();
	void _thProcMoveRandomPos();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonRandomMove();
	afx_msg void OnBnClickedButtonRandomMoveTh();	
};
