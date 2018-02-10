#if !defined(_PRODUCT_)
#define _PRODUCT_

#include "stdafx.h"
#include "autorc.hpp"
#include "var.hpp"

#include <vector>
#include <list>
using namespace std;

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

//Реестр субкодов исключений TMyException !!!
#include "..\\RUDAKOW\\SERVIS\\subcode.hpp"

class TExpressionFram
 {
public:
  TExpressionFram() 
   {
     m_dKD = 1;	 
   }  

  void Create( LPCSTR lpExpr, TNameSpace *pSpc, double dKD );

  TAutoMemoryRc<TExpression*> m_arcExpr;  
  CString m_strExpr;
  double m_dKD;
 };


void ThrowNeedKD( LPCSTR lpProd );
void ThrowInvalidKD( LPCSTR lpProd );

enum SPEXC { SPE_Unknown = 0, SPE_NeedKD = 1, SPE_InvalidKD = 2,
             SPE_NeedEQU = 3, SPE_OnlyOneEQU = 4, SPE_NeedKDFac = 5,
			 SPE_InvalValue = 6, SPE_VarAlradyInit = 7,
			 SPE_BadQList = 8, SPE_NeedVarName = 9,
			 SPE_KDRange = 10, SPE_DisblEQU = 11
           };
        
class TSPException: public TMyException
 {

  enum { TSC_Subcode = TREG_TSPException }; //! - у каждого субкласса должен быть свой

public:
  TSPException( BOOL bFlDel, int iExcCode, LPSTR lpName );    

  virtual int GetSubCode() const;
 };


class TSimpleProduct
 {

friend class TWorkView;
friend class TResult;

public:
  TSimpleProduct( TNameSpace *pSpc, double dKDImpl ):
   m_pNamSpc( pSpc )
	{
	  m_dKDImpl = dKDImpl;	 
	}
  ~TSimpleProduct();

  void Attach( TNameSpace *pSpc )
   {
     m_pNamSpc = pSpc;
   }

  void Create( LPCSTR lpPos, LPCSTR lpImpl, LPCSTR lpCmnt, TNameSpace *pSpc );

  TExpressionFram *FindVarInPos( TVariantVar *pV, TExpression*& pExpr );

  bool CalcPos();
  void CalcImpl();
  
  void ClearUsedFlag();

protected:
  vector<TExpressionFram*> m_vecPos; //посылки  
  TAutoMemoryRc<TExpression*> m_arcImpl; //заключение
  double m_dKDImpl;

  TVariantVar *m_pImplVar;

  void ClearVec();

  CString m_strProd;
  CString m_strComment;

  CString m_strImpl;
  CString m_strPos;

  TNameSpace* m_pNamSpc;
 };

class TFactWork
 {

friend class TWorkView;
friend class TResult;

public:
  TFactWork()
   {
     m_dKD = 1, m_pVar = NULL;
   }

  void Create( LPCSTR lpVar, LPCSTR lpVal, LPCSTR lpCmnt, TNameSpace *pSpc );

protected:
  CString m_strFac;
  CString m_strComment;
  double m_dKD;

  CString m_strVar;

  TVariantVar *m_pVar;
 };

struct TVarDescr
 {
   TVarDescr()
	{
	  m_pVar = NULL;
	}

   TVariantVar* m_pVar;
   CString m_strName;
 };

class TQWork  //запрос значений переменных в конкретном пространстве имён
 {

friend class TWorkView;
friend class TStaDlg;

public:
  TQWork()
   {
     m_pNamSpc = NULL;
   }
  ~TQWork();

  void Create( LPCSTR lpQ, LPCSTR lpCmnt, TNameSpace* pNamSpc );

protected:
  CString m_strQ;
  CString m_strComment;

  vector<TVarDescr*> m_vecVarQ;
  void ClearVec();

  TNameSpace* m_pNamSpc;
 };

#endif
