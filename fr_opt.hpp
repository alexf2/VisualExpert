#if !defined(_FR_OPT_)
#define _FR_OPT_

#include "stdafx.h"
#include "autorc.hpp"

#include "userlvl.hpp"



#define GLOBAL_CONFIG_FILENAME "ve.cfg"
#define GLOBAL_USERS_FILENAME  "users.cfg"

#define DEF_PASS_ADMINISTRATOR  "ADM"
#define DEF_PASS_KOGNITOLOG     "KOGNITOLOG"
#define DEF_PASS_EXPERT         "EXPERT"
#define DEF_PASS_USER           "USER"


class TExceptCfgNotFound {};
class TExceptCfgBadCRC {};

struct TOptData
 {
   //*********** Protect ***********//
   int m_iMaxRepass;
   bool m_bUseCoding;
   TCodingBAKMirrorFile::TCodingAlho codUserFile;
	                 
   char m_cKey[9];

   //*********** User global interface ***********//   
   bool m_bSaveMainFramePosOnExit;
   bool m_bRestoreMainFrame;

   //*********** Folder DLG ***********//   
   bool m_bMode; //list or icons

   //*********** Acc DLG ***********//   
   bool m_bExp1, m_bKogn1, m_bUs1;
   bool m_bExp2, m_bKogn2, m_bUs2;
 };

struct TScreenData
 {
   int m_iScreenX;
   int m_iScreenY;
   WINDOWPLACEMENT m_wpFrame;
 };

struct TMFOptions
 {

 friend CArchive& AFXAPI operator<<( CArchive& rA, TMFOptions& );
 friend CArchive& AFXAPI operator>>( CArchive& rA, TMFOptions& );

   TMFOptions()
	{
      d.m_iMaxRepass  = 3;
	  d.m_bUseCoding = true;
	  strcpy( d.m_cKey, "GLOBAL#1" );
	  d.codUserFile   = TCodingBAKMirrorFile::ALHO_XOR;	  

      d.m_bSaveMainFramePosOnExit = false;
      d.m_bRestoreMainFrame = false;

	  d.m_bMode = true;

	  d.m_bExp1 = d.m_bKogn1 = d.m_bUs1 = true;
	  d.m_bExp2 = d.m_bKogn2 = d.m_bUs2 = true;

	  memset( &s, 0, sizeof(TScreenData) );

	  dwCRC = 0;
	}

   TMFOptions( const  TMFOptions& rOpt )
	{
	  operator=( rOpt );
	}

   TMFOptions& operator=( const TMFOptions& rOpt )
	{
      d = rOpt.d;
	  s = rOpt.s;

	  return *this;
	}

   bool CheckCRC() { return dwCRC == CalcCRC(); }//need after loading

   //*********** Global options ***********//
   TOptData d;

   //*********** MainFrame data ***********//
   TScreenData s;

private:
  DWORD dwCRC;

  DWORD CalcCRC();
 };


inline CArchive& AFXAPI operator<<( CArchive& rA, TMFOptions& rOpt )
 {
   rA.Write( &rOpt.d, sizeof(TOptData) );
   rA << rOpt.CalcCRC();
   rA.Write( &rOpt.s, sizeof(TScreenData) );

   return rA;
 }


#endif
