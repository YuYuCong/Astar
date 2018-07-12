// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0061E59E_E08F_4A40_A6DB_AF525D585F84__INCLUDED_)
#define AFX_MAINFRM_H__0061E59E_E08F_4A40_A6DB_AF525D585F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Pathplan.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	int point;//判断是哪个事件需要响应。如画矩形1，还是画圆2
	BOOL flag;//判断矩形或圆是否已经画了，决定菜单栏里响应图标是否可以被选
	CSplitterWnd m_wndSplitter_Horizontal;
	CSplitterWnd m_wndSplitter_Vertical;
	int M[HMAX][WMAX];
	int VirtualMap[HMAX][WMAX];
	path best;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRect();
	afx_msg void OnUpdateRect(CCmdUI* pCmdUI);
	afx_msg void OnRobotPoint();
	afx_msg void OnUpdateRobotPoint(CCmdUI* pCmdUI);
	afx_msg void OnGoalPoint();
	afx_msg void OnUpdateGoalPoint(CCmdUI* pCmdUI);
	afx_msg void OnBegin();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0061E59E_E08F_4A40_A6DB_AF525D585F84__INCLUDED_)
