#if !defined(_CRE_DATABASE_)
#define _CRE_DATABASE_

#include "stdafx.h"
//#include "autorc.hpp"
#include "vessubj.hpp"

#include <afxdao.h>

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"
#include "..\\RUDAKOW\\SERVIS\\subcode.hpp"

struct TMyFieldInfo
 {
   LPCSTR m_strName;
   short m_nType;               
   long m_lSize;                
   long m_lAttributes;          
 };

enum FORMATEXC { FRM_BadDBFormat = 0, FRM_Unknown = 1, FRM_SQLErr = 2 };
                      
class TFormatException: public TMyException
 {

  enum { TSC_Subcode = TREG_TFormatException }; //! - у каждого субкласса должен быть свой

public:
  TFormatException( BOOL bFlDel, int iExcCode, LPSTR lpName );    

  virtual int GetSubCode() const;
 };

void ThrowBadDBFormat( LPCSTR lpNam );
void ThrowSQLErr( LPCSTR lpErr );

void CreateTable( CDaoTableDef& rTbl, TMyFieldInfo *pFinf );
void CreateMyDB( LPCSTR lpPath, LPCSTR lpName, TProtectedObj *pObj, TMyFieldInfo *pClm );
void CheckDBFormat( LPCSTR lpPath, LPCSTR lpNameTable, TMyFieldInfo *pFinf, TProtectedObj *pObj );

void RegisterSource( LPCSTR lpPath, LPCSTR lpNameSrc, bool bFlRemove );

void CheckSQLCode( int retcode );

class TTempDSN
 {
public:
  TTempDSN() { m_bFlInit = false; };
  ~TTempDSN();

  void Create( LPCSTR lpPath, LPCSTR lpNameSrc );   
  void Remove();

private:
  CString m_strPath, m_strDSN;
  bool m_bFlInit;
 };

#endif
