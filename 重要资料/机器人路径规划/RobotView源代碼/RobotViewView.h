// RobotViewView.h : interface of the CRobotViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTVIEWVIEW_H__F96A6F40_56DD_45AC_BDF9_BD3C42091D10__INCLUDED_)
#define AFX_ROBOTVIEWVIEW_H__F96A6F40_56DD_45AC_BDF9_BD3C42091D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RobotViewDoc.h"

class CRobotViewView : public CView
{
protected: // create from serialization only
	CRobotViewView();
	DECLARE_DYNCREATE(CRobotViewView)

// Attributes
public:
	CRobotViewDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotViewView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void showGoalPoint(int x,int y);
	int m_goalPointy;
	int m_goalPointx;
	int m_robotPointy;
	int m_robotPointx;

	void showRobotPoint(int x,int y);
	void showBlock(int x1,int y1,int x2,int y2);
	int Width;
	int Height;
	void read(int h,int w);
	virtual ~CRobotViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotViewView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RobotViewView.cpp
inline CRobotViewDoc* CRobotViewView::GetDocument()
   { return (CRobotViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTVIEWVIEW_H__F96A6F40_56DD_45AC_BDF9_BD3C42091D10__INCLUDED_)
