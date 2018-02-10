#if !defined(_DOCEXP_)
#define _DOCEXP_

#include "stdafx.h"
#include "docexp.hpp"
#include "autorc.hpp"
#include "vessubj.hpp"

#include "warea.hpp"

#include "vs.rh"

class TSubject;


class TExpertDoc: public CDocument
 {

protected: 
	TExpertDoc();
	DECLARE_DYNCREATE( TExpertDoc )
    
    virtual BOOL OnNewDocument();

public:
  virtual ~TExpertDoc();
  virtual void Serialize( CArchive& ar );   

  CSize GetDocSize() const
   { return m_sizeDoc; }

  virtual void DeleteContents();

  virtual BOOL OnSaveDocument( LPCTSTR lpszPathName );
  virtual BOOL OnOpenDocument( LPCTSTR lpszPathName );
  virtual void OnCloseDocument();
  virtual void SetTitle( LPCTSTR lpszTitle );
  void SetTitleMy( CString&, bool bFlCallOutside = false );
  
  virtual BOOL CanCloseFrame( CFrameWnd* pFrame );
  
  TExpertSystem* GetESystem() { return m_arcES(); }

  TSubject *m_pSubjKey;

  TAutoMemoryRc<TWorkArea*> arcWA;

protected:
  virtual CFile* GetFile( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError );  
  virtual void ReportSaveLoadException( LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault );
  virtual void ReleaseFile( CFile* pFile, BOOL bAbort );

private:
  CSize m_sizeDoc;

  TAutoMemoryRc<TExpertSystem*> m_arcES;

  bool CreateES( bool bAsk );
  

  DECLARE_MESSAGE_MAP()
 };


#endif
