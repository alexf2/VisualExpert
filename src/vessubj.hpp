#if !defined(_SUBJECT_)
#define _SUBJECT_

#include "stdafx.h"
#include "autorc.hpp"


#include "userlvl.hpp"
#include "coding.hpp"

#include "subj.hpp"
#include "netfsubj.hpp"

#include <list>
using namespace std;


class TFac: public TSubject
 {
friend CArchive& AFXAPI operator<<( CArchive& rA, TFac& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TFac& );

public:
  TFac();
  TFac( TProtectedObj *pPar, LPCSTR lpShortPath );
  virtual ~TFac();  

  virtual TP_ClassTyp IsA() const;
  virtual void CreateEmtyFile( LPCSTR );
  virtual LPCSTR MyName() const;

  virtual void ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath );
  virtual bool RemoveObj( LPCSTR lpPathES, int iAction );
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  virtual void CheckDBFormat( LPCSTR lpPath );

protected:  
 };

inline CArchive& AFXAPI operator<<( CArchive& rA, TFac& rAc )
 {
   rA << *(TSubject*)&rAc;   
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TFac& rAc )
 {
   rA >> ((TSubject&)rAc);   
   return rA;
 }


class TQuery: public TSubject
 {
friend CArchive& AFXAPI operator<<( CArchive& rA, TQuery& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TQuery& );

public:
  TQuery();
  TQuery( TProtectedObj *pPar, LPCSTR lpShortPath );
  virtual ~TQuery();  

  virtual TP_ClassTyp IsA() const;
  virtual void CreateEmtyFile( LPCSTR );
  virtual LPCSTR MyName() const;

  virtual void ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath );
  virtual bool RemoveObj( LPCSTR lpPathES, int iAction );
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  virtual void CheckDBFormat( LPCSTR lpPath );

protected:
 };

inline CArchive& AFXAPI operator<<( CArchive& rA, TQuery& rAc )
 {
   rA << *(TSubject*)&rAc;   
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TQuery& rAc )
 {
   rA >> ((TSubject&)rAc);   
   return rA;
 }


class TPro: public TSubject
 {
friend CArchive& AFXAPI operator<<( CArchive& rA, TPro& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TPro& );
friend class TExpertView;
friend class TExpertSystem;
friend CArchive& AFXAPI operator>>( CArchive& rA, TExpertSystem& rAc );
friend class TWorkArea;

public:
  TPro();
  TPro( TProtectedObj *pPar, LPCSTR lpShortPath );
  virtual ~TPro();

  virtual TP_ClassTyp IsA() const;
  virtual LPCSTR MyName() const;

  int Source() const { return m_iFromCompiled; }
  void SetSource( int iS ) { m_iFromCompiled = iS; }
  virtual void CreateEmtyFile( LPCSTR );

  virtual void ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath );
  virtual bool RemoveObj( LPCSTR lpPathES, int iAction );
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  virtual void CheckDBFormat( LPCSTR lpPath );

protected:
  int m_iFromCompiled; //0 - self, 1 - from net, 2 - from frame

  TAutoMemoryRc<TQuery*> m_arcQ;
  TAutoMemoryRc<TFac*> m_arcF;
 };


template<class T> void ClearList( T& rL );

class TExpertSystem: public TProtectedObj
 {

friend class TExpertDoc;
friend class TExpertView;
friend CArchive& AFXAPI operator<<( CArchive& rA, TExpertSystem& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TExpertSystem& );

public:
  TExpertSystem();
  TExpertSystem( TProtectedObj *pPar, LPCSTR lpPath, LPCSTR lpName );

  virtual ~TExpertSystem();

  LPCSTR GetName() const { return m_csName; }  
  void AssPath( LPCSTR lp ) { m_csPath = lp; }
  void AddVSubj( TSubject* pToAdd, TSubject* pParent = NULL );

  void LoadHdrOnly( CArchive& rA );

  void SetModify( bool bFl = true ) { m_bModified = bFl; }
  bool IsModified() const { return m_bModified; }

  bool RemoveObj( TSubject* pSubj, int iAction );
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  void GetFullObjPath( CString&, TSubject* );

protected:
  bool m_bModified;
  CString m_csName;
  CString m_csPath;

  //void ClearList( list<TSubject*>& );  

  void CheckExist( TSubject* );
  
  list<TPro*> m_lstPro;
  list<TNet*> m_lstNet;
  list<TFrm*> m_lstFrm;

  bool m_bWarnOnLoad;
 };


#endif
