// BlockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RobotView.h"
#include "BlockDlg.h"
#include "BlockView.h"
#include "RobotViewView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockDlg dialog


CBlockDlg::CBlockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockDlg)
	m_height = 0;
	m_width = 0;
	//}}AFX_DATA_INIT
}


void CBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockDlg)
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockDlg, CDialog)
	//{{AFX_MSG_MAP(CBlockDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockDlg message handlers

void CBlockDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	UpdateData(true);
	CRobotViewApp* myapp=(CRobotViewApp*)AfxGetApp();
    CMainFrame* mainframe=(CMainFrame*)myapp->m_pMainWnd;
    CRobotViewView * treeview=(CRobotViewView *) mainframe->m_wndSplitter_Vertical.GetPane(0,0);
	treeview->read(m_height,m_width);
	mainframe->flag=false;

}
