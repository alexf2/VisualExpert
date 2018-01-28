#if !defined(_MDIEXP_)
#define _MDIEXP_

#include "stdafx.h"
//#include "docexp.hpp"
#include "autorc.hpp"

#include "..\\RUDAKOW\\SERVIS\\borctl.hpp"
#include "..\\RUDAKOW\\SERVIS\\dibapi.h"

#include "viewexp.hpp"

class CRcFile: public CFile
 {
public:
  CRcFile( HINSTANCE hinst, UINT uiIdRc, LPCSTR lpName );

  virtual ~CRcFile();

  virtual UINT Read( void* lpBuf, UINT nCount );  
  virtual DWORD GetLength() const;

private:
  HINSTANCE m_hHan;
  HRSRC m_hrc;
  HGLOBAL m_hg;
  LPSTR m_lpRc;
  DWORD m_dwSizeRc;

  LPSTR m_lpCuros;
  DWORD m_dwCnt;
  bool m_bEOF;
 };



#define MUL_DSTBUTTONS 2
struct TBtnItem //pPrivBtn устанавливается CMyDialogBar 
 {
   TBtnItem( CWnd *pBtn, bool bFixedDst )
	{
	  m_pBtn = pBtn; 
	  m_bFixedDst = bFixedDst;
	  m_pPrivBtn = NULL;
	}

   CWnd *m_pBtn;
   CWnd *m_pPrivBtn;
   bool m_bFixedDst;

   CRect r;
   int m_iW;
   int m_iH;
 };

class CMyDialogBar: public CDialogBar
 {

friend class TMDIFrame;

public:
  CMyDialogBar( bool bFlHoriz ): CDialogBar()
   {
     m_bFlHoriz = bFlHoriz;
	 m_imaxBounds = 50;
     m_vievsCurrent = TV_Expert;
   }

  virtual ~CMyDialogBar();

  CMyDialogBar& operator+( TToolTipData *pDta )
   {
     m_vecToolTips.push_back( pDta );
	 return *this;
   }

  CMyDialogBar& operator+( TBtnItem *pBtnIt );
  CMyDialogBar& operator+( TBtnItem &rBtnIt )
   {
     return operator+( &rBtnIt );
   }

  void InsertHead( TBtnItem &rBtnIt );
  

  void ClearBtns();
  
  
  afx_msg void OnDestroy();

  virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );

  afx_msg BOOL OnEraseBkgnd( CDC* );
  afx_msg void OnSize( UINT nType, int cx, int cy );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );  
  //afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
  
  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );

  void MyLayout( bool bRedr, bool bFlCallDirectly = false, int iBnd = 0 );

  virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

  LONG QPalette( LONG );

  virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

  void SetMaxBounds( int iMax ) { m_imaxBounds = iMax; }
  int GetMaxBounds() const { return m_imaxBounds; }
  bool IsHoris() const { return m_bFlHoriz; }

  bool TextureIsOn() const;

  void SetCurVievClass( TV_Views vievsCurrent )
  {
    m_vievsCurrent = vievsCurrent;
  }

private:
  list<TToolTipData *> m_vecToolTips;
  bool m_bFlHoriz;
  //CBrush m_brBkGnd;

  int m_imaxBounds;

  TV_Views m_vievsCurrent;

  static int m_iW, m_iH;
  static CPalette m_palPal;
  static HDIB m_hrcBmp;
  static int m_iUseCnt;
  static CRect m_rS;

  static int m_iWv, m_iHv;
  static CPalette m_palPalv;
  static HDIB m_hrcBmpv;
  static int m_iUseCntv;
  static CRect m_rSv;

  list<TBtnItem*> m_lstBtn;
 

  DECLARE_MESSAGE_MAP()
 };

enum TSWT_Codes { TSWT_Explorer, TSWT_Rules, TSWT_Facts, TSWT_Q, 
                  TSWT_Run, TSWT_Net, TSWT_Frm };

class TMDIFrame: public CMDIChildWnd
 {
   DECLARE_DYNCREATE( TMDIFrame )

public:
   TMDIFrame();
   virtual ~TMDIFrame();

   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
   afx_msg BOOL OnEraseBkgnd( CDC* );
   

   afx_msg LONG OnGlobalQPal( UINT, LONG );
   afx_msg LONG OnGlobalPalCh( UINT, LONG );
   afx_msg LONG OnSwtVievs( UINT, LONG );

   
   void SwitchOnVerBar( bool bFl = true );

   TBaseView* ChangeView( TV_Views tvToActivate, TProtectedObj *pObj = NULL );

   void Setup_DirectlyView( TBaseView *pV )
	{
	  pV->MakeBindSubj( ((TExpertDoc*)(pV->GetDocument()))->GetESystem() );
      pV->SetupHorizBar( m_dbarHor );
      pV->SetupVerBar( m_dbarVer );
	  RecalcLayout();
	  ChangeTitle();
	}

   virtual BOOL DestroyWindow();

   void ChangeTitle();

   CMyDialogBar* GetHorDlgBarPtr()  { return &m_dbarHor; }

protected:
   CMyDialogBar m_dbarHor;
   CMyDialogBar m_dbarVer;

   DECLARE_MESSAGE_MAP()
 };


#endif
