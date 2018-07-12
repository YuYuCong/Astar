#if !defined(AFX_ROUTE_H__A4D65CBD_8442_4C9B_8FBB_1C4270667CAA__INCLUDED_)
#define AFX_ROUTE_H__A4D65CBD_8442_4C9B_8FBB_1C4270667CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Route.h : header file
//

#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// Route view

class Route : public CView
{
protected:
	Route();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(Route)

// Attributes
public:

// Operations
public:
	
	void DrawRoute();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Route)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~Route();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(Route)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTE_H__A4D65CBD_8442_4C9B_8FBB_1C4270667CAA__INCLUDED_)
