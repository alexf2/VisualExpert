#if !defined(_NAT_AND_FRAME_SUBJECT_)
#define _NAT_AND_FRAME_SUBJECT_

#include "stdafx.h"
#include "autorc.hpp"


#include "userlvl.hpp"
#include "coding.hpp"

#include "subj.hpp"

#include <list>
using namespace std;

class TNet;
class TPro;
void CompileNet( TNet*, TPro*, LPCSTR lpFullPathPro, LPCSTR lpFullPathNet ); 
								

class TNet: public TSubject
 {
friend CArchive& AFXAPI operator<<( CArchive& rA, TNet& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TNet& );

public:
  TNet();
  TNet( TProtectedObj *pPar, LPCSTR lpShortPath );
  virtual ~TNet();

  virtual TP_ClassTyp IsA() const;
  virtual void CreateEmtyFile( LPCSTR );
  virtual LPCSTR MyName() const;

  virtual void ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath );
  virtual bool RemoveObj( LPCSTR lpPathES, int iAction );
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  virtual void CheckDBFormat( LPCSTR lpPath );
  
 };

inline CArchive& AFXAPI operator<<( CArchive& rA, TNet& rAc )
 {
   rA << *(TSubject*)&rAc;
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TNet& rAc )
 {
   rA >> ((TSubject&)rAc);   
   return rA;
 }


class TFrm: public TSubject
 {
friend CArchive& AFXAPI operator<<( CArchive& rA, TFrm& );
friend CArchive& AFXAPI operator>>( CArchive& rA, TFrm& );

public:
  TFrm();
  TFrm( TProtectedObj *pPar, LPCSTR lpShortPath );
  virtual ~TFrm();

  virtual TP_ClassTyp IsA() const;
  virtual void CreateEmtyFile( LPCSTR );
  virtual LPCSTR MyName() const;

  virtual void ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath );
  virtual bool RemoveObj( LPCSTR lpPathES, int iAction );
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление

  virtual void CheckDBFormat( LPCSTR lpPath );
  
 };

inline CArchive& AFXAPI operator<<( CArchive& rA, TFrm& rAc )
 {
   rA << *(TSubject*)&rAc;   
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TFrm& rAc )
 {
   rA >> ((TSubject&)rAc);   
   return rA;
 }


#endif
