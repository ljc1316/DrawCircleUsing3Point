#pragma once

struct StDrawInfo; 

class CImgDataHdr  //Image data handler
{
public:
	CImgDataHdr() {}; 
	virtual ~CImgDataHdr() {}

	StDrawInfo* _stDrawInfoPtr = nullptr; 
	inline void _setDrawInfoPtr(StDrawInfo* stDrawInfoPtr) 
	{ _stDrawInfoPtr = stDrawInfoPtr; }

	void _markPointCircle(CPoint& cPtRef, byte byMarkValue); 
	void _mark3PointOuterCircle(byte byMarkValue); 
	bool _isInCircle(int iI, int iJ, int iCenterX, int iCenterY, int iRadius2n);

	bool _calcOuterCircleInfo(StDrawInfo* stDrawInfoPtr); 
	bool _getOuterCircleCenter(_In_ CPoint& cPt1Ref, _In_ CPoint& cPt2Ref, 
		_In_ CPoint& cPt3Ref, _Out_ CPoint& cPtRadiusRef);
	void _markPointCircleOuterLine(CPoint& cPtRef, byte byMarkValue, int iRadius, 
		int iLineSize); 
	bool _isInCircleLine(int iI, int iJ, int iCenterX, int iCenterY, int iRadius, 
		int iRadius2n, int iLineSize);


	int _getDistance(_In_ CPoint& cPt1Ref, _In_ CPoint& cPt2Ref);
	int _getDistance(_In_ int iX1, _In_ int iY1, _In_ int iX2, _In_ int iY2);

	int _getPtIdxClicked(_In_ CPoint& cPt);
};

