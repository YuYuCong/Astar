// RobotViewDoc.cpp : implementation of the CRobotViewDoc class
//

#include "stdafx.h"
#include "RobotView.h"

#include "RobotViewDoc.h"
#include "BlockView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotViewDoc

IMPLEMENT_DYNCREATE(CRobotViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CRobotViewDoc, CDocument)
	//{{AFX_MSG_MAP(CRobotViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotViewDoc construction/destruction

CRobotViewDoc::CRobotViewDoc()
{
	// TODO: add one-time construction code here

}

CRobotViewDoc::~CRobotViewDoc()
{
}

BOOL CRobotViewDoc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRobotViewDoc serialization

void CRobotViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRobotViewDoc diagnostics

#ifdef _DEBUG
void CRobotViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRobotViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotViewDoc commands
