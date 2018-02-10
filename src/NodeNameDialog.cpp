// NodeNameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "vs.hpp"
#include "NodeNameDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeNameDialog dialog


CNodeNameDialog::CNodeNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNodeNameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeNameDialog)
	m_nodename = _T("");
	//}}AFX_DATA_INIT
}


void CNodeNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeNameDialog)
	DDX_Text(pDX, IDC_NODENAME, m_nodename);
	DDV_MaxChars(pDX, m_nodename, 250);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeNameDialog, CDialog)
	//{{AFX_MSG_MAP(CNodeNameDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeNameDialog message handlers
