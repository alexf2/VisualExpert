#if !defined(AFX_OWNERDRAWBOX_H__FD232BB2_924E_11D1_AC95_D7831836D837__INCLUDED_)
#define AFX_OWNERDRAWBOX_H__FD232BB2_924E_11D1_AC95_D7831836D837__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OwnerDrawBox.h : header file
//
#include <afxtempl.h>

enum ESide {
	left_,
	right_
};

/////////////////////////////////////////////////////////////////////////////
// SAttributes

struct SAttributes {
	BOOL bold;
	COLORREF color;
	CString str;
};

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBox window

class COwnerDrawBox : public CListBox
{
// Construction
public:
	COwnerDrawBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerDrawBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	ESide Align;
	int AddString( LPCTSTR str, COLORREF color=0, BOOL bold=false );
	virtual ~COwnerDrawBox();
	CArray<SAttributes, SAttributes&> ArrayOfAttr;

	// Generated message map functions
protected:
	CFont* m_boldfont;
	int m_sz;

	//{{AFX_MSG(COwnerDrawBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWBOX_H__FD232BB2_924E_11D1_AC95_D7831836D837__INCLUDED_)
