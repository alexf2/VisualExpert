// NetLine.cpp: implementation of the CNetLine class.
//
//////////////////////////////////////////////////////////////////////

#include "vs.hpp"
#include "NetLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetLine::CNetLine()
{
    p[0] = CPoint( 0, 0 );
    p[1] = CPoint( 0, 0 );
    node[0] = -1;
    node[1] = -1;
    type = prop;
    strcpy( name, "без имени" );
    red = false;
}

CNetLine::~CNetLine()
{

}

CString CNetLine::GetName()
{
    CString s;
    switch (type) {
    case prop  : s = "[p] "; 
                 break; 
    case inh   : s = "[i] "; 
                 break;
    case trans : s = "[t] ";
                 break;
    };
    return s + name;
}
