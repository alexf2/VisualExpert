// Net.cpp: implementation of the CNet class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "Net.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( CNet, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNet::CNet()
{
}

CNet::~CNet()
{
}

__int32 CNet::AddNode(CPoint p)
{
    CNode n( p );    
    return nodes.Add( n );
}

__int32 CNet::GetNodeByPos(CPoint p)
{
    for(__int32 i=0; i<nodes.GetSize(); i++) 
        if (nodes[i].PtInRect( p )) break;

    if (i == nodes.GetSize()) return -1;
    
    return i;
}

void CNet::DeleteNode(__int32 n)
{
    nodes.RemoveAt( n );

    // correct lines reference
    for (__int32 i=0; i<lines.GetSize(); i++) {
        if (lines[i].node[0] == n) lines[i].node[0] = -1; //maybe it would be better to delete node
        if (lines[i].node[1] == n) lines[i].node[1] = -1; 
        if (lines[i].node[0] > n) lines[i].node[0]--;
        if (lines[i].node[1] > n) lines[i].node[1]--;
    };
}

__int32 CNet::AddLine( CPoint p1, CPoint p2 )
{
    CNetLine line;
    lines.InsertAt( 0, line );

    MoveLine( 0, 0, p1 );
    MoveLine( 0, 1, p2 );

    return 0;
}

// returns true if line has actualy moved
bool CNet::MoveLine(__int32 iLine, __int32 lineend, CPoint newp)
{
    CPoint oldp = lines[iLine].p[lineend];
    __int32 i = GetNodeByPos( newp );

    // if inside node
    if ( i != -1 ) {
        lines[iLine].node[lineend] = i;
        lines[iLine].p[lineend] = nodes[i].GetDockingPos( lines[iLine].p[!lineend] );
    }
    else {
        lines[iLine].node[lineend] = -1;
        lines[iLine].p[lineend] = newp;
    }

    if (oldp == lines[iLine].p[lineend]) return false;
    else return true;

}

void CNet::MoveNode(__int32 iNode, CPoint delta)
{
    CRect newrect = nodes[iNode] + delta;    
   
    nodes[iNode].CopyRect( newrect );
   
     // move all connected to the node lines using GetDockingPos for this node
    for (__int32 i=0; i<lines.GetSize(); i++) 
        for (__int32 j=0; j<2; j++) {        
            if (lines[i].node[j] == iNode) {
                lines[i].p[j] = nodes[iNode].GetDockingPos( lines[i].p[!j] );
            
                // adjust the other end of line 
                if (lines[i].node[!j] != -1) 
                    lines[i].p[!j] = nodes[lines[i].node[!j]].GetDockingPos( lines[i].p[j] );
            };
        };
}

void CNet::CreateRgnFromLine(__int32 i, CRgn * rgn)
{
    POINT p[4];   
    CreatePointArrayFromLine( i, p );
    rgn->CreatePolygonRgn( p, 4, ALTERNATE );
}

void CNet::CreatePointArrayFromLine(__int32 i, POINT * p)
{
    const dx = 10;
    const dy = 5;

    POINT p1 = lines[i].p[0];
    POINT p2 = lines[i].p[1];
    
    double len = sqrt( (p2.x-p1.x)*(p2.x-p1.x) + 
                       (p2.y-p1.y)*(p2.y-p1.y) );
    if (len == 0) {
        p[0].x = p1.x - dx; p[0].y = p1.y + dy;
        p[1].x = p1.x + dx; p[1].y = p1.y + dy;
        p[2].x = p1.x + dx; p[2].y = p1.y - dy;
        p[3].x = p1.x - dx; p[3].y = p1.y - dy;
        return;
    };
    double sin_a = (p2.y - p1.y) / len;
    double cos_a = (p2.x - p1.x) / len;

    p[0].x = (long)len + dx; p[0].y = dy;
    p[1].x = (long)len + dx; p[1].y = -dy;
    p[2].x = -dx; p[2].y = -dy;
    p[3].x = -dx; p[3].y = dy;

    for (i=0; i<4; i++) {
        POINT np;
        np.x = p1.x + (long)(p[i].x*cos_a - p[i].y*sin_a);
        np.y = p1.y + (long)(p[i].x*sin_a + p[i].y*cos_a);
        p[i] = np;
    };
    
    /*
    if (lines[i].p[0].y < lines[i].p[1].y) {
        p[0].x = lines[i].p[1].x-dx; p[0].y = lines[i].p[1].y+dy;
        p[1].x = lines[i].p[1].x+dx; p[1].y = lines[i].p[1].y+dy;
        p[2].x = lines[i].p[0].x+dx; p[2].y = lines[i].p[0].y-dy;
        p[3].x = lines[i].p[0].x-dx; p[3].y = lines[i].p[0].y-dy;
    }
    else {
        p[0].x = lines[i].p[0].x-dx; p[0].y = lines[i].p[0].y+dy;
        p[1].x = lines[i].p[0].x+dx; p[1].y = lines[i].p[0].y+dy;
        p[2].x = lines[i].p[1].x+dx; p[2].y = lines[i].p[1].y-dy;
        p[3].x = lines[i].p[1].x-dx; p[3].y = lines[i].p[1].y-dy;
    };
    */
}

void CNet::CreatePointArrayFromNode(__int32 i, POINT * p)
{
    //not working ok
    CRect r = nodes[i];
   
    p[0].x = r.left; p[0].y = r.top;
    p[1].x = r.right; p[1].y = r.top;
    p[2].x = r.right; p[2].y = r.bottom;
    p[3].x = r.left; p[3].y = r.bottom;
}

__int32 CNet::GetLineByPoint(CPoint p, __int32* lineend)
{
    POINT points[4];
    for (__int32 i=0; i<lines.GetSize(); i++) {
     
        CreatePointArrayFromLine( i, points );

        CRgn r;

        r.CreatePolygonRgn( points, 4, WINDING );

        if (r.PtInRegion( p )) {        
            if (lineend) {
                POINT p1 = lines[i].p[0];
                POINT p2 = lines[i].p[1];
    
                double len0 = sqrt( (p.x-p1.x)*(p.x-p1.x) + 
                                    (p.y-p1.y)*(p.y-p1.y) );
                double len1 = sqrt( (p.x-p2.x)*(p.x-p2.x) + 
                                    (p.y-p2.y)*(p.y-p2.y) );

                if (len0<len1) *lineend = 0;
                else *lineend = 1;
            };

            return i;
        };

        r.DeleteObject();
    };
    return -1;
}

void CNet::DeleteLine(__int32 i)
{
    lines.RemoveAt( i );
}

void CNet::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CObject::Serialize( archive );
    nodes.Serialize( archive );
    lines.Serialize( archive );


    // now do the stuff for our specific class
    
    /*if( archive.IsStoring() )
        archive << lines << nodes;
    else
        archive >> lines >> nodes;
        */
}