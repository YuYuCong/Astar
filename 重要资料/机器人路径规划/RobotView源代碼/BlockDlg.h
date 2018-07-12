#if !defined(AFX_BLOCKDLG_H__8196201A_05AE_4073_B038_435C1C347BD3__INCLUDED_)
#define AFX_BLOCKDLG_H__8196201A_05AE_4073_B038_435C1C347BD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlockDlg dialog

class CBlockDlg : public CDialog
{
// Construction
public:
	CBlockDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlockDlg)
	enum { IDD = IDD_DIALOG2 };
	int		m_height;
	int		m_width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKDLG_H__8196201A_05AE_4073_B038_435C1C347BD3__INCLUDED_)
