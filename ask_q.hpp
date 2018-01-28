#if !defined(_ASK_DLG_Q_)
#define _ASK_DLG_Q_

#include "stdafx.h"
#include "warea.hpp"


class TStaDlg: public CDialog
 {
public:
  TStaDlg( CWnd *pPar, int id, TWorkArea *pW ):     
    CDialog( id, pPar )
   {         
     m_pW = pW;
	 m_bFlRemove = true;
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  virtual void OnOK();

  int m_iGoal;
  bool m_bFlRemove;

private:    
  TWorkArea *m_pW;
  CListBox m_lbLst;  
  CButton m_btnChk;

  void FillList();
 };


#endif
