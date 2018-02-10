#if !defined(AFX_TCALCDIALOG_H__2A199180_90C9_11D1_AC95_444553540000__INCLUDED_)
#define AFX_TCALCDIALOG_H__2A199180_90C9_11D1_AC95_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TCalcDialog.h : header file
//
#include <afxcmn.h>

#include "stdafx.h"
#include "vs.rh"
#include "OwnerDrawBox.h"

/////////////////////////////////////////////////////////////////////////////
// TCalcDialog dialog

class TVariantVar;

class TCalcDialog : public CDialog
{
// Construction
public:
	TCalcDialog(CWnd* pParent = NULL);   // standard constructor

	int m_iPrec, m_iShowMode;
// Dialog Data
	//{{AFX_DATA(TCalcDialog)
	enum { IDD = IDD_CALC };
	COwnerDrawBox	m_resultsbox;
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TCalcDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	afx_msg void OnDestroy();
	afx_msg void OnDblClk();
	afx_msg void OnRadio( UINT );
	virtual void PostNcDestroy();

	virtual BOOL PreTranslateMessage( MSG* pMsg );	

// Implementation
protected:
    CBitmap m_bmpClear, m_bmpYes, m_bmpNo;
	CButton m_btnClear, m_btnYes, m_btnNo;
	CSliderCtrl m_sld;	

	// Generated message map functions
	//{{AFX_MSG(TCalcDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnCount();
	afx_msg void OnClear();
	virtual void OnCancel();
	//}}AFX_MSG

	int Ask( TVariantVar* pVar );

	HICON m_hIcCalc;
	int m_kX, m_kY;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCALCDIALOG_H__2A199180_90C9_11D1_AC95_444553540000__INCLUDED_)
