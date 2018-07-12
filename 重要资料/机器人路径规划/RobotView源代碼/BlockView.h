#if !defined(AFX_BLOCKVIEW_H__3D143CCD_109F_4DB6_BA35_9FB5C67A2439__INCLUDED_)
#define AFX_BLOCKVIEW_H__3D143CCD_109F_4DB6_BA35_9FB5C67A2439__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlockView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CBlockView : public CFormView
{
protected:
	CBlockView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBlockView)

// Form Data
public:
	//{{AFX_DATA(CBlockView)
	enum { IDD = IDD_DIALOG1 };
	int		m_0_9;
	int		m_0_8;
	int		m_0_7;
	int		m_0_6;
	int		m_0_5;
	int		m_0_4;
	int		m_0_3;
	int		m_0_2;
	int		m_0_19;
	int		m_0_18;
	int		m_0_17;
	int		m_0_16;
	int		m_0_15;
	int		m_0_14;
	int		m_0_13;
	int		m_0_12;
	int		m_0_11;
	int		m_0_10;
	int		m_0_1;
	int		m_0_0;
	int		m_1_0;
	int		m_1_1;
	int		m_1_10;
	int		m_1_11;
	int		m_1_12;
	int		m_1_13;
	int		m_1_14;
	int		m_1_15;
	int		m_1_16;
	int		m_1_17;
	int		m_1_18;
	int		m_1_19;
	int		m_1_2;
	int		m_1_3;
	int		m_1_4;
	int		m_1_5;
	int		m_1_6;
	int		m_1_7;
	int		m_1_8;
	int		m_1_9;
	int		m_2_0;
	int		m_2_1;
	int		m_2_10;
	int		m_2_11;
	int		m_2_12;
	int		m_2_13;
	int		m_2_14;
	int		m_2_15;
	int		m_2_16;
	int		m_2_17;
	int		m_2_18;
	int		m_2_19;
	int		m_2_2;
	int		m_2_3;
	int		m_2_4;
	int		m_2_5;
	int		m_2_6;
	int		m_2_7;
	int		m_2_8;
	int		m_2_9;
	int		m_3_0;
	int		m_3_1;
	int		m_3_10;
	int		m_3_11;
	int		m_3_12;
	int		m_3_14;
	int		m_3_13;
	int		m_3_15;
	int		m_3_16;
	int		m_3_17;
	int		m_3_18;
	int		m_3_19;
	int		m_3_2;
	int		m_3_3;
	int		m_3_4;
	int		m_3_5;
	int		m_3_6;
	int		m_3_7;
	int		m_3_8;
	int		m_3_9;
	int		m_4_0;
	int		m_4_1;
	int		m_4_10;
	int		m_4_11;
	int		m_4_12;
	int		m_4_13;
	int		m_4_14;
	int		m_4_15;
	int		m_4_16;
	int		m_4_17;
	int		m_4_18;
	int		m_4_2;
	int		m_4_19;
	int		m_4_3;
	int		m_4_4;
	int		m_4_5;
	int		m_4_6;
	int		m_4_7;
	int		m_4_8;
	int		m_4_9;
	int		m_5_0;
	int		m_5_1;
	int		m_5_10;
	int		m_5_11;
	int		m_5_12;
	int		m_5_13;
	int		m_5_14;
	int		m_5_15;
	int		m_5_16;
	int		m_5_17;
	int		m_5_18;
	int		m_5_19;
	int		m_5_2;
	int		m_5_3;
	int		m_5_4;
	int		m_5_5;
	int		m_5_6;
	int		m_5_7;
	int		m_5_8;
	int		m_5_9;
	int		m_6_0;
	int		m_6_1;
	int		m_6_10;
	int		m_6_11;
	int		m_6_12;
	int		m_6_13;
	int		m_6_14;
	int		m_6_15;
	int		m_6_16;
	int		m_6_17;
	int		m_6_18;
	int		m_6_19;
	int		m_6_2;
	int		m_6_3;
	int		m_6_4;
	int		m_6_5;
	int		m_6_6;
	int		m_6_7;
	int		m_6_8;
	int		m_6_9;
	int		m_7_0;
	int		m_7_1;
	int		m_7_10;
	int		m_7_11;
	int		m_7_12;
	int		m_7_13;
	int		m_7_14;
	int		m_7_15;
	int		m_7_16;
	int		m_7_17;
	int		m_7_18;
	int		m_7_19;
	int		m_7_2;
	int		m_7_3;
	int		m_7_4;
	int		m_7_5;
	int		m_7_6;
	int		m_7_7;
	int		m_7_8;
	int		m_7_9;
	int		m_8_0;
	int		m_8_1;
	int		m_8_10;
	int		m_8_11;
	int		m_8_12;
	int		m_8_13;
	int		m_8_14;
	int		m_8_15;
	int		m_8_16;
	int		m_8_17;
	int		m_8_18;
	int		m_8_19;
	int		m_8_2;
	int		m_8_3;
	int		m_8_4;
	int		m_8_5;
	int		m_8_6;
	int		m_8_7;
	int		m_8_8;
	int		m_8_9;
	int		m_9_0;
	int		m_9_1;
	int		m_9_10;
	int		m_9_11;
	int		m_9_12;
	int		m_9_13;
	int		m_9_14;
	int		m_9_15;
	int		m_9_16;
	int		m_9_17;
	int		m_9_18;
	int		m_9_19;
	int		m_9_2;
	int		m_9_3;
	int		m_9_4;
	int		m_9_5;
	int		m_9_6;
	int		m_9_7;
	int		m_9_8;
	int		m_9_9;
	int		m_10_0;
	int		m_10_1;
	int		m_10_10;
	int		m_10_11;
	int		m_10_12;
	int		m_10_13;
	int		m_10_14;
	int		m_10_15;
	int		m_10_16;
	int		m_10_17;
	int		m_10_18;
	int		m_10_19;
	int		m_10_2;
	int		m_10_3;
	int		m_10_4;
	int		m_10_5;
	int		m_10_6;
	int		m_10_7;
	int		m_10_8;
	int		m_10_9;
	int		m_11_0;
	int		m_11_1;
	int		m_11_10;
	int		m_11_11;
	int		m_11_12;
	int		m_11_13;
	int		m_11_14;
	int		m_11_15;
	int		m_11_16;
	int		m_11_17;
	int		m_11_18;
	int		m_11_19;
	int		m_11_2;
	int		m_11_3;
	int		m_11_4;
	int		m_11_5;
	int		m_11_6;
	int		m_11_7;
	int		m_11_8;
	int		m_11_9;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void MToControl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBlockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBlockView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKVIEW_H__3D143CCD_109F_4DB6_BA35_9FB5C67A2439__INCLUDED_)
