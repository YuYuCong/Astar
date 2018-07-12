// RobotView.h : main header file for the ROBOTVIEW application
//

#if !defined(AFX_ROBOTVIEW_H__C3A9D1CE_3126_47DA_B916_9328C8085EAE__INCLUDED_)
#define AFX_ROBOTVIEW_H__C3A9D1CE_3126_47DA_B916_9328C8085EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRobotViewApp:
// See RobotView.cpp for the implementation of this class
//

class CRobotViewApp : public CWinApp
{
public:
	CRobotViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRobotViewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTVIEW_H__C3A9D1CE_3126_47DA_B916_9328C8085EAE__INCLUDED_)
