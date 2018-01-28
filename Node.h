// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__60CA9001_972F_11D1_94CA_444553540000__INCLUDED_)
#define AFX_NODE_H__60CA9001_972F_11D1_94CA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"

class CNode : public CRect, CObject  
{
 DECLARE_SERIAL( CNode )

public:
	CPoint GetDockingPos( CPoint p );
	char name[255];
	CNode( CRect rect );
    CNode( CPoint p );
	
    CNode();
	
    void Serialize( CArchive& archive );

    virtual ~CNode();
    CNode( const CNode &n )  // copy ctor
    { 
        strcpy( name, n.name );
        top = n.top;
        bottom = n.bottom;
        left = n.left;
        right = n.right;
    }
    CNode& operator=( const CNode &n )  // assignment operator
    { 
        strcpy( name, n.name ); 
        top = n.top;
        bottom = n.bottom;
        left = n.left;
        right = n.right;    
        return *this; 
    }
};

#endif // !defined(AFX_NODE_H__60CA9001_972F_11D1_94CA_444553540000__INCLUDED_)
