#if !defined(AFX_RDOCOLUMNS_H__BAD271E5_9804_11D1_9504_444553540000__INCLUDED_)
#define AFX_RDOCOLUMNS_H__BAD271E5_9804_11D1_9504_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
#include <afxdisp.h>
class C_rdoColumn;

/////////////////////////////////////////////////////////////////////////////
// CrdoColumns wrapper class

class CrdoColumns : public COleDispatchDriver
{
public:
	CrdoColumns() {}		// Calls COleDispatchDriver default constructor
	CrdoColumns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CrdoColumns(const CrdoColumns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	void Refresh();
	C_rdoColumn GetItem(const VARIANT& Index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOCOLUMNS_H__BAD271E5_9804_11D1_9504_444553540000__INCLUDED_)
