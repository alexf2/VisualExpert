#if !defined(AFX_NODENAMEDIALOG_H__D048BCA1_9A3F_11D1_94CA_444553540000__INCLUDED_)
#define AFX_NODENAMEDIALOG_H__D048BCA1_9A3F_11D1_94CA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NodeNameDialog.h : header file
//

#include "vs.rh"

/////////////////////////////////////////////////////////////////////////////
// CNodeNameDialog dialog

class CNodeNameDialog : public CDialog
{
// Construction
public:
	CNodeNameDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNodeNameDialog)
	enum { IDD = IDD_NODENAME };
	CString	m_nodename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeNameDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeNameDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODENAMEDIALOG_H__D048BCA1_9A3F_11D1_94CA_444553540000__INCLUDED_)
