// OwnerDrawBox.cpp : implementation file
//

#include "stdafx.h"
#include "vs.hpp"
#include "OwnerDrawBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBox

COwnerDrawBox::COwnerDrawBox()
{	 
	Align = left_;
	m_boldfont = NULL;
	m_sz = 0;
	
	LOGFONT lf;
	CFont* systemfont;
	m_boldfont = new CFont;
	
	
	CWnd* dw = GetDesktopWindow();

	CDC* dc = dw->GetDC();

	systemfont = (CFont*) dc->SelectStockObject( SYSTEM_FONT );
	
	systemfont->GetLogFont( &lf );

	lf.lfWeight = FW_BOLD;
	
	m_boldfont->CreateFontIndirect( &lf );

	//ReleaseDC( dc );	
}

COwnerDrawBox::~COwnerDrawBox()
{
  if( m_boldfont )
	delete m_boldfont;
  m_boldfont = NULL;
}


BEGIN_MESSAGE_MAP(COwnerDrawBox, CListBox)
	//{{AFX_MSG_MAP(COwnerDrawBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBox message handlers


void COwnerDrawBox::DrawItem( LPDRAWITEMSTRUCT lpDIS ) 
{
	if (ArrayOfAttr.GetSize() == 0) return;

	SAttributes attr = ArrayOfAttr[ lpDIS->itemID ];
	
	RECT rect = lpDIS->rcItem;
	CDC dc;	
	
	BOOL ret = dc.Attach( lpDIS->hDC );
	ASSERT( ret );

	dc.SetTextColor( attr.color );

	CString str = CString("  ") + attr.str + CString("  ");
	//CString str = CString("  ") + (LPSTR)GetItemData( lpDIS->itemID ) + CString("  ");
	

	//dc.SetBkColor( RGB(0,0,255) );
	dc.SetBkMode( TRANSPARENT );
	//dc.FillSolidRect( &rect, 
	 //(lpDIS->itemState & (ODS_SELECTED)) ? RGB(0,0,255):RGB(255,255,255) );

    bool bDr1 = lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT);
	bool bDr2 = (lpDIS->itemState & ODS_FOCUS) &&
		        (lpDIS->itemAction & (ODA_FOCUS | ODA_DRAWENTIRE));
	bool bDr3 = !(lpDIS->itemState & ODS_FOCUS) &&
		        (lpDIS->itemAction & ODA_FOCUS);

//-------------------------------
    if( bDr1 || bDr2 || bDr3 )
	 {
	   int is;
 	   dc.FillSolidRect( &lpDIS->rcItem, 
	     (is=(lpDIS->itemState & ODS_SELECTED)) ? RGB(0,0,128):RGB(0xFF,0xFF,0xFF) );

	   if( is && attr.color == RGB(0,0,0) )
		  dc.SetTextColor( RGB(255,255,255) );
 
	  
	   //dc.SetTextColor( (lpDIS->itemState & ODS_SELECTED) ? RGB(255,255,255):RGB(0,0,0) );	   

	   switch (attr.bold) {
		   case TRUE: {
			 CFont* oldfont = dc.SelectObject( m_boldfont );
			 switch (Align) {
			   case left_: dc.DrawText( str, &rect, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_NOPREFIX ); break;
			   case right_:	dc.DrawText( str, &rect, DT_VCENTER|DT_RIGHT|DT_SINGLELINE|DT_NOPREFIX ); break;
			 };
			 dc.SelectObject( oldfont );
			}; break;

		   case FALSE:{
			  switch (Align) {
			    case left_: dc.DrawText( str, &rect, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_NOPREFIX ); break;
			    case right_:	dc.DrawText( str, &rect, DT_VCENTER|DT_RIGHT|DT_SINGLELINE|DT_NOPREFIX ); break;
			  };
			}; break;
		};
	 }

	if( bDr2 )
	 {
		// item has been selected - hilite frame
       LOGBRUSH brush;
	   brush.lbStyle = BS_SOLID;
	   brush.lbColor = RGB( 0, 255, 0 );
	   brush.lbHatch = HS_BDIAGONAL;
	   CPen pen( PS_GEOMETRIC, 0, &brush );		   
	   CPen *pOld = dc.SelectObject( &pen );
	   CBrush *pBr = (CBrush *)dc.SelectStockObject( NULL_BRUSH );
       
	   dc.SetBkColor( RGB(0,0,0) );
       dc.Rectangle( &lpDIS->rcItem );

       dc.SelectObject( pBr ); 
  	   dc.SelectObject( pOld );
	 }

	if( bDr3 )
	 {
		// Item has been de-selected -- remove frame		        
	   CPen *pOld = (CPen*)dc.SelectStockObject( WHITE_PEN );
	   CBrush *pBr = (CBrush *)dc.SelectStockObject( NULL_BRUSH );

	   dc.Rectangle( &lpDIS->rcItem );

	   dc.SelectObject( pBr );
	   dc.SelectObject( pOld );
	 }
//-------------------------------	

	dc.Detach();	
}

int COwnerDrawBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	return 0;
}

void COwnerDrawBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	CSize s;

	if( ArrayOfAttr.GetSize() == 0 ) return;
	SAttributes attr = ArrayOfAttr[ lpMeasureItemStruct->itemID ];

	CString str = attr.str;

	CWnd* dw = GetDesktopWindow();
	CClientDC dc( dw );

	switch (attr.bold) {
	case TRUE: {
					CFont* oldfont = dc.SelectObject( m_boldfont );

					s = dc.GetTextExtent( str );

					dc.SelectObject( oldfont );
			   }; break;
	case FALSE:{
					s = dc.GetTextExtent( str );
			   }; break;	
	};

	lpMeasureItemStruct->itemWidth = s.cx;
    lpMeasureItemStruct->itemHeight = s.cy;
}

int COwnerDrawBox::AddString( LPCTSTR str, COLORREF color, BOOL bold )
 {    
	SAttributes attr;

	attr.color = color;
	attr.bold = bold;
	attr.str = str;	

	CClientDC dc( this );
	CFont *pFOld = dc.SelectObject( m_boldfont );
	CSize zs = dc.GetTextExtent( attr.str + "    " );
	dc.SelectObject( pFOld );
	int key = m_sz;
    m_sz = max( m_sz, zs.cx + 4 );
	
	if( key < m_sz )
	  SetHorizontalExtent( m_sz );
	
	ArrayOfAttr.Add( attr );
	
	return CListBox::AddString( "" );
 }
