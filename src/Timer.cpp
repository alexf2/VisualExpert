// Timer.cpp : implementation file
//

#include "stdafx.h"
#include "vs.hpp"
#include "Timer.h"
#include "mfrexp.hpp"

#include "..\\rudakow\\servis\\ind.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimerDialog dialog

extern void ExecAlert( TAlertLebel* pAlertLbl, void* pExtraData );

CTimerDialog::CTimerDialog(TMainFrame *pMW, CWnd* pParent /*=NULL*/)
	: CDialog(CTimerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimerDialog)
	m_hours = -1;
	m_minutes = _T("");
	m_message = _T("");
	//}}AFX_DATA_INIT

	m_pMW = pMW;

	//alertlist = (TMainFrame*)pParent->m_arcCalc->GetReflistAlert();
}


void CTimerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimerDialog)
	DDX_Control(pDX, IDC_DELETE, m_del);
	DDX_Control(pDX, IDC_EVENTLIST, m_eventlist);
	DDX_Control(pDX, IDC_CALENDAR, m_calendar);
	DDX_CBIndex(pDX, IDC_HOURS, m_hours);
	DDX_CBString(pDX, IDC_MINUTES, m_minutes);
	DDV_MaxChars(pDX, m_minutes, 2);
	DDX_Text(pDX, IDC_MESSAGE, m_message);
	//}}AFX_DATA_MAP

    unsigned int mins;
    sscanf(m_minutes, "%u", &mins );

    DDV_MinMaxInt(pDX, mins, 0, 59);
}


BEGIN_MESSAGE_MAP(CTimerDialog, CDialog)
	//{{AFX_MSG_MAP(CTimerDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_EVENTLIST, OnSelchangeEventlist)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimer message handlers

void CTimerDialog::OnCancel() 
 {	
   CDialog::OnCancel();

   //DestroyWindow();
 }

void CTimerDialog::OnOK() 
 {		
   CDialog::OnOK();	
 }

void CTimerDialog::OnAdd() 
{
	if (!UpdateData()) return;
    
    unsigned __int16 day = m_calendar.GetDay();
	unsigned __int16 month = m_calendar.GetMonth();
	unsigned __int16 year = m_calendar.GetYear();
	unsigned __int16 hour = m_hours;
    unsigned int minute;
    sscanf(m_minutes, "%u", &minute );

    CTime t(year, month, day, hour, minute, 0);    
    int i = m_eventlist.AddString( t.Format( "%d %b %Y %H:%M" ) );

    TTimeLbl *pTL = new TTimeLbl( t.GetTime(), m_message );
    m_eventlist.SetItemData( i, (DWORD)pTL );
		
    /*char* pMessage = new char[m_message.GetLength()+1];
    strcpy( pMessage, m_message );
    m_eventlist.SetItemDataPtr( i, pMessage );
*/
    m_del.EnableWindow( TRUE );
}

void CTimerDialog::OnDelete() 
{
    __int32 i = m_eventlist.GetCurSel();

    delete (TTimeLbl*)m_eventlist.GetItemData( i );

	m_eventlist.DeleteString( i );

    if (m_eventlist.GetCount() == 0) m_del.EnableWindow( FALSE );
}

BOOL CTimerDialog::OnInitDialog() 
 {
	CDialog::OnInitDialog();
	
	CTime t = CTime::GetCurrentTime();

    m_hours = t.GetHour();
    m_minutes = t.Format("%M");

    UpdateData( FALSE );

	m_calendar.SetMonth( t.GetMonth() );
	m_calendar.SetDay( t.GetDay() );

	CenterWindow();

	list<TAlertLebel*> &rLst = m_pMW->m_arcTimeAlert()->GetReflistAlert();
	list<TAlertLebel*>::iterator itSta( rLst.begin() );
	list<TAlertLebel*>::iterator itEnd( rLst.end() );

	for( ; itSta != itEnd; itSta++ )
	 {	   
	   CTime t( (*itSta)->GetTime() );    
	   int i = m_eventlist.AddString( t.Format( "%d %b %Y %H:%M" ) );

	   TTimeLbl *pTL = new TTimeLbl( t.GetTime(), (*itSta)->GetMsg() );
       m_eventlist.SetItemData( i, (DWORD)pTL );

	   /*m_eventlist.SetItemData( i, t.GetTime() );

	   char* pMessage = new char[strlen((*itSta)->GetMsg())+1];
	   strcpy( pMessage, m_message );
	   m_eventlist.SetItemDataPtr( i, pMessage );*/
	 }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
 }

void CTimerDialog::OnSelchangeEventlist() 
{
    __int32 i = m_eventlist.GetCurSel();

    m_message = ((TTimeLbl*)m_eventlist.GetItemData( i ))->m_str;

    UpdateData( FALSE );
}

void CTimerDialog::OnDestroy() 
{
	//list* l = (TMainFrame*)GetParent()->m_arcTimeAlert();

    //l->clear();
    list<TAlertLebel*> &rLst = m_pMW->m_arcTimeAlert()->GetReflistAlert();
	list<TAlertLebel*>::iterator itSta( rLst.begin() );
	list<TAlertLebel*>::iterator itEnd( rLst.end() );

	for( ; itSta != itEnd; itSta++ )
	  delete *itSta;

	rLst.clear();
	

    for (__int32 i = 0; i<m_eventlist.GetCount(); i++) 
	 {      
        TAlertLebel *pLabel = new TAlertLebel( 
		   ((TTimeLbl*)m_eventlist.GetItemData( i ))->m_str,
           ((TTimeLbl*)m_eventlist.GetItemData( i ))->m_tm,
           0,
           TAlertLebel::TA_Normal,
           ExecAlert,
           m_pMW );
	   

		rLst.push_back( pLabel );
        
	/*TAlertLebel( LPCSTR lpMsg, time_t tim, time_t dtim, 
	 TA_Types typ, 
               TAlertExecuteFunc_Ptr pFunc, void* pExtra ):*/
     }
    
    CDialog::OnDestroy();

    for (i = 0; i<m_eventlist.GetCount(); i++) delete (TTimeLbl*)m_eventlist.GetItemData( i );
}
