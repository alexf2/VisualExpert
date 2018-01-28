#include "vessubj.hpp"
#include "creator.hpp"
#include "expdlg.hpp"
#include "vs.hpp"
#include "mfrexp.hpp"
#include "credb.hpp"
#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

#include <afxdao.h>

TFac::TFac(): TSubject()
 {
 }

TFac::TFac( TProtectedObj *pPar, LPCSTR lpShortPath ):
  TSubject( pPar, lpShortPath )
 {
 }

TFac::~TFac()
 {
 }

TP_ClassTyp TFac::IsA() const
 {
   return TPC_TFac;
 }

LPCSTR TFac::MyName() const
 {
   return "База фактов";
 }

static TMyFieldInfo fldFacs[] =
 {
  { "Номер", dbLong, 0, dbUpdatableField },
  { "Переменная", dbMemo, 0, dbUpdatableField },
  { "= Значение", dbMemo, 0, dbUpdatableField },
  { "Комментарий", dbMemo, 0, dbUpdatableField },
  { "", -1, -1, -1 } 
 };


void TFac::CreateEmtyFile( LPCSTR lp )
 {
   CreateMyDB( lp, MyName(), 
	 this, fldFacs );	
 }

void TFac::ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath )
 {   
   ::CheckDBFormat( lpFromPath, MyName(), fldFacs, this );
   TSubject::ImportFile( lpToPath, lpFromPath ); //выполняет простое копирование файла в каталог ЭС
 }

bool TFac::RemoveObj( LPCSTR lpPathES, int iAction )
 {
   return TSubject::RemoveObj( lpPathES, iAction );
 }

void TFac::CheckDBFormat( LPCSTR lpPath )
 {
   ::CheckDBFormat( lpPath, MyName(), fldFacs, this );
 }


TQuery::TQuery(): TSubject()
 {
 }

TQuery::TQuery( TProtectedObj *pPar, LPCSTR lpShortPath ):
  TSubject( pPar, lpShortPath )
 {
 }

TQuery::~TQuery()
 {
 }

static TMyFieldInfo fldQ[] =
 {
  { "Номер", dbLong, 0, dbUpdatableField },
  { "Список переменных", dbMemo, 0, dbUpdatableField },
  { "Формулировка вопроса", dbMemo, 0, dbUpdatableField },  
  { "", -1, -1, -1 } 
 };

void TQuery::CreateEmtyFile( LPCSTR lp )
 {   
   CreateMyDB( lp, MyName(), 
	 this, fldQ );	
 }

void TQuery::ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath )
 {
   ::CheckDBFormat( lpFromPath, MyName(), fldQ, this );
   TSubject::ImportFile( lpToPath, lpFromPath ); //выполняет простое копирование файла в каталог ЭС
 }

bool TQuery::RemoveObj( LPCSTR lpPathES, int iAction )
 {
   return TSubject::RemoveObj( lpPathES, iAction );
 }

void TQuery::CheckDBFormat( LPCSTR lpPath )
 {
   ::CheckDBFormat( lpPath, MyName(), fldQ, this );
 }


TP_ClassTyp TQuery::IsA() const
 {
   return TPC_TQuery;
 }

LPCSTR TQuery::MyName() const
 {
   return "База вопросов";
 }


TPro::TPro(): TSubject()
 {
 }

TPro::TPro( TProtectedObj *pPar, LPCSTR lpShortPath ):
  TSubject( pPar, lpShortPath )
 {
 }

TPro::~TPro()
 {
 }

TP_ClassTyp TPro::IsA() const
 {
   return TPC_TPro;
 }

LPCSTR TPro::MyName() const
 {
   return "Продукционная база знаний";
 }

static TMyFieldInfo fldPro[] =
 {
  { "Номер", dbLong, 0, dbUpdatableField },
  { "Посылка (ЕСЛИ)", dbMemo, 0, dbUpdatableField },
  { "Заключение (ТО)", dbMemo, 0, dbUpdatableField },
  { "Комментарий", dbMemo, 0, dbUpdatableField },
  { "", -1, -1, -1 } 
 };

void TPro::CreateEmtyFile( LPCSTR lp )
 {
   CreateMyDB( lp, MyName(), 
	 this, fldPro );	
 }

void TPro::ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath )
 {
   ::CheckDBFormat( lpFromPath, MyName(), fldPro, this );
   TSubject::ImportFile( lpToPath, lpFromPath ); //выполняет простое копирование файла в каталог ЭС
 }

bool TPro::RemoveObj( LPCSTR lpPathES, int iAction )
 {
   return TSubject::RemoveObj( lpPathES, iAction );
 }

void TPro::CheckDBFormat( LPCSTR lpPath )
 {
   ::CheckDBFormat( lpPath, MyName(), fldPro, this );
 }



/*
  int m_iFromCompiled; //0 - self, 1 - from net, 2 - from frame

  TAutoMemoryRc<TQuery*> m_arcQ;
  TAutoMemoryRc<TFac*> m_arcF;
*/
CArchive& AFXAPI operator<<( CArchive& rA, TPro& rAc )
 {
   rA << *(TSubject*)&rAc;
   rA << rAc.m_iFromCompiled << int(rAc.m_arcQ()) << int(rAc.m_arcF());

   if( rAc.m_arcQ() ) rA << *rAc.m_arcQ();
   if( rAc.m_arcF() ) rA << *rAc.m_arcF();

   return rA;
 }

CArchive& AFXAPI operator>>( CArchive& rA, TPro& rAc )
 {
   int iQ, iF;
   rA >> ((TSubject&)rAc);
   rA >> rAc.m_iFromCompiled >> iQ >> iF;

   if( iQ )
	{
      rAc.m_arcQ = new TQuery();
	  rA >> *rAc.m_arcQ();
	}

   if( iF )
	{
      rAc.m_arcF = new TFac();
	  rA >> *rAc.m_arcF();
	}

   return rA;
 }






TExpertSystem::TExpertSystem(): TProtectedObj()
 {
   m_bModified = false;
   m_bWarnOnLoad = true;
 }

TExpertSystem::TExpertSystem( TProtectedObj *pPar, LPCSTR lpPath, LPCSTR lpName ):
  TProtectedObj( pPar ),
  m_csName( lpName ),
  m_csPath( lpPath ) 
 {
   m_bModified = false; 
   m_bWarnOnLoad = true;
 }

template<class T> void ClearList( T& rL )
 {
   T::iterator itSta( rL.begin() );
   T::iterator itEnd( rL.end() );
   for( ; itSta != itEnd; itSta++ )
	 delete *itSta;

   rL.clear();
 }

TExpertSystem::~TExpertSystem()
 {   
   ClearList( m_lstPro );
   ClearList( m_lstNet );
   ClearList( m_lstFrm );   
 }


void TExpertSystem::LoadHdrOnly( CArchive& rA )
 {
   rA >> *((TProtectedObj*)this);
   
   rA >> m_csName >> m_csPath;
 }

void TExpertSystem::AddVSubj( TSubject* pToAdd, TSubject* pParent )
 {
   switch( pToAdd->IsA() )
	{
	  case TPC_TNet:
	    m_lstNet.push_back( (TNet*)pToAdd );
		break;
	  case TPC_TFrm:
	    m_lstFrm.push_back( (TFrm*)pToAdd );
		break;
	  case TPC_TFac:
	    ((TPro*)pParent)->m_arcF = (TFac*)pToAdd;
        break;
	  case TPC_TQuery:
	   ((TPro*)pParent)->m_arcQ = (TQuery*)pToAdd;
        break;
	  case TPC_TPro:
	    m_lstPro.push_back( (TPro*)pToAdd );
		break;
	}   
 }

CArchive& AFXAPI operator<<( CArchive& rA, TExpertSystem& rAc )
 {
   rA << *(TProtectedObj*)&rAc;
   
   rA << rAc.m_csName << rAc.m_csPath;
      

   rA << rAc.m_lstPro.size();
   list<TPro*>::iterator itSta2( rAc.m_lstPro.begin() ); 
   list<TPro*>::iterator itEnd2( rAc.m_lstPro.end() ); 
   for( ;itSta2 != itEnd2; itSta2++ )
     rA << **itSta2;

   rA << rAc.m_lstNet.size();
   list<TNet*>::iterator itSta3( rAc.m_lstNet.begin() ); 
   list<TNet*>::iterator itEnd3( rAc.m_lstNet.end() ); 
   for( ;itSta3 != itEnd3; itSta3++ )
     rA << **itSta3;

   rA << rAc.m_lstFrm.size();
   list<TFrm*>::iterator itSta4( rAc.m_lstFrm.begin() ); 
   list<TFrm*>::iterator itEnd4( rAc.m_lstFrm.end() ); 
   for( ;itSta4 != itEnd4; itSta4++ )
     rA << **itSta4;  

   return rA;
 }

CArchive& AFXAPI operator>>( CArchive& rA, TExpertSystem& rAc )
 {
   rA >> ((TProtectedObj&)rAc);

   int iSz;
   rA >> rAc.m_csName >> rAc.m_csPath >> iSz;
  
   for( int i = 0; i < iSz; i++ )
	{
      TPro *pPro = new TPro();
	  rA >> *pPro;
	  rAc.m_lstPro.push_back( pPro );
	  rAc.CheckExist( pPro );
	  if( pPro->m_arcQ() ) rAc.CheckExist( pPro->m_arcQ() );
	  if( pPro->m_arcF() ) rAc.CheckExist( pPro->m_arcF() );
	}

   rA >> iSz;
   for( i = 0; i < iSz; i++ )
	{
      TNet *pNet = new TNet();
	  rA >> *pNet;
	  rAc.m_lstNet.push_back( pNet );
	  rAc.CheckExist( pNet );
	}

   rA >> iSz;
   for( i = 0; i < iSz; i++ )
	{
      TFrm *pFrm = new TFrm();
	  rA >> *pFrm;
	  rAc.m_lstFrm.push_back( pFrm );
	  rAc.CheckExist( pFrm );
	}

   return rA;
 }

void TExpertSystem::CheckExist( TSubject* pSubj )
 {   
   if( m_bWarnOnLoad == false ) return;

   CString sP = m_csPath + pSubj->GetPath();
   if( ::CheckExist(sP) == false )
	{
      TExpertApp *pApp = (TExpertApp*)AfxGetApp();
      TMainFrame *pFram = (TMainFrame*)pApp->m_pMainWnd;  

	  CString str;
	  str.Format( "Файл [%s] для объекта [%s \"%s\"] не найден. Изменить папку ЭС ?",
        (LPCSTR)sP, pSubj->MyName(), pSubj->GetName() );
      int iRes = MessageBox( pFram->m_hWnd, (LPCSTR)str, "Ошибка", MB_YESNOCANCEL|MB_ICONSTOP );
	  if( iRes == IDCANCEL )
	   {
	      m_bWarnOnLoad = false; return;
	   }
	  if( iRes == IDNO ) return;
	   	  

	  TFolderDlg dlgF( pFram, sP, true, *pFram->GetOptPtr() );
	  if( dlgF.DoModal() == IDOK ) m_csPath = sP, m_bModified = true;
	  else return;
	}
 }


bool TExpertSystem::RemoveObj( TSubject* pSubj, int iAction )
  //iAction - IDYES - удалить с файлами, IDNO - без файлов, IDCANCEL - отменить удаление
 {
   if( iAction == IDCANCEL ) return false;
   pSubj->RemoveObj( (LPCSTR)m_csPath, iAction );
   
   if( pSubj->IsA() == TPC_TNet ) 
	 m_lstNet.remove( (TNet*)pSubj ), delete pSubj;
   else if( pSubj->IsA() == TPC_TFrm ) 
	 m_lstFrm.remove( (TFrm*)pSubj ), delete pSubj;
   else if( pSubj->IsA() == TPC_TPro ) 
	{
	  if( ((TPro*)pSubj)->m_arcF() )
	    ((TPro*)pSubj)->m_arcF()->RemoveObj( (LPCSTR)m_csPath, iAction );
	  if( ((TPro*)pSubj)->m_arcQ() )
	    ((TPro*)pSubj)->m_arcQ()->RemoveObj( (LPCSTR)m_csPath, iAction );
	  m_lstPro.remove( (TPro*)pSubj ), delete pSubj;
	}
   else 
	{
      int isa = pSubj->IsA();

	  list<TPro*>::iterator itSta2( m_lstPro.begin() ); 
      list<TPro*>::iterator itEnd2( m_lstPro.end() ); 
      for( ;itSta2 != itEnd2; itSta2++ )
	    if( isa == TPC_TFac && (void*)((*itSta2)->m_arcF()) == (void*)pSubj )
		 {
            (*itSta2)->m_arcF.FreeRc();
			break;
		 }
		else if( isa == TPC_TQuery && (void*)((*itSta2)->m_arcQ()) == (void*)pSubj )
		 {
            (*itSta2)->m_arcQ.FreeRc();
			break;
		 }
	}   	

   return true;
 }

void TExpertSystem::GetFullObjPath( CString& rStr, TSubject* pSubj )
 {
   rStr = m_csPath + CString( pSubj->GetPath() );
 }
