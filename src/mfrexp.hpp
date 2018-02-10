#if !defined(_MFREXP_)
#define _MFREXP_

#include "stdafx.h"
#include "autorc.hpp"
#include "vs.rh"

#include "userlvl.hpp"
#include "coding.hpp"
#include "dlgach.hpp"
#include "fr_opt.hpp"

#include "..\\RUDAKOW\\SERVIS\\mymenu.hpp"
#include "..\\RUDAKOW\\SERVIS\\ind.hpp"

#define ID_CALC  1
#define ID_ALERT 2


class TMainFrame: public CMDIFrameWnd
 {

friend void ExecAlert( TAlertLebel*, void* );
friend class CTimerDialog;

protected: 	
	DECLARE_DYNCREATE( TMainFrame )

public:	
    TMainFrame();
	virtual ~TMainFrame();

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	void SetupMenu( HMENU hMenu );

	TProtectedObj* GetProtectLvl() { return &m_protAcessLvl; }
	TUserLocal* GetCurUser() { return (TUserLocal*)*m_protAcessLvl.m_lstUsers.begin(); }

	TMFOptions* GetOptPtr() { return &m_optOptions; }

protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnClose();	
	afx_msg void OnDestroy();
	
	afx_msg void OnAbout();
	afx_msg void OnAcess();
    afx_msg void OnUsers();
	
	afx_msg void OnAcessCurrent();

	afx_msg void OnAppWindow();
	afx_msg void OnCloseAll();
	afx_msg void OnNext();	
	afx_msg void OnPriv();
		
	afx_msg LONG OnCalc( UINT, LONG );
	afx_msg LONG OnClock( UINT, LONG );

	afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged( CWnd* pFocusWnd );
		

    afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );

	void OnUpdateUsers( CCmdUI* ); 
    void OnUserWnd( CCmdUI* );  	
	void OnAlways( CCmdUI* );  	
	void OnUpdateAcess( CCmdUI* pCmdUI );

	void DefaultPos();

	void SetNameFrom( TPRopPageUser& rPage, TProtectedObj& rProtObj );
	void SetPasswordFrom( TPRopPageUser& rPage, TProtectedObj& rProtObj  );
	void SetPasswordFrom( TPRopPageAdm& rPage, TProtectedObj& rProtObj );

    int FindUser( TUser&, TUserLocal**, bool bFlAsk = true );

	TProtectedObj m_protAcessLvl;	
	void SaveUserCfg();

	TAutoMemoryRc<TClickBmp*> m_arcCalc;
	TAutoMemoryRc<TClickBmpWithClock*> m_arcTimeAlert;
	
private:  
	TStatusWithLamp  m_wndStatusBar;
	int m_iCountRePass;

	static UINT m_iMsgCalc, m_iMsgClock;		

	TAutoMemoryRc<TMSStyleTooolBar*> m_arcTool;
	//TAutoMemoryRc<CBitmap*> m_MenuBmp;
	CBitmap* m_parrMenuBmp;

	TMFOptions m_optOptions;
	bool m_bFlLoadingFrame;
	bool m_bFlShow;

	int m_iCurAcessSheetIndex;

	
	void CreateGlobalConfig();
    void LoadGlobalCongig();    
	int LoadConfigOnStart();

	void SaveGlobalConfig();
	void KeepScreenConfig();

	void CreateDefaultUsers();


	DECLARE_MESSAGE_MAP()
};


#endif
