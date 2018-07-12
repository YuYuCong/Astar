// BlockView.cpp : implementation file
//

#include "stdafx.h"
#include "RobotView.h"
#include "BlockView.h"
#include "RobotViewView.h"
#include "MainFrm.h"
#include "RobotViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockView

IMPLEMENT_DYNCREATE(CBlockView, CFormView)

CBlockView::CBlockView()
	: CFormView(CBlockView::IDD)
{

	//{{AFX_DATA_INIT(CBlockView)
	m_0_9 = 0;
	m_0_8 = 0;
	m_0_7 = 0;
	m_0_6 = 0;
	m_0_5 = 0;
	m_0_4 = 0;
	m_0_3 = 0;
	m_0_2 = 0;
	m_0_19 = 0;
	m_0_18 = 0;
	m_0_17 = 0;
	m_0_16 = 0;
	m_0_15 = 0;
	m_0_14 = 0;
	m_0_13 = 0;
	m_0_12 = 0;
	m_0_11 = 0;
	m_0_10 = 0;
	m_0_1 = 0;
	m_0_0 = 0;
	m_1_0 = 0;
	m_1_1 = 0;
	m_1_10 = 0;
	m_1_11 = 0;
	m_1_12 = 0;
	m_1_13 = 0;
	m_1_14 = 0;
	m_1_15 = 0;
	m_1_16 = 0;
	m_1_17 = 0;
	m_1_18 = 0;
	m_1_19 = 0;
	m_1_2 = 0;
	m_1_3 = 0;
	m_1_4 = 0;
	m_1_5 = 0;
	m_1_6 = 0;
	m_1_7 = 0;
	m_1_8 = 0;
	m_1_9 = 0;
	m_2_0 = 0;
	m_2_1 = 0;
	m_2_10 = 0;
	m_2_11 = 0;
	m_2_12 = 0;
	m_2_13 = 0;
	m_2_14 = 0;
	m_2_15 = 0;
	m_2_16 = 0;
	m_2_17 = 0;
	m_2_18 = 0;
	m_2_19 = 0;
	m_2_2 = 0;
	m_2_3 = 0;
	m_2_4 = 0;
	m_2_5 = 0;
	m_2_6 = 0;
	m_2_7 = 0;
	m_2_8 = 0;
	m_2_9 = 0;
	m_3_0 = 0;
	m_3_1 = 0;
	m_3_10 = 0;
	m_3_11 = 0;
	m_3_12 = 0;
	m_3_14 = 0;
	m_3_13 = 0;
	m_3_15 = 0;
	m_3_16 = 0;
	m_3_17 = 0;
	m_3_18 = 0;
	m_3_19 = 0;
	m_3_2 = 0;
	m_3_3 = 0;
	m_3_4 = 0;
	m_3_5 = 0;
	m_3_6 = 0;
	m_3_7 = 0;
	m_3_8 = 0;
	m_3_9 = 0;
	m_4_0 = 0;
	m_4_1 = 0;
	m_4_10 = 0;
	m_4_11 = 0;
	m_4_12 = 0;
	m_4_13 = 0;
	m_4_14 = 0;
	m_4_15 = 0;
	m_4_16 = 0;
	m_4_17 = 0;
	m_4_18 = 0;
	m_4_2 = 0;
	m_4_19 = 0;
	m_4_3 = 0;
	m_4_4 = 0;
	m_4_5 = 0;
	m_4_6 = 0;
	m_4_7 = 0;
	m_4_8 = 0;
	m_4_9 = 0;
	m_5_0 = 0;
	m_5_1 = 0;
	m_5_10 = 0;
	m_5_11 = 0;
	m_5_12 = 0;
	m_5_13 = 0;
	m_5_14 = 0;
	m_5_15 = 0;
	m_5_16 = 0;
	m_5_17 = 0;
	m_5_18 = 0;
	m_5_19 = 0;
	m_5_2 = 0;
	m_5_3 = 0;
	m_5_4 = 0;
	m_5_5 = 0;
	m_5_6 = 0;
	m_5_7 = 0;
	m_5_8 = 0;
	m_5_9 = 0;
	m_6_0 = 0;
	m_6_1 = 0;
	m_6_10 = 0;
	m_6_11 = 0;
	m_6_12 = 0;
	m_6_13 = 0;
	m_6_14 = 0;
	m_6_15 = 0;
	m_6_16 = 0;
	m_6_17 = 0;
	m_6_18 = 0;
	m_6_19 = 0;
	m_6_2 = 0;
	m_6_3 = 0;
	m_6_4 = 0;
	m_6_5 = 0;
	m_6_6 = 0;
	m_6_7 = 0;
	m_6_8 = 0;
	m_6_9 = 0;
	m_7_0 = 0;
	m_7_1 = 0;
	m_7_10 = 0;
	m_7_11 = 0;
	m_7_12 = 0;
	m_7_13 = 0;
	m_7_14 = 0;
	m_7_15 = 0;
	m_7_16 = 0;
	m_7_17 = 0;
	m_7_18 = 0;
	m_7_19 = 0;
	m_7_2 = 0;
	m_7_3 = 0;
	m_7_4 = 0;
	m_7_5 = 0;
	m_7_6 = 0;
	m_7_7 = 0;
	m_7_8 = 0;
	m_7_9 = 0;
	m_8_0 = 0;
	m_8_1 = 0;
	m_8_10 = 0;
	m_8_11 = 0;
	m_8_12 = 0;
	m_8_13 = 0;
	m_8_14 = 0;
	m_8_15 = 0;
	m_8_16 = 0;
	m_8_17 = 0;
	m_8_18 = 0;
	m_8_19 = 0;
	m_8_2 = 0;
	m_8_3 = 0;
	m_8_4 = 0;
	m_8_5 = 0;
	m_8_6 = 0;
	m_8_7 = 0;
	m_8_8 = 0;
	m_8_9 = 0;
	m_9_0 = 0;
	m_9_1 = 0;
	m_9_10 = 0;
	m_9_11 = 0;
	m_9_12 = 0;
	m_9_13 = 0;
	m_9_14 = 0;
	m_9_15 = 0;
	m_9_16 = 0;
	m_9_17 = 0;
	m_9_18 = 0;
	m_9_19 = 0;
	m_9_2 = 0;
	m_9_3 = 0;
	m_9_4 = 0;
	m_9_5 = 0;
	m_9_6 = 0;
	m_9_7 = 0;
	m_9_8 = 0;
	m_9_9 = 0;
	m_10_0 = 0;
	m_10_1 = 0;
	m_10_10 = 0;
	m_10_11 = 0;
	m_10_12 = 0;
	m_10_13 = 0;
	m_10_14 = 0;
	m_10_15 = 0;
	m_10_16 = 0;
	m_10_17 = 0;
	m_10_18 = 0;
	m_10_19 = 0;
	m_10_2 = 0;
	m_10_3 = 0;
	m_10_4 = 0;
	m_10_5 = 0;
	m_10_6 = 0;
	m_10_7 = 0;
	m_10_8 = 0;
	m_10_9 = 0;
	m_11_0 = 0;
	m_11_1 = 0;
	m_11_10 = 0;
	m_11_11 = 0;
	m_11_12 = 0;
	m_11_13 = 0;
	m_11_14 = 0;
	m_11_15 = 0;
	m_11_16 = 0;
	m_11_17 = 0;
	m_11_18 = 0;
	m_11_19 = 0;
	m_11_2 = 0;
	m_11_3 = 0;
	m_11_4 = 0;
	m_11_5 = 0;
	m_11_6 = 0;
	m_11_7 = 0;
	m_11_8 = 0;
	m_11_9 = 0;
	//}}AFX_DATA_INIT

}

CBlockView::~CBlockView()
{
}

void CBlockView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockView)
	DDX_Text(pDX, IDC_M0_9, m_0_9);
	DDX_Text(pDX, IDC_M0_8, m_0_8);
	DDX_Text(pDX, IDC_M0_7, m_0_7);
	DDX_Text(pDX, IDC_M0_6, m_0_6);
	DDX_Text(pDX, IDC_M0_5, m_0_5);
	DDX_Text(pDX, IDC_M0_4, m_0_4);
	DDX_Text(pDX, IDC_M0_3, m_0_3);
	DDX_Text(pDX, IDC_M0_2, m_0_2);
	DDX_Text(pDX, IDC_M0_19, m_0_19);
	DDX_Text(pDX, IDC_M0_18, m_0_18);
	DDX_Text(pDX, IDC_M0_17, m_0_17);
	DDX_Text(pDX, IDC_M0_16, m_0_16);
	DDX_Text(pDX, IDC_M0_15, m_0_15);
	DDX_Text(pDX, IDC_M0_14, m_0_14);
	DDX_Text(pDX, IDC_M0_13, m_0_13);
	DDX_Text(pDX, IDC_M0_12, m_0_12);
	DDX_Text(pDX, IDC_M0_11, m_0_11);
	DDX_Text(pDX, IDC_M0_10, m_0_10);
	DDX_Text(pDX, IDC_M0_1, m_0_1);
	DDX_Text(pDX, IDC_M0_0, m_0_0);
	DDX_Text(pDX, IDC_M1_0, m_1_0);
	DDX_Text(pDX, IDC_M1_1, m_1_1);
	DDX_Text(pDX, IDC_M1_10, m_1_10);
	DDX_Text(pDX, IDC_M1_11, m_1_11);
	DDX_Text(pDX, IDC_M1_12, m_1_12);
	DDX_Text(pDX, IDC_M1_13, m_1_13);
	DDX_Text(pDX, IDC_M1_14, m_1_14);
	DDX_Text(pDX, IDC_M1_15, m_1_15);
	DDX_Text(pDX, IDC_M1_16, m_1_16);
	DDX_Text(pDX, IDC_M1_17, m_1_17);
	DDX_Text(pDX, IDC_M1_18, m_1_18);
	DDX_Text(pDX, IDC_M1_19, m_1_19);
	DDX_Text(pDX, IDC_M1_2, m_1_2);
	DDX_Text(pDX, IDC_M1_3, m_1_3);
	DDX_Text(pDX, IDC_M1_4, m_1_4);
	DDX_Text(pDX, IDC_M1_5, m_1_5);
	DDX_Text(pDX, IDC_M1_6, m_1_6);
	DDX_Text(pDX, IDC_M1_7, m_1_7);
	DDX_Text(pDX, IDC_M1_8, m_1_8);
	DDX_Text(pDX, IDC_M1_9, m_1_9);
	DDX_Text(pDX, IDC_M2_0, m_2_0);
	DDX_Text(pDX, IDC_M2_1, m_2_1);
	DDX_Text(pDX, IDC_M2_10, m_2_10);
	DDX_Text(pDX, IDC_M2_11, m_2_11);
	DDX_Text(pDX, IDC_M2_12, m_2_12);
	DDX_Text(pDX, IDC_M2_13, m_2_13);
	DDX_Text(pDX, IDC_M2_14, m_2_14);
	DDX_Text(pDX, IDC_M2_15, m_2_15);
	DDX_Text(pDX, IDC_M2_16, m_2_16);
	DDX_Text(pDX, IDC_M2_17, m_2_17);
	DDX_Text(pDX, IDC_M2_18, m_2_18);
	DDX_Text(pDX, IDC_M2_19, m_2_19);
	DDX_Text(pDX, IDC_M2_2, m_2_2);
	DDX_Text(pDX, IDC_M2_3, m_2_3);
	DDX_Text(pDX, IDC_M2_4, m_2_4);
	DDX_Text(pDX, IDC_M2_5, m_2_5);
	DDX_Text(pDX, IDC_M2_6, m_2_6);
	DDX_Text(pDX, IDC_M2_7, m_2_7);
	DDX_Text(pDX, IDC_M2_8, m_2_8);
	DDX_Text(pDX, IDC_M2_9, m_2_9);
	DDX_Text(pDX, IDC_M3_0, m_3_0);
	DDX_Text(pDX, IDC_M3_1, m_3_1);
	DDX_Text(pDX, IDC_M3_10, m_3_10);
	DDX_Text(pDX, IDC_M3_11, m_3_11);
	DDX_Text(pDX, IDC_M3_12, m_3_12);
	DDX_Text(pDX, IDC_M3_14, m_3_14);
	DDX_Text(pDX, IDC_M3_13, m_3_13);
	DDX_Text(pDX, IDC_M3_15, m_3_15);
	DDX_Text(pDX, IDC_M3_16, m_3_16);
	DDX_Text(pDX, IDC_M3_17, m_3_17);
	DDX_Text(pDX, IDC_M3_18, m_3_18);
	DDX_Text(pDX, IDC_M3_19, m_3_19);
	DDX_Text(pDX, IDC_M3_2, m_3_2);
	DDX_Text(pDX, IDC_M3_3, m_3_3);
	DDX_Text(pDX, IDC_M3_4, m_3_4);
	DDX_Text(pDX, IDC_M3_5, m_3_5);
	DDX_Text(pDX, IDC_M3_6, m_3_6);
	DDX_Text(pDX, IDC_M3_7, m_3_7);
	DDX_Text(pDX, IDC_M3_8, m_3_8);
	DDX_Text(pDX, IDC_M3_9, m_3_9);
	DDX_Text(pDX, IDC_M4_0, m_4_0);
	DDX_Text(pDX, IDC_M4_1, m_4_1);
	DDX_Text(pDX, IDC_M4_10, m_4_10);
	DDX_Text(pDX, IDC_M4_11, m_4_11);
	DDX_Text(pDX, IDC_M4_12, m_4_12);
	DDX_Text(pDX, IDC_M4_13, m_4_13);
	DDX_Text(pDX, IDC_M4_14, m_4_14);
	DDX_Text(pDX, IDC_M4_15, m_4_15);
	DDX_Text(pDX, IDC_M4_16, m_4_16);
	DDX_Text(pDX, IDC_M4_17, m_4_17);
	DDX_Text(pDX, IDC_M4_18, m_4_18);
	DDX_Text(pDX, IDC_M4_2, m_4_2);
	DDX_Text(pDX, IDC_M4_19, m_4_19);
	DDX_Text(pDX, IDC_M4_3, m_4_3);
	DDX_Text(pDX, IDC_M4_4, m_4_4);
	DDX_Text(pDX, IDC_M4_5, m_4_5);
	DDX_Text(pDX, IDC_M4_6, m_4_6);
	DDX_Text(pDX, IDC_M4_7, m_4_7);
	DDX_Text(pDX, IDC_M4_8, m_4_8);
	DDX_Text(pDX, IDC_M4_9, m_4_9);
	DDX_Text(pDX, IDC_M5_0, m_5_0);
	DDX_Text(pDX, IDC_M5_1, m_5_1);
	DDX_Text(pDX, IDC_M5_10, m_5_10);
	DDX_Text(pDX, IDC_M5_11, m_5_11);
	DDX_Text(pDX, IDC_M5_12, m_5_12);
	DDX_Text(pDX, IDC_M5_13, m_5_13);
	DDX_Text(pDX, IDC_M5_14, m_5_14);
	DDX_Text(pDX, IDC_M5_15, m_5_15);
	DDX_Text(pDX, IDC_M5_16, m_5_16);
	DDX_Text(pDX, IDC_M5_17, m_5_17);
	DDX_Text(pDX, IDC_M5_18, m_5_18);
	DDX_Text(pDX, IDC_M5_19, m_5_19);
	DDX_Text(pDX, IDC_M5_2, m_5_2);
	DDX_Text(pDX, IDC_M5_3, m_5_3);
	DDX_Text(pDX, IDC_M5_4, m_5_4);
	DDX_Text(pDX, IDC_M5_5, m_5_5);
	DDX_Text(pDX, IDC_M5_6, m_5_6);
	DDX_Text(pDX, IDC_M5_7, m_5_7);
	DDX_Text(pDX, IDC_M5_8, m_5_8);
	DDX_Text(pDX, IDC_M5_9, m_5_9);
	DDX_Text(pDX, IDC_M6_0, m_6_0);
	DDX_Text(pDX, IDC_M6_1, m_6_1);
	DDX_Text(pDX, IDC_M6_10, m_6_10);
	DDX_Text(pDX, IDC_M6_11, m_6_11);
	DDX_Text(pDX, IDC_M6_12, m_6_12);
	DDX_Text(pDX, IDC_M6_13, m_6_13);
	DDX_Text(pDX, IDC_M6_14, m_6_14);
	DDX_Text(pDX, IDC_M6_15, m_6_15);
	DDX_Text(pDX, IDC_M6_16, m_6_16);
	DDX_Text(pDX, IDC_M6_17, m_6_17);
	DDX_Text(pDX, IDC_M6_18, m_6_18);
	DDX_Text(pDX, IDC_M6_19, m_6_19);
	DDX_Text(pDX, IDC_M6_2, m_6_2);
	DDX_Text(pDX, IDC_M6_3, m_6_3);
	DDX_Text(pDX, IDC_M6_4, m_6_4);
	DDX_Text(pDX, IDC_M6_5, m_6_5);
	DDX_Text(pDX, IDC_M6_6, m_6_6);
	DDX_Text(pDX, IDC_M6_7, m_6_7);
	DDX_Text(pDX, IDC_M6_8, m_6_8);
	DDX_Text(pDX, IDC_M6_9, m_6_9);
	DDX_Text(pDX, IDC_M7_0, m_7_0);
	DDX_Text(pDX, IDC_M7_1, m_7_1);
	DDX_Text(pDX, IDC_M7_10, m_7_10);
	DDX_Text(pDX, IDC_M7_11, m_7_11);
	DDX_Text(pDX, IDC_M7_12, m_7_12);
	DDX_Text(pDX, IDC_M7_13, m_7_13);
	DDX_Text(pDX, IDC_M7_14, m_7_14);
	DDX_Text(pDX, IDC_M7_15, m_7_15);
	DDX_Text(pDX, IDC_M7_16, m_7_16);
	DDX_Text(pDX, IDC_M7_17, m_7_17);
	DDX_Text(pDX, IDC_M7_18, m_7_18);
	DDX_Text(pDX, IDC_M7_19, m_7_19);
	DDX_Text(pDX, IDC_M7_2, m_7_2);
	DDX_Text(pDX, IDC_M7_3, m_7_3);
	DDX_Text(pDX, IDC_M7_4, m_7_4);
	DDX_Text(pDX, IDC_M7_5, m_7_5);
	DDX_Text(pDX, IDC_M7_6, m_7_6);
	DDX_Text(pDX, IDC_M7_7, m_7_7);
	DDX_Text(pDX, IDC_M7_8, m_7_8);
	DDX_Text(pDX, IDC_M7_9, m_7_9);
	DDX_Text(pDX, IDC_M8_0, m_8_0);
	DDX_Text(pDX, IDC_M8_1, m_8_1);
	DDX_Text(pDX, IDC_M8_10, m_8_10);
	DDX_Text(pDX, IDC_M8_11, m_8_11);
	DDX_Text(pDX, IDC_M8_12, m_8_12);
	DDX_Text(pDX, IDC_M8_13, m_8_13);
	DDX_Text(pDX, IDC_M8_14, m_8_14);
	DDX_Text(pDX, IDC_M8_15, m_8_15);
	DDX_Text(pDX, IDC_M8_16, m_8_16);
	DDX_Text(pDX, IDC_M8_17, m_8_17);
	DDX_Text(pDX, IDC_M8_18, m_8_18);
	DDX_Text(pDX, IDC_M8_19, m_8_19);
	DDX_Text(pDX, IDC_M8_2, m_8_2);
	DDX_Text(pDX, IDC_M8_3, m_8_3);
	DDX_Text(pDX, IDC_M8_4, m_8_4);
	DDX_Text(pDX, IDC_M8_5, m_8_5);
	DDX_Text(pDX, IDC_M8_6, m_8_6);
	DDX_Text(pDX, IDC_M8_7, m_8_7);
	DDX_Text(pDX, IDC_M8_8, m_8_8);
	DDX_Text(pDX, IDC_M8_9, m_8_9);
	DDX_Text(pDX, IDC_M9_0, m_9_0);
	DDX_Text(pDX, IDC_M9_1, m_9_1);
	DDX_Text(pDX, IDC_M9_10, m_9_10);
	DDX_Text(pDX, IDC_M9_11, m_9_11);
	DDX_Text(pDX, IDC_M9_12, m_9_12);
	DDX_Text(pDX, IDC_M9_13, m_9_13);
	DDX_Text(pDX, IDC_M9_14, m_9_14);
	DDX_Text(pDX, IDC_M9_15, m_9_15);
	DDX_Text(pDX, IDC_M9_16, m_9_16);
	DDX_Text(pDX, IDC_M9_17, m_9_17);
	DDX_Text(pDX, IDC_M9_18, m_9_18);
	DDX_Text(pDX, IDC_M9_19, m_9_19);
	DDX_Text(pDX, IDC_M9_2, m_9_2);
	DDX_Text(pDX, IDC_M9_3, m_9_3);
	DDX_Text(pDX, IDC_M9_4, m_9_4);
	DDX_Text(pDX, IDC_M9_5, m_9_5);
	DDX_Text(pDX, IDC_M9_6, m_9_6);
	DDX_Text(pDX, IDC_M9_7, m_9_7);
	DDX_Text(pDX, IDC_M9_8, m_9_8);
	DDX_Text(pDX, IDC_M9_9, m_9_9);
	DDX_Text(pDX, IDC_M10_0, m_10_0);
	DDX_Text(pDX, IDC_M10_1, m_10_1);
	DDX_Text(pDX, IDC_M10_10, m_10_10);
	DDX_Text(pDX, IDC_M10_11, m_10_11);
	DDX_Text(pDX, IDC_M10_12, m_10_12);
	DDX_Text(pDX, IDC_M10_13, m_10_13);
	DDX_Text(pDX, IDC_M10_14, m_10_14);
	DDX_Text(pDX, IDC_M10_15, m_10_15);
	DDX_Text(pDX, IDC_M10_16, m_10_16);
	DDX_Text(pDX, IDC_M10_17, m_10_17);
	DDX_Text(pDX, IDC_M10_18, m_10_18);
	DDX_Text(pDX, IDC_M10_19, m_10_19);
	DDX_Text(pDX, IDC_M10_2, m_10_2);
	DDX_Text(pDX, IDC_M10_3, m_10_3);
	DDX_Text(pDX, IDC_M10_4, m_10_4);
	DDX_Text(pDX, IDC_M10_5, m_10_5);
	DDX_Text(pDX, IDC_M10_6, m_10_6);
	DDX_Text(pDX, IDC_M10_7, m_10_7);
	DDX_Text(pDX, IDC_M10_8, m_10_8);
	DDX_Text(pDX, IDC_M10_9, m_10_9);
	DDX_Text(pDX, IDC_M11_0, m_11_0);
	DDX_Text(pDX, IDC_M11_1, m_11_1);
	DDX_Text(pDX, IDC_M11_10, m_11_10);
	DDX_Text(pDX, IDC_M11_11, m_11_11);
	DDX_Text(pDX, IDC_M11_12, m_11_12);
	DDX_Text(pDX, IDC_M11_13, m_11_13);
	DDX_Text(pDX, IDC_M11_14, m_11_14);
	DDX_Text(pDX, IDC_M11_15, m_11_15);
	DDX_Text(pDX, IDC_M11_16, m_11_16);
	DDX_Text(pDX, IDC_M11_17, m_11_17);
	DDX_Text(pDX, IDC_M11_18, m_11_18);
	DDX_Text(pDX, IDC_M11_19, m_11_19);
	DDX_Text(pDX, IDC_M11_2, m_11_2);
	DDX_Text(pDX, IDC_M11_3, m_11_3);
	DDX_Text(pDX, IDC_M11_4, m_11_4);
	DDX_Text(pDX, IDC_M11_5, m_11_5);
	DDX_Text(pDX, IDC_M11_6, m_11_6);
	DDX_Text(pDX, IDC_M11_7, m_11_7);
	DDX_Text(pDX, IDC_M11_8, m_11_8);
	DDX_Text(pDX, IDC_M11_9, m_11_9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockView, CFormView)
	//{{AFX_MSG_MAP(CBlockView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockView diagnostics

#ifdef _DEBUG
void CBlockView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBlockView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBlockView message handlers

void CBlockView::MToControl()
{
	CMainFrame* mainframe=(CMainFrame*)AfxGetMainWnd();
    m_0_0=mainframe->M[0][0];
	m_0_1=mainframe->M[0][1];
    m_0_2=mainframe->M[0][2];
	m_0_3=mainframe->M[0][3];
	m_0_4=mainframe->M[0][4];
	m_0_5=mainframe->M[0][5];
	m_0_6=mainframe->M[0][6];
	m_0_7=mainframe->M[0][7];
	m_0_8=mainframe->M[0][8];
	m_0_9=mainframe->M[0][9];
	m_0_10=mainframe->M[0][10];
	m_0_11=mainframe->M[0][11];
	m_0_12=mainframe->M[0][12];
	m_0_13=mainframe->M[0][13];
	m_0_14=mainframe->M[0][14];
	m_0_15=mainframe->M[0][15];
	m_0_16=mainframe->M[0][16];
	m_0_17=mainframe->M[0][17];
	m_0_18=mainframe->M[0][18];
	m_0_19=mainframe->M[0][19];
	m_1_0=mainframe->M[1][0];
	m_1_1=mainframe->M[1][1];
	m_1_2=mainframe->M[1][2];
	m_1_3=mainframe->M[1][3];
	m_1_4=mainframe->M[1][4];
	m_1_5=mainframe->M[1][5];
	m_1_6=mainframe->M[1][6];
	m_1_7=mainframe->M[1][7];
	m_1_8=mainframe->M[1][8];
	m_1_9=mainframe->M[1][9];
	m_1_10=mainframe->M[1][10];
	m_1_11=mainframe->M[1][11];
	m_1_12=mainframe->M[1][12];
	m_1_13=mainframe->M[1][13];
	m_1_14=mainframe->M[1][14];
	m_1_15=mainframe->M[1][15];
	m_1_16=mainframe->M[1][16];
	m_1_17=mainframe->M[1][17];
	m_1_18=mainframe->M[1][18];
	m_1_19=mainframe->M[1][19];
	m_2_0=mainframe->M[2][0];
	m_2_1=mainframe->M[2][1];
	m_2_2=mainframe->M[2][2];
	m_2_3=mainframe->M[2][3];
	m_2_4=mainframe->M[2][4];
	m_2_5=mainframe->M[2][5];
	m_2_6=mainframe->M[2][6];
	m_2_7=mainframe->M[2][7];
	m_2_8=mainframe->M[2][8];
	m_2_9=mainframe->M[2][9];
	m_2_10=mainframe->M[2][10];
	m_2_11=mainframe->M[2][11];
	m_2_12=mainframe->M[2][12];
	m_2_13=mainframe->M[2][13];
	m_2_14=mainframe->M[2][14];
	m_2_15=mainframe->M[2][15];
	m_2_16=mainframe->M[2][16];
	m_2_17=mainframe->M[2][17];
	m_2_18=mainframe->M[2][18];
	m_2_19=mainframe->M[2][19];
	m_3_0=mainframe->M[3][0];
	m_3_1=mainframe->M[3][1];
	m_3_2=mainframe->M[3][2];
	m_3_3=mainframe->M[3][3];
	m_3_4=mainframe->M[3][4];
	m_3_5=mainframe->M[3][5];
	m_3_6=mainframe->M[3][6];
	m_3_7=mainframe->M[3][7];
	m_3_8=mainframe->M[3][8];
	m_3_9=mainframe->M[3][9];
	m_3_10=mainframe->M[3][10];
	m_3_11=mainframe->M[3][11];
	m_3_12=mainframe->M[3][12];
	m_3_13=mainframe->M[3][13];
	m_3_14=mainframe->M[3][14];
	m_3_15=mainframe->M[3][15];
	m_3_16=mainframe->M[3][16];
	m_3_17=mainframe->M[3][17];
	m_3_18=mainframe->M[3][18];
	m_3_19=mainframe->M[3][19];
	m_4_0=mainframe->M[4][0];
	m_4_1=mainframe->M[4][1];
	m_4_2=mainframe->M[4][2];
	m_4_3=mainframe->M[4][3];
	m_4_4=mainframe->M[4][4];
	m_4_5=mainframe->M[4][5];
	m_4_6=mainframe->M[4][6];
	m_4_7=mainframe->M[4][7];
	m_4_8=mainframe->M[4][8];
	m_4_9=mainframe->M[4][9];
	m_4_10=mainframe->M[4][10];
	m_4_11=mainframe->M[4][11];
	m_4_12=mainframe->M[4][12];
	m_4_13=mainframe->M[4][13];
	m_4_14=mainframe->M[4][14];
	m_4_15=mainframe->M[4][15];
	m_4_16=mainframe->M[4][16];
	m_4_17=mainframe->M[4][17];
	m_4_18=mainframe->M[4][18];
	m_4_19=mainframe->M[4][19];
	m_5_0=mainframe->M[5][0];
	m_5_1=mainframe->M[5][1];
	m_5_2=mainframe->M[5][2];
	m_5_3=mainframe->M[5][3];
	m_5_4=mainframe->M[5][4];
	m_5_5=mainframe->M[5][5];
	m_5_6=mainframe->M[5][6];
	m_5_7=mainframe->M[5][7];
	m_5_8=mainframe->M[5][8];
	m_5_9=mainframe->M[5][9];
	m_5_10=mainframe->M[5][10];
	m_5_11=mainframe->M[5][11];
	m_5_12=mainframe->M[5][12];
	m_5_13=mainframe->M[5][13];
	m_5_14=mainframe->M[5][14];
	m_5_15=mainframe->M[5][15];
	m_5_16=mainframe->M[5][16];
	m_5_17=mainframe->M[5][17];
	m_5_18=mainframe->M[5][18];
	m_5_19=mainframe->M[5][19];
	m_6_0=mainframe->M[6][0];
	m_6_1=mainframe->M[6][1];
	m_6_2=mainframe->M[6][2];
	m_6_3=mainframe->M[6][3];
	m_6_4=mainframe->M[6][4];
	m_6_5=mainframe->M[6][5];
	m_6_6=mainframe->M[6][6];
	m_6_7=mainframe->M[6][7];
	m_6_8=mainframe->M[6][8];
	m_6_9=mainframe->M[6][9];
	m_6_10=mainframe->M[6][10];
	m_6_11=mainframe->M[6][11];
	m_6_12=mainframe->M[6][12];
	m_6_13=mainframe->M[6][13];
	m_6_14=mainframe->M[6][14];
	m_6_15=mainframe->M[6][15];
	m_6_16=mainframe->M[6][16];
	m_6_17=mainframe->M[6][17];
	m_6_18=mainframe->M[6][18];
	m_6_19=mainframe->M[6][19];
	m_7_0=mainframe->M[7][0];
	m_7_1=mainframe->M[7][1];
	m_7_2=mainframe->M[7][2];
	m_7_3=mainframe->M[7][3];
	m_7_4=mainframe->M[7][4];
	m_7_5=mainframe->M[7][5];
	m_7_6=mainframe->M[7][6];
	m_7_7=mainframe->M[7][7];
	m_7_8=mainframe->M[7][8];
	m_7_9=mainframe->M[7][9];
	m_7_10=mainframe->M[7][10];
	m_7_11=mainframe->M[7][11];
	m_7_12=mainframe->M[7][12];
	m_7_13=mainframe->M[7][13];
	m_7_14=mainframe->M[7][14];
	m_7_15=mainframe->M[7][15];
	m_7_16=mainframe->M[7][16];
	m_7_17=mainframe->M[7][17];
	m_7_18=mainframe->M[7][18];
	m_7_19=mainframe->M[7][19];
	m_8_0=mainframe->M[8][0];
	m_8_1=mainframe->M[8][1];
	m_8_2=mainframe->M[8][2];
	m_8_3=mainframe->M[8][3];
	m_8_4=mainframe->M[8][4];
	m_8_5=mainframe->M[8][5];
	m_8_6=mainframe->M[8][6];
	m_8_7=mainframe->M[8][7];
	m_8_8=mainframe->M[8][8];
	m_8_9=mainframe->M[8][9];
	m_8_10=mainframe->M[8][10];
	m_8_11=mainframe->M[8][11];
	m_8_12=mainframe->M[8][12];
	m_8_13=mainframe->M[8][13];
	m_8_14=mainframe->M[8][14];
	m_8_15=mainframe->M[8][15];
	m_8_16=mainframe->M[8][16];
	m_8_17=mainframe->M[8][17];
	m_8_18=mainframe->M[8][18];
	m_8_19=mainframe->M[8][19];
	m_9_0=mainframe->M[9][0];
	m_9_1=mainframe->M[9][1];
	m_9_2=mainframe->M[9][2];
	m_9_3=mainframe->M[9][3];
	m_9_4=mainframe->M[9][4];
	m_9_5=mainframe->M[9][5];
	m_9_6=mainframe->M[9][6];
	m_9_7=mainframe->M[9][7];
	m_9_8=mainframe->M[9][8];
	m_9_9=mainframe->M[9][9];
	m_9_10=mainframe->M[9][10];
	m_9_11=mainframe->M[9][11];
	m_9_12=mainframe->M[9][12];
	m_9_13=mainframe->M[9][13];
	m_9_14=mainframe->M[9][14];
	m_9_15=mainframe->M[9][15];
	m_9_16=mainframe->M[9][16];
	m_9_17=mainframe->M[9][17];
	m_9_18=mainframe->M[9][18];
	m_9_19=mainframe->M[9][19];
	m_10_0=mainframe->M[10][0];
	m_10_1=mainframe->M[10][1];
	m_10_2=mainframe->M[10][2];
	m_10_3=mainframe->M[10][3];
	m_10_4=mainframe->M[10][4];
	m_10_5=mainframe->M[10][5];
	m_10_6=mainframe->M[10][6];
	m_10_7=mainframe->M[10][7];
	m_10_8=mainframe->M[10][8];
	m_10_9=mainframe->M[10][9];
	m_10_10=mainframe->M[10][10];
	m_10_11=mainframe->M[10][11];
	m_10_12=mainframe->M[10][12];
	m_10_13=mainframe->M[10][13];
	m_10_14=mainframe->M[10][14];
	m_10_15=mainframe->M[10][15];
	m_10_16=mainframe->M[10][16];
	m_10_17=mainframe->M[10][17];
	m_10_18=mainframe->M[10][18];
	m_10_19=mainframe->M[10][19];
	m_11_0=mainframe->M[11][0];
	m_11_1=mainframe->M[11][1];
	m_11_2=mainframe->M[11][2];
	m_11_3=mainframe->M[11][3];
	m_11_4=mainframe->M[11][4];
	m_11_5=mainframe->M[11][5];
	m_11_6=mainframe->M[11][6];
	m_11_7=mainframe->M[11][7];
	m_11_8=mainframe->M[11][8];
	m_11_9=mainframe->M[11][9];
	m_11_10=mainframe->M[11][10];
	m_11_11=mainframe->M[11][11];
	m_11_12=mainframe->M[11][12];
	m_11_13=mainframe->M[11][13];
	m_11_14=mainframe->M[11][14];
	m_11_15=mainframe->M[11][15];
	m_11_16=mainframe->M[11][16];
	m_11_17=mainframe->M[11][17];
	m_11_18=mainframe->M[11][18];
	m_11_19=mainframe->M[11][19];
	UpdateData(false);
}


void CBlockView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

    MToControl();
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(false);
	
}

HBRUSH CBlockView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
		switch (nCtlColor)
	{
		case CTLCOLOR_STATIC: //静态文本
		{
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B = CreateSolidBrush(RGB(104,45,176)); //控件背景颜色
			pDC->SetTextColor(RGB(138 ,43, 226)); //控件中的文字的颜色
			return (HBRUSH) B;
		}
		case CTLCOLOR_LISTBOX://列表框
		{
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B = CreateSolidBrush(RGB(253,253,204));
			pDC->SetTextColor(RGB(255,0,0));
			return (HBRUSH) B;
		}
		case CTLCOLOR_DLG : //对话框
		{
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B = CreateSolidBrush(RGB(253,253,204));
			pDC->SetTextColor(RGB(139 ,105, 20 ));
			return (HBRUSH) B;
		}
		case CTLCOLOR_EDIT : //文本编辑框
		{
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B = CreateSolidBrush(RGB(253,253,204));
			pDC->SetTextColor(RGB(74,112,139));
			return (HBRUSH) B;
		}
		case CTLCOLOR_BTN : //button
		{
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B=CreateSolidBrush(RGB(253,253,204));
			pDC->SetTextColor(RGB(74 ,112, 139 ));
//			pDC->SetBkColor(RGB(171, 130, 255));
			return (HBRUSH) B;
		}
		default:
		{
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B = CreateSolidBrush(RGB(0, 60, 0));
			pDC->SetTextColor(RGB(255,0,0));
			return (HBRUSH) B;
		}
	}
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
