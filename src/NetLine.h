// NetLine.h: interface for the CNetLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETLINE_H__00EFA721_9959_11D1_94CA_444553540000__INCLUDED_)
#define AFX_NETLINE_H__00EFA721_9959_11D1_94CA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



class CNetLine  
{
public:
	bool red;
	CString GetName();
    enum {
        prop, //property
        inh,  //inheritance
        trans //transistance
    } type;
	char name[255];
	__int32 node[2];
	POINT p[2];
	CNetLine();
	virtual ~CNetLine();

};

#endif // !defined(AFX_NETLINE_H__00EFA721_9959_11D1_94CA_444553540000__INCLUDED_)
