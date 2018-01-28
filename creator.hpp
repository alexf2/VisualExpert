#if !defined(_CREATOR_NEWOBJ_)
#define _CREATOR_NEWOBJ_

#include "stdafx.h"
#include "autorc.hpp"
#include "vessubj.hpp"

#include "..\\RUDAKOW\\SERVIS\\bwcc.hpp"

enum TC_Creation { TC_Rules, TC_Facts, TC_Quest, TC_Net, TC_Frames };


TSubject* CreateMgr( TC_Creation tcObj, LPCSTR lpFileNam, LPCSTR lpPath, LPCSTR lpName );
TSubject* ImportMgr( TC_Creation tcObj, LPCSTR lpFileNam, 
					 LPCSTR lpPath, LPCSTR lpName, LPSTR lpImportPath );
void Create_CREATOR_Null( LPCSTR );
bool CheckExist( LPCSTR strName );

#endif
