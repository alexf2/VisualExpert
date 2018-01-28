#if !defined(AFX_QUESTIONDIALOG_H__1CB32801_A0D0_11D1_94CA_444553540000__INCLUDED_)
#define AFX_QUESTIONDIALOG_H__1CB32801_A0D0_11D1_94CA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// QuestionDialog.h : header file
//

#include "net.h"
#include "vs.rh"
#include "v_net.hpp"


/////////////////////////////////////////////////////////////////////////////
// CQuestionDialog dialog

class CQuestionDialog : public CDialog
{
// Construction
public:
	TNetView* NetView;
	CNet* net;
	CQuestionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQuestionDialog)
	enum { IDD = IDD_QUESTION };
	CComboBox	m_questions;
	CComboBox	m_nodes;
	CComboBox	m_lines;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuestionDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTIONDIALOG_H__1CB32801_A0D0_11D1_94CA_444553540000__INCLUDED_)
