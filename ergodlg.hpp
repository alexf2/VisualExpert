#if !defined(_ERGO_DLG_)
#define _ERGO_DLG_

#include "stdafx.h"
#include "autorc.hpp"
#include "vs.rh"

#include "userlvl.hpp"

using namespace std;
#include <list>

class TColorCombo: public CComboBox
 {
public:
  TColorCombo(): CComboBox()
   {
   }

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
  virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
 };


class TErgoCategory
 {
public:
   TErgoCategory( LPCSTR lpName ): m_strName( lpName )
	{
	}

   TErgoCategory( TErgoCategory &rC );
   ~TErgoCategory();

   TErgoCategory& operator+( TErgoItem* pIt )
	{
      m_lst.push_back( pIt );
	  return *this;
	}

   TErgoCategory& operator=( TErgoCategory& rC );

   list<TErgoItem*> m_lst; //don't owned
   list<TErgoItem*> m_lstDup;//ownn
   CString m_strName;
 };


class TErgView: public CStatic
 {
public:
  TErgView(): CStatic() 
   {
     m_pIt = NULL;
   }

  afx_msg BOOL OnEraseBkgnd( CDC* pDC );
  afx_msg void OnPaint();

  TErgoItem *m_pIt;
  TUserLocal *m_pUsl;

  DECLARE_MESSAGE_MAP()
 };



class TErgoDlg: public CDialog
 {
public:
  TErgoDlg( CWnd *pPar ):     
    CDialog( IDD_DLG_ERGONOMIC, pPar )	
   {              
     m_bChangedFont = false;
   }

  virtual ~TErgoDlg();

  TErgoDlg& operator+( TErgoCategory *pCat )
   {
     m_lst.push_back( pCat );
	 return *this;
   }

  virtual void OnOK();

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );

  afx_msg void OnSelCat();
  afx_msg void OnSelIt();
  afx_msg void OnComboBk();
  afx_msg void OnComboF();

  afx_msg void OnFont();

  bool m_bChangedFont;
  
private:        
   list<TErgoCategory*> m_lst; //owned
   list<TErgoCategory*> m_lstDup; //owned

   void FillCat();
   void FillIt( int iCat );

   CListBox m_lstCat, m_lstIt;
   TColorCombo m_cbnBk, m_cbnFore;
   CButton m_btnFont;

   TErgView m_eView;

   void Dup();

   DECLARE_MESSAGE_MAP()
 };

#endif
