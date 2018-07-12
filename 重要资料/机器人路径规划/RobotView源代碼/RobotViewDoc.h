// RobotViewDoc.h : interface of the CRobotViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTVIEWDOC_H__0C87F5D2_C02E_4B2C_BD37_628BB1F765C3__INCLUDED_)
#define AFX_ROBOTVIEWDOC_H__0C87F5D2_C02E_4B2C_BD37_628BB1F765C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRobotViewDoc : public CDocument
{
protected: // create from serialization only
	CRobotViewDoc();
	DECLARE_DYNCREATE(CRobotViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotViewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTVIEWDOC_H__0C87F5D2_C02E_4B2C_BD37_628BB1F765C3__INCLUDED_)
