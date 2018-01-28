#if !defined(_VS_)
#define _VS_

#include "stdafx.h"
#include "..\\RUDAKOW\\SERVIS\\bwcc.hpp"

#include <afxwin.h>

class TExpertApp: public CWinApp
 {
public:
  TExpertApp();  

  virtual BOOL InitInstance();
  virtual BOOL OnIdle( LONG lCount );

  afx_msg void OnAbout();  

  TBWCC bwcc; 

  LPCSTR GetStartPath() const { return m_Startpath; }
  LPCSTR GetDescrPath() const { return m_Descriptpath; }
  
  virtual void WinHelp( DWORD dwData, UINT nCmd = HELP_CONTEXT );

private:  
  CString m_Startpath;
  CString m_Descriptpath;
  

  DECLARE_MESSAGE_MAP()
 };

class TMyDocManeger: public CDocManager
 {

   DECLARE_DYNCREATE( TMyDocManeger )

public:
  TMyDocManeger(): CDocManager() {};

  virtual ~TMyDocManeger();

  virtual BOOL DoPromptFileName( CString& fileName, UINT nIDSTitle,
                                 DWORD lFlags, BOOL bOpenFileDialog, 
								 CDocTemplate* pTempl
							    );	 
 };

class TMyDocTemplate: public CMultiDocTemplate
 {

  DECLARE_DYNAMIC(TMyDocTemplate)

public:
  TMyDocTemplate( UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass ):
    CMultiDocTemplate( nIDResource, pDocClass, pFrameClass, pViewClass )
	 {
	 }

  virtual ~TMyDocTemplate() {};

  virtual void AddDocument( CDocument* pDoc );
  virtual void RemoveDocument( CDocument* pDoc );
  
  virtual BOOL GetDocString( CString& rString, enum DocStringIndex index ) const;

  //virtual void LoadTemplate();

private:

 };

#endif
