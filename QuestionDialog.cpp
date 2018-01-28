// QuestionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "vs.hpp"
#include "QuestionDialog.h"
#include "v_net.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuestionDialog dialog


CQuestionDialog::CQuestionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuestionDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    NetView = ((TNetView*)pParent);
    net = NetView->net;
}


void CQuestionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestionDialog)
	DDX_Control(pDX, IDC_QUESTION, m_questions);
	DDX_Control(pDX, IDC_NODE, m_nodes);
	DDX_Control(pDX, IDC_LINE, m_lines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuestionDialog, CDialog)
	//{{AFX_MSG_MAP(CQuestionDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuestionDialog message handlers

BOOL CQuestionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    for (__int32 i=0; i<net->lines.GetSize(); i++ ) {
        m_lines.AddString(net->lines[i].name);
    }
    for (i=0; i<net->nodes.GetSize(); i++ ) {
        m_nodes.AddString(net->nodes[i].name);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuestionDialog::OnOK() 
{


	NetView->SolveQuestion( m_lines.GetCurSel(), m_nodes.GetCurSel() );

	//CDialog::OnOK();
}
