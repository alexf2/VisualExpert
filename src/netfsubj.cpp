#include "netfsubj.hpp"
#include "creator.hpp"
#include "credb.hpp"
#include <afxdao.h>

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"
#include "net.h"

TNet::TNet(): TSubject()
 {
 }

TNet::TNet( TProtectedObj *pPar, LPCSTR lpShortPath ):
  TSubject( pPar, lpShortPath )
 {
 }

TNet::~TNet()
 {
 }

TP_ClassTyp TNet::IsA() const
 {
   return TPC_TNet;
 }

LPCSTR TNet::MyName() const
 {
   return "Семантическая сеть";
 }


void TNet::CreateEmtyFile( LPCSTR lp )
 {
   Create_CREATOR_Null( lp );
 }

void TNet::ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath )
 {
   TSubject::ImportFile( lpToPath, lpFromPath ); //выполняет простое копирование файла в каталог ЭС
 }

bool TNet::RemoveObj( LPCSTR lpPathES, int iAction )
 {
   return TSubject::RemoveObj( lpPathES, iAction );
 }

void TNet::CheckDBFormat( LPCSTR lpPath )
 {   
 }



TFrm::TFrm(): TSubject()
 {
 }

TFrm::TFrm( TProtectedObj *pPar, LPCSTR lpShortPath ):
  TSubject( pPar, lpShortPath )
 {
 }

TFrm::~TFrm()
 {
 }

TP_ClassTyp TFrm::IsA() const
 {
   return TPC_TFrm;
 }

LPCSTR TFrm::MyName() const
 {
   return "Фреймовая база знаний";
 }


void TFrm::CreateEmtyFile( LPCSTR lp )
 {
   Create_CREATOR_Null( lp );
 }

void TFrm::ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath )
 {
   TSubject::ImportFile( lpToPath, lpFromPath ); //выполняет простое копирование файла в каталог ЭС
 }

bool TFrm::RemoveObj( LPCSTR lpPathES, int iAction )
 {
   return TSubject::RemoveObj( lpPathES, iAction );
 }

void TFrm::CheckDBFormat( LPCSTR lpPath )
 {   
 }


static TMyFieldInfo fldPro[] =
 {
  { "Номер", dbLong, 0, dbUpdatableField },
  { "Посылка (ЕСЛИ)", dbMemo, 0, dbUpdatableField },
  { "Заключение (ТО)", dbMemo, 0, dbUpdatableField },
  { "Комментарий", dbMemo, 0, dbUpdatableField },
  { "", -1, -1, -1 } 
 };

void CompileNet( TNet* /*net*/, TPro* pro, LPCSTR lpFullPathPro, LPCSTR lpFullPathNet )
 {
    CNet* net = NULL;
    try {
        CFile theFile;    
        theFile.Open( lpFullPathNet, CFile::modeRead );
        CArchive archive( &theFile, CArchive::load );

        archive >> net;

        archive.Close();
        theFile.Close();
    }
    catch( CArchiveException* pE ) {
        net = new CNet;
		pE->Delete();
    }
    catch( CFileException* /*pE*/ ) {
        net = new CNet;
		//pE->Delete();
    }

   CDaoDatabase db;
   db.Open( lpFullPathPro );

   CDaoTableDef tb( &db );
   tb.Open( pro->MyName() );

   CDaoRecordset rset( &db );
   rset.Open( &tb );
   rset.Delete();

/*
old
1.1. если имя_узла = '@START' то имя_узла = имя_узла;
1.2. Для каждой дуги, входящей в узел, создается правило вида: 
     если   имя_дуги = имя_узла   то   имя_узла = имя_узла;
1.3. Для каждой дуги, исходящей из узла, создается правило вида:
     если имя_узла = имя_узла то имя_дуги = имя_узла_в_который_входит_дуга;
*/

   // 1.1
   //Для каждого узла сети сформировать правило вида:
   //если имя_узла = '@START' то имя_узла = имя_узла;
   for (__int32 i=0; i<net->nodes.GetSize(); i++ ) {
       rset.AddNew();
       COleVariant v1( _T( "имя_узла==\"@START\"" ), VT_BSTRT );
       COleVariant v2( _T( "имя_узла=\""+CString(net->nodes[i].name)+"\":1" ), VT_BSTRT );
       COleVariant v3( _T( " " ), VT_BSTRT );
       
       rset.SetFieldValue( 0, COleVariant((long)(i+1)) );
       rset.SetFieldValue( 1, v1 );
       rset.SetFieldValue( 2, v2 );
       rset.SetFieldValue( 3, v3 );

       rset.Update();
   };
   i++;
   for (__int32 j=0; j<net->lines.GetSize(); j++ ) {

       if (net->lines[j].node[0]!=-1 && 
           net->lines[j].node[1]!=-1) {
           
           //2.1
           //Для каждой дуги сети сформировать правило:
           //если имя_узла = имя_узла_из_которого_выходит_дуга 
           //то имя_дуги = имя_узла_в_который_входит_дуга;
           rset.AddNew();
           COleVariant v1( _T( "имя_узла==\""+CString(net->nodes[net->lines[j].node[0]].name)+"\"" ), VT_BSTRT );
           COleVariant v2( _T( CString(net->lines[j].name) + "=\""+CString(net->nodes[net->lines[j].node[1]].name)+"\":1" ), VT_BSTRT );
           COleVariant v3( _T( " " ), VT_BSTRT );
       
           rset.SetFieldValue( 0, COleVariant((long)(i+j*2)) );
           rset.SetFieldValue( 1, v1 );
           rset.SetFieldValue( 2, v2 );
           rset.SetFieldValue( 3, v3 );

           rset.Update();


           //2.2
           //Eсли дуга имеет тип [i], то сформировать правило вида:
           //если   имя_дуги = имя_узла_в_который_входит_дуга    
           //то   имя_узла = имя_узла_в_который_входит_дуга;
           if (net->lines[j].type == CNetLine::inh) {
               rset.AddNew();
               COleVariant v4( _T( CString(net->lines[j].name) + "==\""+CString(net->nodes[net->lines[j].node[1]].name)+"\"" ), VT_BSTRT );
               COleVariant v5( _T( "имя_узла=\""+CString(net->nodes[net->lines[j].node[0]].name)+"\":1" ), VT_BSTRT );
               COleVariant v6( _T( " " ), VT_BSTRT );
       
               rset.SetFieldValue( 0, COleVariant((long)(i+j*2+1)) );
               rset.SetFieldValue( 1, v4 );
               rset.SetFieldValue( 2, v5 );
               rset.SetFieldValue( 3, v6 );

               rset.Update();
           }

           //2.3
           //Если дуга имеет тип [t], то сформировать правило вида:
           //если имя_дуги = имя_узла_из_которого_выходит_дуга 
           //то имя_дуги = имя_узла_в_который_входит_дуга;
           if (net->lines[j].type == CNetLine::trans) {
               rset.AddNew();
               COleVariant v4( _T( CString(net->lines[j].name) + "==\""+CString(net->nodes[net->lines[j].node[0]].name)+"\"" ), VT_BSTRT );
               COleVariant v5( _T( CString(net->lines[j].name) + "=\""+CString(net->nodes[net->lines[j].node[1]].name)+"\":1" ), VT_BSTRT );
               COleVariant v6( _T( " " ), VT_BSTRT );
       
               rset.SetFieldValue( 0, COleVariant((long)(i+j*2+1)) );
               rset.SetFieldValue( 1, v4 );
               rset.SetFieldValue( 2, v5 );
               rset.SetFieldValue( 3, v6 );

               rset.Update();
           }       
       };

   };

   rset.Close();

   db.Close();   

   if (net) delete net;
 }
