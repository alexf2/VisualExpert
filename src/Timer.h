//{{AFX_INCLUDES()
#include "calendar.h"
//}}AFX_INCLUDES
#if !defined(AFX_TIMER_H__3AA4B206_9286_11D1_AC95_D37CE18E0E33__INCLUDED_)
#define AFX_TIMER_H__3AA4B206_9286_11D1_AC95_D37CE18E0E33__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Timer.h : header file
//
#include <list>


#include "vs.rh"
#include "vs.hpp"
#include "../rudakow/servis/servis.hpp"
#include "../rudakow/servis/ind.hpp"

/////////////////////////////////////////////////////////////////////////////
// CTimer dialog

struct TTimeLbl
 {
   TTimeLbl( time_t t, LPCSTR lp ):
     m_str( lp ),
	 m_tm( t )
	  {
	  }

   time_t m_tm;
   CString m_str;
 };

class TMainFrame;

class CTimerDialog : public CDialog
{
// Construction
public:
	CTimerDialog( TMainFrame *pMW, CWnd* pParent = NULL );   // standard constructor

	//list<TAlertLebel*>& alertlist;

// Dialog Data
	//{{AFX_DATA(CTimerDialog)
	enum { IDD = IDD_TIMER };
	CButton	m_del;
	CListBox	m_eventlist;
	CCalendar	m_calendar;
	int		m_hours;
	CString	m_minutes;
	CString	m_message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    TMainFrame *m_pMW;
	// Generated message map functions
	//{{AFX_MSG(CTimerDialog)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEventlist();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMER_H__3AA4B206_9286_11D1_AC95_D37CE18E0E33__INCLUDED_)
