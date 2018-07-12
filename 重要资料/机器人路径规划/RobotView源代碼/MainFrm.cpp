// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RobotView.h"
#include "RobotViewView.h"
#include "RobotView.h"
#include "BlockView.h"
#include "BlockDlg.h"
#include "Pathplan.h"
#include "Route.h"
#include "RobotViewDoc.h"


#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_RECT, OnRect)
	ON_UPDATE_COMMAND_UI(ID_RECT, OnUpdateRect)
	ON_COMMAND(ID_ROBOT_POINT, OnRobotPoint)
	ON_UPDATE_COMMAND_UI(ID_ROBOT_POINT, OnUpdateRobotPoint)
	ON_COMMAND(ID_GOAL_POINT, OnGoalPoint)
	ON_UPDATE_COMMAND_UI(ID_GOAL_POINT, OnUpdateGoalPoint)
	ON_COMMAND(ID_BEGIN, OnBegin)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	flag=true;
	for(int i=0;i<HMAX;i++)
		for(int j=0;j<WMAX;j++)
			M[i][j]=0;
	
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!m_wndSplitter_Vertical.CreateStatic(this,1,2)) 	
		return FALSE; 	
	if(!m_wndSplitter_Vertical.CreateView(0,0,RUNTIME_CLASS(CRobotViewView),CSize(WIDTH,HEIGHT), pContext)) 	
		return FALSE; 
	if(!m_wndSplitter_Horizontal.CreateStatic(&m_wndSplitter_Vertical,2,1,WS_CHILD|WS_VISIBLE,m_wndSplitter_Vertical.IdFromRowCol(0,1)))
		return FALSE;
	if(!m_wndSplitter_Horizontal.CreateView(0,0,RUNTIME_CLASS(CBlockView),CSize(300,290),pContext)) 	
		return FALSE; 
	if(!m_wndSplitter_Horizontal.CreateView(1,0,RUNTIME_CLASS(Route),CSize(300,300),pContext)) 	
		return FALSE; 

	return TRUE; 
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnRect() 
{
	// TODO: Add your command handler code here
	CBlockDlg dlg;
	dlg.DoModal();
	point=1;
}

void CMainFrame::OnUpdateRect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(flag);
}

void CMainFrame::OnRobotPoint() 
{
	// TODO: Add your command handler code here
	point=2;
//	flag=false;
}

void CMainFrame::OnUpdateRobotPoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(flag);
}

void CMainFrame::OnGoalPoint() 
{
	// TODO: Add your command handler code here
	point=3;
}

void CMainFrame::OnUpdateGoalPoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(flag);
}

void CMainFrame::OnBegin() 
{
	CRobotViewApp* myapp=(CRobotViewApp*)AfxGetApp();
    CMainFrame* mainframe=(CMainFrame*)myapp->m_pMainWnd;
	CBlockView* treeview=(CBlockView*)mainframe->m_wndSplitter_Horizontal.GetPane(0,0); 
	CRobotViewView* robotview=(CRobotViewView*)mainframe->m_wndSplitter_Vertical.GetPane(0,0);
	treeview->MToControl();
	Pathplan *path=new Pathplan();
	path->plan(M);
	path->setRobotPos(robotview->m_robotPointy*12/HEIGHT,robotview->m_robotPointx*20/WIDTH);
	path->pathPlanAll(robotview->m_goalPointy*12/HEIGHT,robotview->m_goalPointx*20/WIDTH);
	path->GetVmap(VirtualMap);
	best=path->GetPath();

	
	Route* route=(Route*)mainframe->m_wndSplitter_Horizontal.GetPane(1,0);
	route->DrawRoute();
	// TODO: Add your command handler code here
//	UpdateData(false);
}

void CMainFrame::OnFileNew() 
{
	// TODO: Add your command handler code here
	CRobotViewView* robotview=(CRobotViewView*)this->m_wndSplitter_Vertical.GetPane(0,0);
	CBlockView* treeview=(CBlockView*)this->m_wndSplitter_Horizontal.GetPane(0,0); 
	for(int i=0;i<HMAX;i++)
		for(int j=0;j<WMAX;j++)
			M[i][j]=0;
	treeview->MToControl();
	CDocument* pDoc = robotview->GetDocument();
	pDoc->UpdateAllViews(/*robotview,0L,NULL*/NULL); 

}
