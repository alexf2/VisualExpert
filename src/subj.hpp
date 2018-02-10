#if !defined(_BASE_SUBJECT_)
#define _BASE_SUBJECT_

#include "stdafx.h"
#include "autorc.hpp"

#include "userlvl.hpp"
#include "coding.hpp"

#include <list>
using namespace std;

enum TP_ClassTyp { TPC_TSubject, TPC_TNet, TPC_TFrm, TPC_TFac,
                   TPC_TQuery, TPC_TPro };

class TSubject: public TProtectedObj
 {

friend CArchive& AFXAPI operator<<( CArchive& rA, TSubject& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TSubject& );

public: 
  TSubject();
  TSubject( TProtectedObj *pPar, LPCSTR lpShortPath );

  virtual ~TSubject();

  bool IsDirty() const { return m_fDirty; }
  void SetDirty( bool bFl = true ) { m_fDirty = bFl; }

  void SetName( LPCSTR lp ) { m_csNameObj = lp; }
  LPCSTR GetName() const { return m_csNameObj; }
  LPCSTR GetPath() const { return m_csSPath; }

  void SetSel( bool bFl ) { m_bSelected = bFl; }
  bool GetSel() const { return m_bSelected; }

  virtual TP_ClassTyp IsA() const = 0;
  virtual void CreateEmtyFile( LPCSTR ) = 0;
  virtual LPCSTR MyName() const = 0;

  virtual void ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath );
  virtual bool RemoveObj( LPCSTR lpPathES, int iAction );
   //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  virtual void CheckDBFormat( LPCSTR lpPath ) = 0;

protected:
  CString m_csSPath;
  CString m_csNameObj;
  bool m_fDirty;

  bool m_bSelected;
 };

inline CArchive& AFXAPI operator<<( CArchive& rA, TSubject& rAc )
 {   
   rA << *(TProtectedObj*)&rAc;
   rA << rAc.m_csSPath << rAc.m_csNameObj;
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TSubject& rAc )
 {   
   rA >> ((TProtectedObj&)rAc);
   rA >> rAc.m_csSPath >> rAc.m_csNameObj;
   return rA;
 }


#endif
