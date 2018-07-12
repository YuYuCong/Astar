// RobotViewView.cpp : implementation of the CRobotViewView class
//

#include "stdafx.h"
#include "RobotView.h"

#include "RobotViewDoc.h"
#include "RobotViewView.h"
#include "MainFrm.h"
#include "BlockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotViewView

IMPLEMENT_DYNCREATE(CRobotViewView, CView)

BEGIN_MESSAGE_MAP(CRobotViewView, CView)
	//{{AFX_MSG_MAP(CRobotViewView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotViewView construction/destruction

CRobotViewView::CRobotViewView()
{
	// TODO: add construction code here

}

CRobotViewView::~CRobotViewView()
{
}

BOOL CRobotViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRobotViewView drawing

void CRobotViewView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
//	pDC->FillSolidRect(&rect,RGB(202,225,255));
//	pDC->FillSolidRect(&rect,RGB(132,183, 234));
	pDC->FillSolidRect(&rect,RGB(165,202,239));
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CRobotViewView diagnostics

#ifdef _DEBUG
void CRobotViewView::AssertValid() const
{
	CView::AssertValid();
}

void CRobotViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRobotViewDoc* CRobotViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotViewDoc)));
	return (CRobotViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotViewView message handlers

void CRobotViewView::read(int h, int w)
{
	Height=h;
	Width=w;
}

void CRobotViewView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CView::OnLButtonDown(nFlags, point);
	CRobotViewApp* myapp=(CRobotViewApp*)AfxGetApp();
    CMainFrame* mainframe=(CMainFrame*)myapp->m_pMainWnd;

	if(mainframe->flag==false&&mainframe->point==1)
	{
		showBlock(point.x-Width/2,point.y-Height/2,point.x+Width/2,point.y+Height/2);
		mainframe->flag=true;
		for(int i=((point.x-Width/2)*20/WIDTH);i<=((point.x+Width/2)*20/WIDTH);i++)
			for(int j=((point.y-Height/2)*12/HEIGHT);j<=((point.y+Height/2)*12/HEIGHT);j++)
			{
				mainframe->M[j][i]=1;
			}
		CBlockView* treeview=(CBlockView*)mainframe->m_wndSplitter_Horizontal.GetPane(0,0); 
	    treeview->MToControl();
	}
	else if(mainframe->point==2)
	{
		showRobotPoint(point.x,point.y);
		mainframe->flag==true;
		m_robotPointx=point.x;
		m_robotPointy=point.y;
	}
	else if(mainframe->point==3)
	{
		showGoalPoint(point.x,point.y);
		m_goalPointx=point.x;
		m_goalPointy=point.y;
	}
	
	
}

void CRobotViewView::showBlock(int x1,int y1,int x2,int y2)
{
	CDC *pDC=GetDC();
	CBrush brush;
	brush.CreateSolidBrush(RGB(234,77,0));
//	pDC->SelectStockObject(brush);
	pDC->SelectObject(brush);
	pDC->Rectangle(CRect(x1,y1,x2,y2));
}

void CRobotViewView::showRobotPoint(int x,int y)
{
	CDC *pDC=GetDC();
	CBrush brush;
	brush.CreateSolidBrush(RGB(112,207,119));
//	pDC->SelectStockObject(brush);
	pDC->SelectObject(brush);
	pDC->Ellipse(x-15,y-15,x+15,y+15);
}

void CRobotViewView::showGoalPoint(int x, int y)
{
	CDC *pDC=GetDC();
	CBrush brush;
	brush.CreateSolidBrush(RGB(254,247,107));
//	pDC->SelectStockObject(brush);
	pDC->SelectObject(brush);
	pDC->Ellipse(x-15,y-15,x+20,y+15);
}
