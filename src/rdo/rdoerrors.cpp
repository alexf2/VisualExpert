// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "..\\stdafx.h"
#include <afxdisp.h>
#include "rdoerrors.h"

// Dispatch interfaces referenced by this interface
#include "rdoError.h"


/////////////////////////////////////////////////////////////////////////////
// CrdoErrors properties

/////////////////////////////////////////////////////////////////////////////
// CrdoErrors operations

long CrdoErrors::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CrdoError CrdoErrors::GetItem(const VARIANT& Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&Index);
	return CrdoError(pDispatch);
}

void CrdoErrors::Clear()
{
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
