#include "pch.h"
#include "CImgDataHdr.h"

#include "DrawCircleUsing3PointDlg.h"

void CImgDataHdr::_markPointCircle(CPoint& cPtRef, byte byMarkValue)
{
	if((_stDrawInfoPtr == nullptr) || (_stDrawInfoPtr->_chImgDataPtr == nullptr)) return; 

	PUCHAR chDtPtr = _stDrawInfoPtr->_chImgDataPtr; 

	int iCenterX = cPtRef.x; 
	int iCenterY = cPtRef.y; 
	int iRadius = _stDrawInfoPtr->_iCircleRadius; 
	int iPitch = _stDrawInfoPtr->_iPitch; 

	//좌상단 부터 렉트 체크. 
	int iX = iCenterX - iRadius; 
	if(iX < 0) iX = 0; 

	int iY = iCenterY - iRadius; 
	if(iY < 0) iY = 0; 

	int iRadius2n = iRadius*iRadius; 

	for (int j = iY; j < iY + iRadius * 2; j++) 
	{
		for (int i = iX; i < iX + iRadius * 2; i++) 
		{
			if (_isInCircle(i, j, iCenterX, iCenterY, iRadius2n))
				chDtPtr[j*iPitch + i] = byMarkValue;
		}
	}


	return; 
}

void CImgDataHdr::_mark3PointOuterCircle(byte byMarkValue)
{
	if (_calcOuterCircleInfo(_stDrawInfoPtr) == false) return;

	CPoint cpCenter(_stDrawInfoPtr->_iOuterCenterX, _stDrawInfoPtr->_iOuterCenterY); 

	_markPointCircleOuterLine(cpCenter, byMarkValue, _stDrawInfoPtr->_iOuterRadius, 
		_stDrawInfoPtr->_iOuterCircleLineSize); 

	return; 
}

bool CImgDataHdr::_isInCircle(int i, int j, int iCenterX, int iCenterY, int iRadius2n)
{
	bool bRet = false;

	double dX = i - iCenterX;
	double dY = j - iCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < iRadius2n) 
		bRet = true;

	return bRet;
}

struct Point2D
{
	double x;
	double y;
};

bool CImgDataHdr::_calcOuterCircleInfo(StDrawInfo* stDrawInfoPtr)
{
	if (stDrawInfoPtr == nullptr) return false;

	std::vector<CPoint>& vecPointRef = stDrawInfoPtr->_vecPoint;
	if (vecPointRef.size() != 3) return false;

	CPoint& cPt1Ref = vecPointRef[0];
	CPoint& cPt2Ref = vecPointRef[1];
	CPoint& cPt3Ref = vecPointRef[2];
	CPoint cPtCenter; 


	if (_getOuterCircleCenter(cPt1Ref, cPt2Ref, cPt3Ref, cPtCenter) == false) 
		return false;

	int iTempRadius1 = _getDistance(cPt1Ref, cPtCenter);

	//Validate the radius
	int iTempRadius2 = _getDistance(cPt2Ref, cPtCenter);
	int iTempRadius3 = _getDistance(cPt3Ref, cPtCenter);

	int iSumValue = iTempRadius1 + iTempRadius2 + iTempRadius3; 
	int iTempValue = iTempRadius1*3; 
	int iResultValue = abs(iSumValue - iTempValue);
	if (iResultValue > 3) 
		return false;
	
	//Set data
	stDrawInfoPtr->_iOuterCenterX = cPtCenter.x; 
	stDrawInfoPtr->_iOuterCenterY = cPtCenter.y; 
	stDrawInfoPtr->_iOuterRadius = iTempRadius1; 

	return true;
}

bool CImgDataHdr::_getOuterCircleCenter(CPoint& cPt1Ref, CPoint& cPt2Ref, CPoint& cPt3Ref, 
	CPoint& cPtCenterRef)
{
	double A = cPt2Ref.x - cPt1Ref.x;
	double B = cPt2Ref.y - cPt1Ref.y;
	double C = cPt3Ref.x - cPt1Ref.x;
	double D = cPt3Ref.y - cPt1Ref.y;
	double E = A * (cPt1Ref.x + cPt2Ref.x) + B * (cPt1Ref.y + cPt2Ref.y);
	double F = C * (cPt1Ref.x + cPt3Ref.x) + D * (cPt1Ref.y + cPt3Ref.y);
	double G = 2.0 * (A * (cPt3Ref.y - cPt2Ref.y) - B * (cPt3Ref.x - cPt2Ref.x));
	
	if (G == 0)
	{
		cPtCenterRef.x = cPt1Ref.x;
		cPtCenterRef.y = cPt1Ref.y;
	}
	else
	{
		cPtCenterRef.x = (int)((D * E - B * F) / G);
		cPtCenterRef.y = (int)((A * F - C * E) / G);
	}

	return true;
}

void CImgDataHdr::_markPointCircleOuterLine(CPoint& cPtRef, byte byMarkValue,
	int iRadius, int iLineSize)
{
	if((_stDrawInfoPtr == nullptr) || (_stDrawInfoPtr->_chImgDataPtr == nullptr)) return; 

	int iImgWidth = _stDrawInfoPtr->_iImgDlgWidth;
	int iImgHeight = _stDrawInfoPtr->_iImgDlgHeight;

	PUCHAR chDtPtr = _stDrawInfoPtr->_chImgDataPtr; 

	int iCenterX = cPtRef.x; 
	int iCenterY = cPtRef.y; 
	int iPitch = _stDrawInfoPtr->_iPitch; 

	//좌상단 부터 렉트 체크. 
	int iX = iCenterX - iRadius; 
	if(iX < 0) iX = 0; 

	int iY = iCenterY - iRadius; 
	if(iY < 0) iY = 0; 

	int iRadius2n = iRadius*iRadius; 

	for (int j = iY; j < iY + iRadius * 2; j++) 
	{
		for (int i = iX; i < iX + iRadius * 2; i++) 
		{
			if((i < iImgWidth) && (j < iImgHeight))
			{
				if (_isInCircleLine(i, j, iCenterX, iCenterY, iRadius, iRadius2n, iLineSize))
					chDtPtr[j*iPitch + i] = byMarkValue;
			}
		}
	}


	return; 
}

bool CImgDataHdr::_isInCircleLine(int i, int j, int iCenterX, int iCenterY,
	int iRadius, int iRadius2n, int iLineSize)
{
	bool bRet = false;

	double dX = i - iCenterX;
	double dY = j - iCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < iRadius2n) 
	{
		//check distance 
		int iDistance = _getDistance(i, j, iCenterX, iCenterY); 
		int iDistDelta = abs(iRadius - iDistance); 
		if(iDistDelta <= iLineSize)
			bRet = true;
	}

	return bRet;
}

int CImgDataHdr::_getDistance(CPoint& cPt1Ref, CPoint& cPt2Ref)
{
	double dX = cPt1Ref.x - cPt2Ref.x;
	double dY = cPt1Ref.y - cPt2Ref.y;
	double dDist = dX * dX + dY * dY;

	int iDistance = (int)sqrt(dDist);

	return iDistance;
}

int CImgDataHdr::_getDistance(int iX1, int iY1, int iX2, int iY2)
{
	double dX = iX1 - iX2;
	double dY = iY1 - iY2;
	double dDist = dX * dX + dY * dY;

	int iDistance = (int)sqrt(dDist);

	return iDistance;
}

int CImgDataHdr::_getPtIdxClicked(CPoint& cPt)
{
	if (_stDrawInfoPtr == nullptr) return INVALID_SEL_PT_IDX;

	int iPtIdxSel = INVALID_SEL_PT_IDX;

	int iPtRadius = _stDrawInfoPtr->_iCircleRadius;

	std::vector<CPoint>& vecPointRef = _stDrawInfoPtr->_vecPoint; 	

	if (vecPointRef.size() == 0)
		return iPtIdxSel; 

	for (int i = 0; i < vecPointRef.size(); i++)
	{
		CPoint& cPtRef = vecPointRef[i];
		if (cPtRef.x == 0 && cPtRef.y == 0)
			continue;

		int iDist = _getDistance(cPtRef, cPt);
		if (iDist <= iPtRadius)
		{
			iPtIdxSel = i;
			break;
		}
	}


	return iPtIdxSel;
}
