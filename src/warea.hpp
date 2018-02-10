#if !defined(_WORK_AREA_)
#define _WORK_AREA_

#include "stdafx.h"
#include "autorc.hpp"
#include "var.hpp"
#include "product.hpp"
#include "vessubj.hpp"

#include <vector>
#include <list>
using namespace std;

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

//Реестр субкодов исключений TMyException !!!
#include "..\\RUDAKOW\\SERVIS\\subcode.hpp"

enum WAEXC { WA_Unknown = 0, WA_NeedQBase = 1, WA_NeedFBase = 2
           };
        
class TWAException: public TMyException
 {

  enum { TSC_Subcode = TREG_TWAException }; //! - у каждого субкласса должен быть свой

public:
  TWAException( BOOL bFlDel, int iExcCode, LPSTR lpName );    

  virtual int GetSubCode() const;
 };

class TTreeVarItem
 {
public:
   TTreeVarItem()
	{
	  m_pProd = NULL, m_bTyp = false;
	}
   TTreeVarItem( void* p, bool bTyp, TVariantVar *pVar )
	{
      m_pProd = (TSimpleProduct*)p;
	  m_bTyp = bTyp;
	  //m_pVar = pVar;
	  m_Var = *pVar;
	  m_Var.m_strName = pVar->m_strName;
	}

   TTreeVarItem( const TTreeVarItem& rT )
	{
	  TTreeVarItem::operator=( rT );
	}

   bool operator==( const TTreeVarItem &rIt )
   {
     return (void*)this == (void*)&rIt;
   }

   bool operator!=( TTreeVarItem &rIt )
   {
     return (void*)this != (void*)&rIt;
   }
   

   bool operator!=( const TTreeVarItem &rIt )
   {
     return (void*)this != (void*)&rIt;
   }

   bool operator>( TTreeVarItem& )
   {
       return true;
   }

   bool operator>( const TTreeVarItem& )
   {
       return true;
   }

   bool operator<( TTreeVarItem& )
   {
       return false;
   }

   
   TTreeVarItem& operator=( const TTreeVarItem& rT )
	{
      m_pProd = rT.m_pProd;
	  m_bTyp = rT.m_bTyp;
	  m_Var = rT.m_Var;
	  m_Var.m_strName = rT.m_Var.m_strName;

	  return *this;
	}

   union
	{
      TSimpleProduct *m_pProd;
	  TFactWork      *m_pFac;
	};
   bool m_bTyp; //true - m_pProd
   TVariantVar m_Var;
 };

inline bool operator!=( const TTreeVarItem &rIt, const TTreeVarItem &rIt2 )
 {
   return (void*)&rIt2 != (void*)&rIt;
 }

inline bool operator>( const TTreeVarItem&, const TTreeVarItem& )
{
  return false;
}


class TTreeVar
 {
public:
  TTreeVar( TVariantVar *pV )
   {
     m_pVar = pV;
   }

  TTreeVar( TTreeVar& );

  list<TTreeVarItem> m_lst;
  TVariantVar *m_pVar;
 };

class TWorkArea
 {

friend class TWorkView;
friend class TStaDlg;

public:
  TWorkArea( TPro* pProdDescr, TExpertSystem* pES )
   {
     m_pProdDescr = pProdDescr;
	 m_pES = pES;
   }

  ~TWorkArea();

  void Create();


protected:
  TPro* m_pProdDescr;
  TExpertSystem* m_pES;

  TAutoMemoryRc<TNameSpace*> m_arcNam;
  
  void ClearVarTree();

  vector<TSimpleProduct*> m_vecProd;
  vector<TFactWork*> m_vecWork;
  vector<TQWork*> m_vecQ;

  void ClearV();
 };

#endif
