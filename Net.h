// Net.h: interface for the CNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NET_H__60CA9002_972F_11D1_94CA_444553540000__INCLUDED_)
#define AFX_NET_H__60CA9002_972F_11D1_94CA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

#include "node.h"
#include "NetLine.h"

class CNet : public CObject  
{
public:

    DECLARE_SERIAL( CNet )

    CNet();
	
    void Serialize( CArchive& archive );

    void DeleteLine( __int32 i );
	__int32 GetLineByPoint( CPoint p, __int32* lineend = NULL);
	void CreatePointArrayFromNode( __int32 i, POINT* p);
	void CreatePointArrayFromLine( __int32 i, POINT* p );
	void CreateRgnFromLine( __int32 i, CRgn* rgn );
	void MoveNode( __int32 iNode, CPoint delta );
	bool MoveLine( __int32 iLine, __int32 lineend, CPoint newp );
	__int32 AddLine( CPoint p1 = CPoint( 0, 0 ), CPoint p2 = CPoint( 0, 0 ) );
	void DeleteNode( __int32 i );
	__int32 GetNodeByPos( CPoint p );
	__int32 AddNode( CPoint p );
	virtual ~CNet();
    CArray<CNode, CNode&> nodes;
    CArray<CNetLine, CNetLine&> lines;
};

#endif // !defined(AFX_NET_H__60CA9002_972F_11D1_94CA_444553540000__INCLUDED_)
