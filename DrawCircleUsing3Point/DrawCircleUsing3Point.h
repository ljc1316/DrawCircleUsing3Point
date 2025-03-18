
// drawCircleUsing3Point.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CdrawCircleUsing3PointApp:
// See drawCircleUsing3Point.cpp for the implementation of this class
//

class CdrawCircleUsing3PointApp : public CWinApp
{
public:
	CdrawCircleUsing3PointApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CdrawCircleUsing3PointApp theApp;
