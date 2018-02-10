#include "userlvl.hpp"


void TErgoItem::GDIDestroy()
 {
   if( arcFnt() )
	 arcFnt.FreeRc();      	
 }

void TErgoItem::Create()
 {
   if( !m_pInchertFnt )
	{
	  arcFnt = new CFont();
      arcFnt()->CreateFontIndirect( &m_fnt );   
	}
 }


CArchive& AFXAPI operator<<( CArchive& rA, TErgoItem& rU )
 {
   rA.Write( &rU.m_clrFore, sizeof(COLORREF) );
   rA.Write( &rU.m_clrBk, sizeof(COLORREF) );
   rA << (int)rU.m_pInchertFnt << rU.m_strItemName;
   if( !rU.m_pInchertFnt ) rA.Write( &rU.m_fnt, sizeof(LOGFONT) );      

   return rA;
 }
CArchive& AFXAPI operator>>( CArchive& rA, TErgoItem& rU )
 {
   rA.Read( &rU.m_clrFore, sizeof(COLORREF) );
   rA.Read( &rU.m_clrBk, sizeof(COLORREF) );
   int pTmp;
   rA >> pTmp >> rU.m_strItemName;
   if( !pTmp ) rA.Read( &rU.m_fnt, sizeof(LOGFONT) );   

   rU.Create();

   return rA;
 }


void TUserConfig::DestroyGDI()
 {
   if( m_ergo_LSPROD_DigConst() ) m_ergo_LSPROD_DigConst()->GDIDestroy();
   if(  m_ergo_LSPROD_StrConst() )  m_ergo_LSPROD_StrConst()->GDIDestroy();
   if(  m_ergo_LSPROD_KeyWord() )  m_ergo_LSPROD_KeyWord()->GDIDestroy();
   if(  m_ergo_LSPROD_Var() )  m_ergo_LSPROD_Var()->GDIDestroy();
   if(  m_ergo_LSPROD_Func() )  m_ergo_LSPROD_Func()->GDIDestroy();
   if(  m_ergo_LSPROD_Oper() )  m_ergo_LSPROD_Oper()->GDIDestroy();
   if(  m_ergo_LSPROD_Other() )  m_ergo_LSPROD_Other()->GDIDestroy();

   if(  m_ergo_LPVAR_Normal() )  m_ergo_LPVAR_Normal()->GDIDestroy();
   if(  m_ergo_LPVAR_NotNeed() )  m_ergo_LPVAR_NotNeed()->GDIDestroy();
   if(  m_ergo_LPVAR_NotInit() )  m_ergo_LPVAR_NotInit()->GDIDestroy();
   if(  m_ergo_LPVAR_Canceled() )  m_ergo_LPVAR_Canceled()->GDIDestroy();
   if(  m_ergo_LPVAR_Cursor() )  m_ergo_LPVAR_Cursor()->GDIDestroy();

   if( m_ergo_LPFAC_Normal() ) m_ergo_LPFAC_Normal()->GDIDestroy();
   if( m_ergo_LPFAC_Cursor() ) m_ergo_LPFAC_Cursor()->GDIDestroy();
 }


TAcessLevel::~TAcessLevel()
 {
 }

LPCSTR TAcessLevel::GetStrClass() const
 {
   switch(m_uClass)
	{
	  case TU_Administrator:
	    return "Администратор";
      case TU_Kognitolog:
	    return "Когнитолог";
      case TU_Expert:
	    return "Эксперт";
      case TU_User:
	    return "Пользователь(режим консультации)";
	  default:
	    return "Unknown";
	};
 }

LPCSTR TAcessLevel::GetStrSubClass() const
 {
   switch(m_usSubClass)
	{
	  case TUS_Administrator:
	    return "Администратор";
      case TUS_Named:
	    return "Иминованный";
      case TUS_Anonime:
	    return "Анонимный";      
	  default:
	    return "Unknown";
	};
 }

CArchive& AFXAPI operator>>( CArchive& rA, TAcessLevel& rAc )
 {
   int i1, i2;
   rA >> i1 >> i2;
   rAc.m_uClass = TAcessLevel::TUserClass(i1);
   rAc.m_usSubClass = TAcessLevel::TUserSubclass(i2);
   return rA;
 }



TUser::~TUser()
 {
 }

int TUser::IsA() const
 {
   return TISA_TUser;
 }

int TUserConfig::IsA() const
 {
   return TISA_TUserConfig;
 }

int TUserLocal::IsA() const
 {
   return TISA_TUserLocal;
 }


CArchive& AFXAPI TUser::operator<<( CArchive& rA )
 {
   ::operator<<( rA, *(TAcessLevel*)this );
   rA << m_csPassword << m_csFirstName << m_csSecondName;
   return rA;
 }

CArchive& AFXAPI TUser::operator>>( CArchive& rA )
 {         
   ::operator>>( rA, *(TAcessLevel*)this );
   rA >> m_csPassword >> m_csFirstName >> m_csSecondName;
   return rA;
 }


TProtectedObj::~TProtectedObj()
 {
    ClearNamed();
 }

void TProtectedObj::ClearNamed()
 {
   list<TUser*>::iterator it( m_lstUsers.begin() );
   list<TUser*>::iterator itEnd( m_lstUsers.end() );

   for( ; it != itEnd; ++it )	
     if( (*it) )
	  {
        delete (*it);		
		*it = NULL;		
	  }	
   m_lstUsers.clear();
 }

void TProtectedObj::GetUserID( CString& strNam )
 {
   TUser *pUs = GetFirstUser();
   if( !pUs ) strNam = "";
   else
     strNam.Format( "%s%s%s", (LPCSTR)pUs->FName(),
	  !strlen(pUs->SName()) ? "":" ",
	  !strlen(pUs->SName()) ? "":(LPCSTR)pUs->SName() );
 }

TUser* TProtectedObj::FindUser( const TUser& rUser )
 {
   list<TUser*>::iterator it( m_lstUsers.begin() );
   list<TUser*>::iterator itEnd( m_lstUsers.end() );

   for( ; it != itEnd; ++it )	
	if( *(*it) == rUser ) return (*it);

   return NULL;
 }

bool TProtectedObj::KonfirmToThis( const TUser& rUser )
 {
   //if( TAcessLevel::KonfirmToThis(rUser) == true ) return true;
   if( rUser.m_uClass == TAcessLevel::TU_Administrator ) return true;   

   if( rUser.m_uClass < m_uClass || 
	   (rUser.m_uClass == m_uClass && m_usSubClass != TAcessLevel::TUS_Named) 
	 ) return true;

   //if( rUser.m_usSubClass != TAcessLevel::TUS_Named ) return false;   

   TUser* pUsr = FindUser( rUser );
   if( !pUsr && m_bInchert == true && m_pParent ) pUsr = m_pParent->FindUser( rUser );     

   return pUsr != NULL;
 }

bool TProtectedObj::KonfirmToObj( TProtectedObj& rObj )
 {
   if( rObj.m_uClass == TAcessLevel::TU_Administrator ) return true;

   if( rObj.m_uClass < m_uClass || 
	   (rObj.m_uClass == m_uClass && m_usSubClass != TAcessLevel::TUS_Named) 
	 ) return true;

   //if( rObj.m_usSubClass != TAcessLevel::TUS_Named ) return false;      

   TUser* pUsr = rObj.GetFirstUser() ? FindUser( *rObj.GetFirstUser() ):NULL;   

   if( !pUsr && m_bInchert == true && m_pParent ) 
	 pUsr = m_pParent->FindUser( *rObj.GetFirstUser() );           

   return pUsr != NULL;
 }

void TProtectedObj::InitList( TProtectedObj *pPar )
 {
   list<TUser*>::iterator it( pPar->m_lstUsers.begin() );
   list<TUser*>::iterator itEnd( pPar->m_lstUsers.end() );
   
   for( ; it != itEnd; ++it )	
	{
      TUser *pU = new TUser( **it );
	  m_lstUsers.push_back( pU );
	}
 }

TProtectedObj& TProtectedObj::operator-( const TUser& rUsr )
 {
   TUser *pUsr = FindUser( rUsr );

   if( pUsr ) 
	{
      m_lstUsers.remove( pUsr );
	  delete pUsr;
	}

   return *this;
 }

TUser* TProtectedObj::GetFirstUser()
 { 
   if( !m_lstUsers.size() ) return NULL;
   return *m_lstUsers.begin(); 
 }
/*
TProtectedObj *m_pParent;
  bool m_bInchert;
*/

/*
CArchive& AFXAPI operator<<( CArchive& rA, TProtectedObj& );
CArchive& AFXAPI operator>>( CArchive& rA, TProtectedObj& );
*/


CArchive& AFXAPI operator<<( CArchive& rA, TProtectedObj& rO )
 {   
   rA << *(TAcessLevel*)&rO;

   rA << (BOOL)rO.m_bInchert;

   list<TUser*>::iterator it( rO.m_lstUsers.begin() );
   list<TUser*>::iterator itEnd( rO.m_lstUsers.end() );
   rA << (DWORD)rO.m_lstUsers.size();
   for( ; it != itEnd; ++it )	
	 rA << (*it)->IsA() << *(*it);
//enum { TISA_TUser = 0, TISA_TUserLocal, TISA_TUserConfig };    	
   return rA;
 }

CArchive& AFXAPI operator>>( CArchive& rA, TProtectedObj& rO )
 {   
   rA >> *(TAcessLevel*)&rO;

   BOOL btmp;
   rA >> btmp;
   rO.m_bInchert = (bool)btmp;

   rO.ClearNamed();

   DWORD sz;
   rA >> sz;
   for( int i = 0; i < sz; ++i )
    {
      int iTyp;
	  rA >> iTyp;
      
	  TAutoMemoryRc<TUser*> arcUsr;
	  switch( iTyp )
	   {
	     case TISA_TUser:
		   arcUsr = new TUser();
		   break;

		 case TISA_TUserLocal:
		   arcUsr = new TUserLocal();
		   break;

		 case TISA_TUserConfig:
		   arcUsr = new TUserConfig();
		   break;
	   };	  

      rA >> *(arcUsr());	  
	  rO.m_lstUsers.push_back( arcUsr() );
	  arcUsr.ClearRc(); 
	}
   
   return rA;
 }

TUserConfig::~TUserConfig()
 {
 }

CArchive& AFXAPI TUserConfig::operator<<( CArchive& rA )
 {
   m_iCRC = CalcCRC();

   TUser::operator<<( rA );
   rA << m_iCRC;
   rA.Write( m_iProtectKey, sizeof(m_iProtectKey)/sizeof(UINT) );

   rA << m_iCalcPrec << m_iCalcShowDigitMode;

   if( m_ergo_LSPROD_DigConst() ) rA << *m_ergo_LSPROD_DigConst();
   if( m_ergo_LSPROD_StrConst() ) rA << *m_ergo_LSPROD_StrConst();
   if( m_ergo_LSPROD_KeyWord() ) rA << *m_ergo_LSPROD_KeyWord();
   if( m_ergo_LSPROD_Var() ) rA << *m_ergo_LSPROD_Var();
   if( m_ergo_LSPROD_Func() ) rA << *m_ergo_LSPROD_Func();
   if( m_ergo_LSPROD_Oper() ) rA << *m_ergo_LSPROD_Oper();
   if( m_ergo_LSPROD_Other() ) rA << *m_ergo_LSPROD_Other();

   if( m_ergo_LPVAR_Normal() ) rA << *m_ergo_LPVAR_Normal();
   if( m_ergo_LPVAR_NotNeed() ) rA << *m_ergo_LPVAR_NotNeed();
   if( m_ergo_LPVAR_NotInit() ) rA << *m_ergo_LPVAR_NotInit();
   if( m_ergo_LPVAR_Canceled() ) rA << *m_ergo_LPVAR_Canceled();
   if( m_ergo_LPVAR_Cursor() ) rA << *m_ergo_LPVAR_Cursor();

   if( m_ergo_LPFAC_Normal() ) rA << *m_ergo_LPFAC_Normal();
   if( m_ergo_LPFAC_Cursor() ) rA << *m_ergo_LPFAC_Cursor();   

   return rA;
 }

CArchive& AFXAPI TUserConfig::operator>>( CArchive& rA )
 {
   TUser::operator>>( rA );
   rA >> m_iCRC;
   rA.Read( m_iProtectKey, sizeof(m_iProtectKey)/sizeof(UINT) );   

   rA >> m_iCalcPrec >> m_iCalcShowDigitMode;

   if( m_ergo_LSPROD_DigConst() ) rA >> *m_ergo_LSPROD_DigConst();
   if( m_ergo_LSPROD_StrConst() ) rA >> *m_ergo_LSPROD_StrConst();
   if( m_ergo_LSPROD_KeyWord() ) rA >> *m_ergo_LSPROD_KeyWord();
   if( m_ergo_LSPROD_Var() ) rA >> *m_ergo_LSPROD_Var();
   if( m_ergo_LSPROD_Func() ) rA >> *m_ergo_LSPROD_Func();
   if( m_ergo_LSPROD_Oper() ) rA >> *m_ergo_LSPROD_Oper();
   if( m_ergo_LSPROD_Other() ) rA >> *m_ergo_LSPROD_Other();

   if( m_ergo_LPVAR_Normal() ) rA >> *m_ergo_LPVAR_Normal();
   if( m_ergo_LPVAR_NotNeed() ) rA >> *m_ergo_LPVAR_NotNeed();
   if( m_ergo_LPVAR_NotInit() ) rA >> *m_ergo_LPVAR_NotInit();
   if( m_ergo_LPVAR_Canceled() ) rA >> *m_ergo_LPVAR_Canceled();
   if( m_ergo_LPVAR_Cursor() ) rA >> *m_ergo_LPVAR_Cursor();

   if( m_ergo_LPFAC_Normal() ) rA >> *m_ergo_LPFAC_Normal();
   if( m_ergo_LPFAC_Cursor() ) rA >> *m_ergo_LPFAC_Cursor();   

   return rA;
 }

TUserConfig& TUserConfig::operator=( const TUserConfig& rU )
 {
   TUser::operator=( (TUser&)rU ); 

   m_iCalcPrec = rU.m_iCalcPrec;
   m_iCalcShowDigitMode = rU.m_iCalcShowDigitMode;

   m_ergo_LSPROD_DigConst = new TErgoItem( rU.m_ergo_LSPROD_DigConst() );
   m_ergo_LSPROD_DigConst()->Create();
   m_ergo_LSPROD_StrConst = new TErgoItem( rU.m_ergo_LSPROD_StrConst() );
   m_ergo_LSPROD_StrConst()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_KeyWord = new TErgoItem( rU.m_ergo_LSPROD_KeyWord() );
   m_ergo_LSPROD_KeyWord()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_Var = new TErgoItem( rU.m_ergo_LSPROD_Var() );
   m_ergo_LSPROD_Var()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_Func = new TErgoItem( rU.m_ergo_LSPROD_Func() );
   m_ergo_LSPROD_Func()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_Oper = new TErgoItem( rU.m_ergo_LSPROD_Oper() );
   m_ergo_LSPROD_Oper()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_Other = new TErgoItem( rU.m_ergo_LSPROD_Other() );
   m_ergo_LSPROD_Other()->m_pInchertFnt = m_ergo_LSPROD_DigConst();


   m_ergo_LPVAR_Normal = new TErgoItem( rU.m_ergo_LPVAR_Normal() );
   m_ergo_LPVAR_Normal()->Create();

   m_ergo_LPVAR_NotNeed = new TErgoItem( rU.m_ergo_LPVAR_NotNeed() );
   m_ergo_LPVAR_NotNeed()->m_pInchertFnt = m_ergo_LPVAR_Normal();

   m_ergo_LPVAR_NotInit = new TErgoItem( rU.m_ergo_LPVAR_NotInit() );
   m_ergo_LPVAR_NotInit()->m_pInchertFnt = m_ergo_LPVAR_Normal();

   m_ergo_LPVAR_Canceled = new TErgoItem( rU.m_ergo_LPVAR_Canceled() );
   m_ergo_LPVAR_Canceled()->m_pInchertFnt = m_ergo_LPVAR_Normal();

   m_ergo_LPVAR_Cursor = new TErgoItem( rU.m_ergo_LPVAR_Cursor() );
   m_ergo_LPVAR_Cursor()->m_pInchertFnt = m_ergo_LPVAR_Normal();


   m_ergo_LPFAC_Normal = new TErgoItem( rU.m_ergo_LPFAC_Normal() );
   m_ergo_LPFAC_Normal()->Create();

   m_ergo_LPFAC_Cursor = new TErgoItem( rU.m_ergo_LPFAC_Cursor() );
   m_ergo_LPFAC_Cursor()->m_pInchertFnt = m_ergo_LPFAC_Normal();
   
   return *this;
 }

TErgoItem& TErgoItem::operator=( const TErgoItem *pE )
 {   
   m_clrFore = pE->m_clrFore;
   m_clrBk = pE->m_clrBk;
   m_fnt = pE->m_fnt;
   m_strItemName = pE->m_strItemName;  
   
   m_pInchertFnt = NULL;

   return *this;
 }


int TUserConfig::CalcCRC()
 {
   int iCRC = 0;
   for( int i = 0; i < sizeof(m_iProtectKey)/sizeof(UINT); ++i )
	 iCRC += m_iProtectKey[ i ];
   return iCRC;
 }

/*
  int m_iCRC;

  bool CheckCRC();
  void CalcRCR();

private:
  UINT m_iProtectKey[8]; 
*/

void DefFont( TErgoItem* pIt )
 {
   static LOGFONT lf = { -14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  /*FIXED_PITCH |*/ FF_DONTCARE, "MS Sans Serif" };
   static CFont fntDef;
   static bool bInit = false;

   if( bInit == false )
	{
      bInit = true;
	  BOOL bFl = fntDef.CreateFontIndirect( &lf );
	}

   memcpy( &(pIt->m_fnt), &lf, sizeof(LOGFONT) );   
   pIt->arcFnt = new CFont();
   pIt->arcFnt()->Attach( (HFONT)fntDef );
 }

TErgoItem::~TErgoItem()
 {
 }

void TUserConfig::Init()
 {
   memset( m_iProtectKey, 0, sizeof(m_iProtectKey) );
   m_iCRC = 0;

   m_iCalcPrec = 8;
   m_iCalcShowDigitMode = 0;

   srand( 1 );
   for( int i = 0; i < sizeof(m_iProtectKey)/sizeof(UINT); ++i )
     m_iProtectKey[ i ] = rand();

   m_ergo_LSPROD_DigConst = new TErgoItem( "Числовые константы" );
   m_ergo_LSPROD_DigConst()->m_clrFore = RGB(0,255,0);   
   DefFont( m_ergo_LSPROD_DigConst() );
   //m_ergo_LSPROD_DigConst()->Create();   

   m_ergo_LSPROD_StrConst = new TErgoItem( "Строковые константы" );
   m_ergo_LSPROD_StrConst()->m_clrFore = RGB(0,255,0);
   m_ergo_LSPROD_StrConst()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_KeyWord  = new TErgoItem( "Ключевые слова" );
   m_ergo_LSPROD_KeyWord()->m_pInchertFnt = m_ergo_LSPROD_DigConst();
   m_ergo_LSPROD_Var      = new TErgoItem( "Имена переменных" );
   m_ergo_LSPROD_Var()->m_clrFore = RGB(255,255,0);
   m_ergo_LSPROD_Var()->m_pInchertFnt = m_ergo_LSPROD_DigConst();

   m_ergo_LSPROD_Func     = new TErgoItem( "Имена функций" );
   m_ergo_LSPROD_Func()->m_pInchertFnt = m_ergo_LSPROD_DigConst();
   m_ergo_LSPROD_Oper     = new TErgoItem( "Операторы" );
   m_ergo_LSPROD_Oper()->m_pInchertFnt = m_ergo_LSPROD_DigConst();
   m_ergo_LSPROD_Oper()->m_clrFore = RGB(255,255,0);

   m_ergo_LSPROD_Other    = new TErgoItem( "Прочие символы" );   
   m_ergo_LSPROD_Other()->m_pInchertFnt = m_ergo_LSPROD_DigConst();
   m_ergo_LSPROD_Other()->m_clrFore = RGB(255,255,0);


   m_ergo_LPVAR_Normal    = new TErgoItem( "Инициц. переменная" );
   DefFont( m_ergo_LPVAR_Normal() );
   //m_ergo_LPVAR_Normal()->Create();   
   m_ergo_LPVAR_NotNeed   = new TErgoItem( "Переменная не нужная для вывода" );
   m_ergo_LPVAR_NotNeed()->m_clrFore = RGB(0x80,0x80,0x80);
   m_ergo_LPVAR_NotNeed()->m_pInchertFnt = m_ergo_LPVAR_Normal();

   m_ergo_LPVAR_NotInit   = new TErgoItem( "Неиниц. переменная" );
   m_ergo_LPVAR_NotInit()->m_clrFore = RGB(255,255,0);
   m_ergo_LPVAR_NotInit()->m_pInchertFnt = m_ergo_LPVAR_Normal();

   m_ergo_LPVAR_Canceled  = new TErgoItem( "Отменённые переменные" );
   m_ergo_LPVAR_Canceled()->m_clrFore = RGB(192, 192, 192);
   m_ergo_LPVAR_Canceled()->m_pInchertFnt = m_ergo_LPVAR_Normal();

   m_ergo_LPVAR_Cursor    = new TErgoItem( "Курсор" );
   m_ergo_LPVAR_Cursor()->m_pInchertFnt = m_ergo_LPVAR_Normal();
   m_ergo_LPVAR_Cursor()->m_clrBk = RGB(0,0,255);   
   m_ergo_LPVAR_Cursor()->m_clrFore = RGB( 255, 255, 0 );


   m_ergo_LPFAC_Normal    = new TErgoItem( "Факт" );
   DefFont( m_ergo_LPFAC_Normal() );
   //m_ergo_LPFAC_Normal()->Create();
   m_ergo_LPFAC_Cursor    = new TErgoItem( "Курсор" );
   m_ergo_LPFAC_Cursor()->m_pInchertFnt = m_ergo_LPFAC_Normal();

   m_ergo_LPFAC_Cursor()->m_clrBk = RGB(0,0,255);   
   m_ergo_LPFAC_Cursor()->m_clrFore = RGB( 255, 255, 0 );
 }


TUserLocal::~TUserLocal()
 {
 }


CArchive& AFXAPI TUserLocal::operator<<( CArchive& rA )
 {
   TUserConfig::operator<<( rA );

   int i1 = m_bShowSource, i2 = m_bUseSpecialDesign;
   rA << i1 << i2 << nt << fr << pr << q << g;
   
   return rA;
 }

CArchive& AFXAPI TUserLocal::operator>>( CArchive& rA )
 {
   TUserConfig::operator>>( rA );

   int i1, i2;
   rA >> i1 >> i2 >> nt >> fr >> pr >> q >> g;
   m_bShowSource = i1, m_bUseSpecialDesign = i2;

   m_bModifiedData = false;
   return rA;
 }

void TUserLocal::InitLocal()
 {
 }
