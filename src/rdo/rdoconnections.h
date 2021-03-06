#if !defined(AFX_RDOCONNECTIONS_H__BAD271DA_9804_11D1_9504_444553540000__INCLUDED_)
#define AFX_RDOCONNECTIONS_H__BAD271DA_9804_11D1_9504_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class C_rdoConnection;

/////////////////////////////////////////////////////////////////////////////
// CrdoConnections wrapper class

class CrdoConnections : public COleDispatchDriver
{
public:
	CrdoConnections() {}		// Calls COleDispatchDriver default constructor
	CrdoConnections(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CrdoConnections(const CrdoConnections& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	void Remove(const VARIANT& ItemToRemove);
	void Add(LPDISPATCH prdoConnection);
	C_rdoConnection GetItem(const VARIANT& Index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOCONNECTIONS_H__BAD271DA_9804_11D1_9504_444553540000__INCLUDED_)
