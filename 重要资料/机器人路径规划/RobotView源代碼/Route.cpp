// Route.cpp : implementation file
//

#include "stdafx.h"
#include "RobotView.h"
#include "Route.h"
#include "MainFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Route

IMPLEMENT_DYNCREATE(Route, CView)

Route::Route()
{
}

Route::~Route()
{
}


BEGIN_MESSAGE_MAP(Route, CView)
	//{{AFX_MSG_MAP(Route)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Route drawing

void Route::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
//	CRect rect;
//	GetClientRect(&rect);
	
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// Route diagnostics

#ifdef _DEBUG
void Route::AssertValid() const
{
	CView::AssertValid();
}

void Route::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Route message handlers

void Route::DrawRoute()
{
	/*画出地势高低图*/
	CDC* pDC=GetDC();
	CRobotViewApp* myapp=(CRobotViewApp*)AfxGetApp();
    CMainFrame* mainframe=(CMainFrame*)myapp->m_pMainWnd;
	CBrush brush0;
	brush0.CreateSolidBrush(RGB(255,255,255));
	CBrush brush1;
	brush1.CreateSolidBrush(RGB(247,248,253));
	CBrush brush2;
	brush2.CreateSolidBrush(RGB(234,236,251));
	CBrush brush3;
	brush3.CreateSolidBrush(RGB(223,225,249));
	CBrush brush4;
	brush4.CreateSolidBrush(RGB(205,209,245));
	CBrush brush5;
	brush5.CreateSolidBrush(RGB(192,198,243));
	CBrush brush6;
	brush6.CreateSolidBrush(RGB(179,185,240));
	CBrush brush7;
	brush7.CreateSolidBrush(RGB(165,172,237));
	CBrush brush8;
	brush8.CreateSolidBrush(RGB(149,157,234));
	CBrush brush9;
	brush9.CreateSolidBrush(RGB(134,143,232));
	CBrush brush10;
	brush10.CreateSolidBrush(RGB(121,132,230));
	CBrush brush11;
	brush11.CreateSolidBrush(RGB(105,117,226));
	CBrush brush12;
	brush12.CreateSolidBrush(RGB(94,106,223));
	CBrush brush13;
	brush13.CreateSolidBrush(RGB(79,92,221));
	CBrush brush14;
	brush14.CreateSolidBrush(RGB(63,79,218));
	CBrush brush15;
	brush15.CreateSolidBrush(RGB(50,67,216));
	CBrush brush16;
	brush16.CreateSolidBrush(RGB(40,57,208));
	CBrush brush17;
	brush17.CreateSolidBrush(RGB(38,54,196));
	CBrush brush18;
	brush18.CreateSolidBrush(RGB(35,50,184));
	CBrush brush19;
	brush19.CreateSolidBrush(RGB(33,47,169));
	CBrush brush20;
	brush20.CreateSolidBrush(RGB(3,4,16));

	for(int i=0;i<HMAX;i++)
	{
		for(int j=0;j<WMAX;j++)
		{
			if(mainframe->VirtualMap[i][j]>=0&&mainframe->VirtualMap[i][j]<=2)
			{
				pDC->SelectObject(brush0);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=3&&mainframe->VirtualMap[i][j]<=5)
			{
				pDC->SelectObject(brush1);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=6&&mainframe->VirtualMap[i][j]<=8)
			{
				pDC->SelectObject(brush2);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=9&&mainframe->VirtualMap[i][j]<=11)
			{
				pDC->SelectObject(brush3);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=12&&mainframe->VirtualMap[i][j]<=14)
			{
				pDC->SelectObject(brush4);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=15&&mainframe->VirtualMap[i][j]<=17)
			{
				pDC->SelectObject(brush5);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=18&&mainframe->VirtualMap[i][j]<=20)
			{
				pDC->SelectObject(brush6);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=21&&mainframe->VirtualMap[i][j]<=23)
			{
				pDC->SelectObject(brush7);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=24&&mainframe->VirtualMap[i][j]<=26)
			{
				pDC->SelectObject(brush8);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=27&&mainframe->VirtualMap[i][j]<=29)
			{
				pDC->SelectObject(brush9);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=30&&mainframe->VirtualMap[i][j]<=32)
			{
				pDC->SelectObject(brush10);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=33&&mainframe->VirtualMap[i][j]<=35)
			{
				pDC->SelectObject(brush11);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=36&&mainframe->VirtualMap[i][j]<=38)
			{
				pDC->SelectObject(brush12);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=39&&mainframe->VirtualMap[i][j]<=41)
			{
				pDC->SelectObject(brush13);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=42&&mainframe->VirtualMap[i][j]<=44)
			{
				pDC->SelectObject(brush14);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=45&&mainframe->VirtualMap[i][j]<=47)
			{
				pDC->SelectObject(brush15);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=48&&mainframe->VirtualMap[i][j]<=50)
			{
				pDC->SelectObject(brush16);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=51&&mainframe->VirtualMap[i][j]<=53)
			{
				pDC->SelectObject(brush17);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=54&&mainframe->VirtualMap[i][j]<=56)
			{
				pDC->SelectObject(brush18);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=57&&mainframe->VirtualMap[i][j]<=59)
			{
				pDC->SelectObject(brush19);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
			else if(mainframe->VirtualMap[i][j]>=60&&mainframe->VirtualMap[i][j]<=80)
			{
				pDC->SelectObject(brush20);
				pDC->Rectangle(CRect(j*RWIDTH/20,i*RHEIGHT/12,(j+1)*RWIDTH/20,(i+1)*RHEIGHT/12));
			}
		}
	}

	/*画出路径*/
	CPen newPen(PS_DASHDOTDOT,3,/*(COLORREF)0*/RGB(244,244,51));
	for(int k=0;k<(mainframe->best.pathlength)-1;k++)
	{
		pDC->SelectObject(&newPen);
		pDC->MoveTo((mainframe->best.pos[k].y*RWIDTH/20+RWIDTH/40),(mainframe->best.pos[k].x*RHEIGHT/12+RHEIGHT/24));
		pDC->LineTo((mainframe->best.pos[k+1].y*RWIDTH/20+RWIDTH/40),(mainframe->best.pos[k+1].x*RHEIGHT/12+RHEIGHT/24));
	}

}
