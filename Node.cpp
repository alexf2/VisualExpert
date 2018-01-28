// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "Node.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( CNode, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNode::CNode()// : CRect( 0, 0, 90, 40 )
{
    *this =  CPoint( 0, 0 );
    //CNode(CPoint(0,0));
}

CNode::CNode( CPoint p )/* : CRect( p, CSize( 90, 40 ) )*/
{
    *this = CRect (p, CSize( 90, 40 ) );
}

CNode::CNode(CRect r) : CRect ( r )
{
    strcpy( name, "без имени");
}

CNode::~CNode()
{

}

void CNode::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CObject::Serialize( archive );

    // now do the stuff for our specific class
    if( archive.IsStoring() )
        archive << top << left << bottom << right;
    else
        archive >> top >> left >> bottom >> right;
};

// this function calculates nearest point ("docking pos") to connect line
CPoint CNode::GetDockingPos(CPoint p)
{
    if (bottom < p.y) return CPoint( left + (right - left)/2, bottom );
    if (top > p.y) return CPoint( left + (right - left)/2, top );

    if (p.x > right) return CPoint( right, bottom + (top - bottom)/2 );
    if (p.x < left) return CPoint( left, bottom + (top - bottom)/2 );
    
    return CenterPoint();
}
