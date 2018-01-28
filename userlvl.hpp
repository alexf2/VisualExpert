#if !defined(_USERLVL_)
#define _USERLVL_

#include "stdafx.h"
#include "autorc.hpp"
#include "coding.hpp"

#include <list>
using namespace std;

class TAcessLevel
 {

friend class TProtectedObj;
 
friend  CArchive& AFXAPI operator<<( CArchive& rA, TAcessLevel& );
friend  CArchive& AFXAPI operator>>( CArchive& rA, TAcessLevel& );

public:
  //more privileged->
  enum TUserClass { TU_Administrator=0, TU_Kognitolog=1, TU_Expert=2, TU_User=3 };
  enum TUserSubclass { TUS_Administrator=0, TUS_Named=1, TUS_Anonime=2 };
  //more->

  TAcessLevel(): m_uClass(TU_User), m_usSubClass(TUS_Anonime) 
   {
     m_bFlValideted = false;
   }
  TAcessLevel( TUserClass uClass, TUserSubclass usSubClass ):
    m_uClass(uClass), m_usSubClass(usSubClass) 
	 {
	   m_bFlValideted = false;
	 }
  TAcessLevel( TAcessLevel& rA )
   {
     m_uClass = rA.m_uClass;
	 m_usSubClass = rA.m_usSubClass;
	 m_bFlValideted = false;
   }

  virtual ~TAcessLevel();


  virtual bool KonfirmToThis( const TAcessLevel& rAL )
   {
     return rAL.m_uClass <= m_uClass && 
	        rAL.m_usSubClass <= m_usSubClass;
   }

  void SetRights( TAcessLevel::TUserClass uClass, TAcessLevel::TUserSubclass usSubClass )
   {
     m_uClass = uClass;
     m_usSubClass = usSubClass;
   }

  void GetRights( TAcessLevel::TUserClass& uClass, TAcessLevel::TUserSubclass& usSubClass ) const
   {
     uClass = m_uClass;
	 usSubClass = m_usSubClass;
   }

  bool IsValidated() const { return m_bFlValideted; }
  void SetValidate( bool bFl ) { m_bFlValideted = bFl; }


  LPCSTR GetStrClass() const;
  LPCSTR GetStrSubClass() const;

  TUserClass GetClass() const { return m_uClass; }
  TUserSubclass GetSubClass() const { return m_usSubClass; }

  bool Cmp( TAcessLevel& rA )
   {
     return m_uClass == rA.m_uClass && m_usSubClass == rA.m_usSubClass;
   }

protected:
  TUserClass m_uClass;
  TUserSubclass m_usSubClass;

  bool m_bFlValideted;
 };


inline CArchive& AFXAPI operator<<( CArchive& rA, TAcessLevel& rAc )
 {
   rA << (int)rAc.m_uClass << (int)rAc.m_usSubClass;
   return rA;
 }


enum { TISA_TUser = 0, TISA_TUserLocal, TISA_TUserConfig };

class TUser: public TAcessLevel
 {

friend  CArchive& AFXAPI operator<<( CArchive& rA, TUser& );
friend  CArchive& AFXAPI operator>>( CArchive& rA, TUser& );
 
public: 
 
 TUser(): TAcessLevel() {};
 TUser( LPCSTR lpFName, LPCSTR lpSName ): TAcessLevel(TAcessLevel::TU_User, TAcessLevel::TUS_Named),
    m_csFirstName(lpFName), m_csSecondName(lpSName) 
   {};

 TUser( const TUser& rUs )
  {
    operator=( rUs );
  }

 TUser& operator=( const TUser& rUs )
  {
    TAcessLevel::operator=( (TAcessLevel&)rUs ); 
     
	m_csPassword   = rUs.m_csPassword;
    m_csFirstName  = rUs.m_csFirstName;
    m_csSecondName = rUs.m_csSecondName;

    return *this;
  }

  virtual ~TUser();

  virtual CArchive& AFXAPI operator<<( CArchive& rA );
  virtual CArchive& AFXAPI operator>>( CArchive& rA );

  virtual int IsA() const;

  int operator==( const TUser& rUs )
   {
     return SameRights( rUs ) &&
	  rUs.m_csFirstName == m_csFirstName && 
	  rUs.m_csSecondName == m_csSecondName;
   }
  int SameRights( const TUser& rUs )
   {
     return rUs.m_uClass == m_uClass && rUs.m_usSubClass == m_usSubClass;
   }
  int SamePassword( const TUser& rUs )
   {
     return m_csPassword == rUs.m_csPassword;
   }

  LPCSTR FName() const { return m_csFirstName; }
  LPCSTR SName() const { return m_csSecondName; }
  LPCSTR Password() const { return m_csPassword; }

  void SetFName( LPCSTR lpN ) { m_csFirstName = lpN; }
  void SetSName( LPCSTR lpN ) { m_csSecondName = lpN; }
  void SetPassword( LPCSTR lpN ) { m_csPassword = lpN; }    

  bool Cmp( TUser& rU )
   {
     return TAcessLevel::Cmp(rU) &&
	        !m_csPassword.CompareNoCase( rU.m_csPassword ) && 
		    m_csFirstName == rU.m_csFirstName &&  	
			m_csSecondName == rU.m_csSecondName;
   }

protected:  
  CString m_csPassword;
  CString m_csFirstName;
  CString m_csSecondName;  
 };

inline  CArchive& AFXAPI operator<<( CArchive& rA, TUser& rU )
 {
   return rU.operator<<( rA );
 }
inline  CArchive& AFXAPI operator>>( CArchive& rA, TUser& rU )
 {
   return rU.operator>>( rA );
 }


class TProtectedObj: public TAcessLevel
 {   
   friend class TMainFrame;

   friend CArchive& AFXAPI operator<<( CArchive& rA, TProtectedObj& );
   friend CArchive& AFXAPI operator>>( CArchive& rA, TProtectedObj& );

public:
  TProtectedObj(): TAcessLevel()
   {
     m_bInchert = true;
	 m_pParent = NULL;
   };

  TProtectedObj( TProtectedObj *pPar, bool bInchert = true ): 
    TAcessLevel( *pPar ), m_pParent(pPar) 
   {
     m_bInchert = bInchert;
	 InitList( pPar );
   };

  TProtectedObj( TUserClass uClass, TUserSubclass usSubClass, TProtectedObj *pPar = NULL, bool bInchert = true ):
    TAcessLevel(uClass, usSubClass)
	 {
       m_bInchert = bInchert;
	   m_pParent = pPar;
	 }

 
  virtual ~TProtectedObj();

  void InitList( TProtectedObj *pPar );

  TProtectedObj& operator+( const TUser& rUsr )
   {
     m_lstUsers.push_back( &const_cast<TUser&>(rUsr) );
	 //m_usSubClass = TAcessLevel::TUS_Named;

	 return *this;
   }

  TProtectedObj& operator-( const TUser& rUsr );
   

  void ClearNamed();

  virtual bool KonfirmToThis( const TUser& rUser );
  bool KonfirmToObj( TProtectedObj& rUser );
  TUser*  FindUser( const TUser& rUser );

  TUser* GetFirstUser(); 
  void GetUserID( CString& );

  list<TUser*>& GetList() { return m_lstUsers; }

  LPCSTR Password()
   {
     return GetFirstUser()->Password();
   }

protected:
  list<TUser*>  m_lstUsers;

  TProtectedObj *m_pParent;
  bool m_bInchert;
 };


CArchive& AFXAPI operator<<( CArchive& rA, TProtectedObj& );
CArchive& AFXAPI operator>>( CArchive& rA, TProtectedObj& );


struct TNetData
 {  
 };
inline CArchive& AFXAPI operator<<( CArchive& rA, TNetData& )
 {
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TNetData& )
 {
   return rA;
 }

struct TFrameData
 {  
 };
inline CArchive& AFXAPI operator<<( CArchive& rA, TFrameData& )
 {
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TFrameData& )
 {
   return rA;
 }

struct TProdData
 {   
 };
inline CArchive& AFXAPI operator<<( CArchive& rA, TProdData& )
 {
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TProdData& )
 {
   return rA;
 }

struct TQueryData
 {  
 };
inline CArchive& AFXAPI operator<<( CArchive& rA, TQueryData& )
 {
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TQueryData& )
 {
   return rA;
 }

struct TGoData
 {
 };
inline CArchive& AFXAPI operator<<( CArchive& rA, TGoData& )
 {
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TGoData& )
 {
   return rA;
 }

class TErgoItem
 {
public:
   TErgoItem( LPCSTR lpNam ):
     m_strItemName( lpNam )
	{
	  m_clrFore = RGB(255,255,255), m_clrBk = RGB(0,0,128);
	  memset( &m_fnt, 0, sizeof(LOGFONT) );
	  m_pInchertFnt = NULL;
	};
   TErgoItem( const TErgoItem *pE )
	{
      operator=( pE );
	}

   TErgoItem& operator=( const TErgoItem *pE );

   virtual ~TErgoItem();

   void Create();
   void GDIDestroy();   	

   COLORREF m_clrFore, m_clrBk;
   LOGFONT  m_fnt;
   CString m_strItemName;
   
   TAutoMemoryRc<CFont*> arcFnt;
   TErgoItem *m_pInchertFnt;
////////////
   TErgoItem *m_pPar;//used by ergo dialog
 };

CArchive& AFXAPI operator<<( CArchive& rA, TErgoItem& rU );
CArchive& AFXAPI operator>>( CArchive& rA, TErgoItem& rU );
 
class TUserConfig: public TUser
 {

friend  CArchive& AFXAPI operator<<( CArchive& rA, TUserConfig& );
friend  CArchive& AFXAPI operator>>( CArchive& rA, TUserConfig& );

public:
  TUserConfig(): TUser() 
   {  
	 Init();
   }
  TUserConfig( const TUser& rUs ): TUser( rUs )
   {
 	 Init();
   }

  TUserConfig( const TUserConfig& rU )
   {
     this->operator=( rU );
   }

  TUserConfig& operator=( const TUserConfig& rU );
  

  virtual ~TUserConfig();    

  virtual int IsA() const;

  virtual CArchive& AFXAPI operator<<( CArchive& rA );
  virtual CArchive& AFXAPI operator>>( CArchive& rA );


  bool CheckCRC()
   {
     return  CalcCRC() == m_iCRC;
   }

  void DestroyGDI();

  CFont* GetItemFnt( TErgoItem* pIt )
   {
     return pIt->m_pInchertFnt ? pIt->m_pInchertFnt->arcFnt():pIt->arcFnt();
   }

  TAutoMemoryRc<TErgoItem*> m_ergo_LSPROD_DigConst,
    m_ergo_LSPROD_StrConst, m_ergo_LSPROD_KeyWord,
	m_ergo_LSPROD_Var, m_ergo_LSPROD_Func, m_ergo_LSPROD_Oper,
	m_ergo_LSPROD_Other;

  TAutoMemoryRc<TErgoItem*> m_ergo_LPVAR_Normal,
    m_ergo_LPVAR_NotNeed, m_ergo_LPVAR_NotInit, m_ergo_LPVAR_Canceled,
	m_ergo_LPVAR_Cursor;

  TAutoMemoryRc<TErgoItem*> m_ergo_LPFAC_Normal, m_ergo_LPFAC_Cursor;

  int m_iCalcPrec, m_iCalcShowDigitMode;

protected:
  int m_iCRC;
  UINT m_iProtectKey[2]; 
  
  int CalcCRC();
  void Init();    
 };

inline CArchive& AFXAPI operator<<( CArchive& rA, TUserConfig& rU )
 {
   rU.operator<<( rA );
   return rA;
 }
inline CArchive& AFXAPI operator>>( CArchive& rA, TUserConfig& rU )
 {
   rU.operator>>( rA );
   return rA;
 }


class TUserLocal: public TUserConfig
 {

friend  CArchive& AFXAPI operator<<( CArchive& rA, TUserLocal& );
friend  CArchive& AFXAPI operator>>( CArchive& rA, TUserLocal& );

friend class TMainFrame;

public:
  TUserLocal(): TUserConfig() 
   {  
	 InitLocal();
	 m_bModifiedData = false;

	 m_bShowSource = true;
	 m_bUseSpecialDesign = true;
   }
  TUserLocal( const TUser& rUs ): TUserConfig( rUs )
   {
 	 InitLocal();
	 m_bModifiedData = false;

	 m_bShowSource = true;
	 m_bUseSpecialDesign = true;
   }
  TUserLocal( const TUserLocal& rU )
   {
     this->operator=( rU );
   }

  TUserLocal& operator=( const TUserLocal& rU )
   {
     TUserConfig::operator=( (TUserConfig&)rU ); 
	 nt = rU.nt;
	 fr = rU.fr;
	 pr = rU.pr;
	 q  = rU.q;
	 g  = rU.g;

	 m_bModifiedData = true;

     return *this;
   }

  virtual ~TUserLocal();

  virtual int IsA() const;

  virtual CArchive& AFXAPI operator<<( CArchive& rA );
  virtual CArchive& AFXAPI operator>>( CArchive& rA );

  
  TNetData    nt;
  TFrameData  fr;
  TProdData   pr;
  TQueryData  q;
  TGoData     g;

  bool m_bShowSource;
  bool m_bUseSpecialDesign;

  void ModifyData() { m_bModifiedData = true; }
  
protected:
  bool m_bModifiedData;
      
  void InitLocal();    
 };

inline  CArchive& AFXAPI operator<<( CArchive& rA, TUserLocal& rU )
 {
   return rU.operator<<( rA );
 }
inline  CArchive& AFXAPI operator>>( CArchive& rA, TUserLocal& rU )
 {
   return rU.operator>>( rA );
 }


#endif
