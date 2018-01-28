#include "mytoolb.hpp"

#include <afxcmn.h>

IMPLEMENT_DYNCREATE( TToolBar, CToolBar )


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP( TToolBar, CToolBar )
	//ON_WM_SHOWWINDOW()	
	//ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

TToolBar::TToolBar()
 {
 }


void TToolBar::AsnText( char* const* lpStr, int iNumb, UINT *pBtn, int iNumbBtn, bool bFlTxt )
 {      
   char* const* lpStr2 = lpStr;
   char *pMax = *lpStr;
   int iLen = 0;
   int iLen2;
   for( int i = 0; i < iNumb; i++, lpStr2++ )
     if( iLen <= (iLen2=strlen(*lpStr2)) )
	  {
        iLen = iLen2; pMax = *lpStr2;    
	  }

   CWindowDC dcDC( this );
   CFont *pFnt = CFont::FromHandle( (HFONT)WindowProc(WM_GETFONT, 0, 0) );
   CFont *pFntOld = (CFont*)dcDC.SelectObject( pFnt );
   CSize sz = dcDC.GetTextExtent( pMax, iLen );
   dcDC.SelectObject( pFntOld );
   

   BITMAP bm;
   ::GetObject( m_hbmImageWell, sizeof(bm), &bm );
   CSize szBtn(sz), szImg(bm.bmWidth / iNumb, bm.bmHeight);

   szBtn.cx = max( szBtn.cx, szImg.cx ) + 2;
   //szBtn.cx += 0.1 * float(szBtn.cx);   
   szBtn.cy += szImg.cy+4; szBtn.cy += 0.2 * float(szBtn.cy);

   if( bFlTxt == TRUE )
	{
	  lpStr2 = lpStr;   
	  for( i = 0; i < iNumbBtn; i++, pBtn++ )
	    if( *pBtn == ID_SEPARATOR ) continue;
	  else
	    SetButtonText( i, *lpStr2++ );      	   
	}   
   else
	{         
      SetSizes( szBtn, szImg );
	}    	        
   SetHeight( szBtn.cy + 0.2*float(szBtn.cy) );    
 }


void TToolBar::CompressBtns()
 {
   static int iExtraWidth = 0;
   if( !iExtraWidth )
	{
      CWindowDC dcDC( this );
	  TEXTMETRIC tm;

	  dcDC.GetTextMetrics( &tm );
	  iExtraWidth = tm.tmAveCharWidth;
	}

   int iEnd = GetToolBarCtrl().GetButtonCount();

   int iSepCount = 0;
   for( int iInd = 0; iInd < iEnd; iInd++ )
	 if( GetButtonStyle(iInd) & TBBS_SEPARATOR ) iSepCount++;	

   BITMAP bm;
   ::GetObject( m_hbmImageWell, sizeof(bm), &bm );
   CSize szImg( bm.bmWidth / (iEnd-iSepCount), bm.bmHeight );

   CWindowDC dcDC( this );
   CFont *pFnt = CFont::FromHandle( (HFONT)WindowProc(WM_GETFONT, 0, 0) );
   CFont *pFntOld = (CFont*)dcDC.SelectObject( pFnt );
   

   for( iInd = 0; iInd < iEnd; iInd++ )
	{
      if( GetButtonStyle(iInd) & TBBS_SEPARATOR ) continue;

      CRect r; GetItemRect( iInd, r );
	  //TBBUTTON btn;
	  //GetToolBarCtrl().GetButton( iInd, &btn );
	  //CWnd *pWnBtn = GetDlgItem( btn.idCommand );
	  CWnd *pWnBtn = GetWindow( GW_CHILD );

	  CString cs = GetButtonText( iInd );
	  CSize sz = dcDC.GetTextExtent( (LPCSTR)cs, cs.GetLength() );   

	  int iWith = max( sz.cx, szImg.cx ) + iExtraWidth;
	  int iDelta = r.Width() - iWith; 

	  pWnBtn->SetWindowPos( NULL, iDelta < 0 ? r.left+iDelta:r.left, r.top, iWith, r.Height(),
	    SWP_NOZORDER | SWP_NOACTIVATE );
	  //::CloseWindow( pWnBtn->m_hWnd );

	  break;
	}

   dcDC.SelectObject( pFntOld );
 }

/*CSize TToolBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
 {
   return CToolBar::CalcFixedLayout( bStretch, bHorz );
 }

CSize TToolBar::CalcDynamicLayout( int nLength, DWORD dwMode )
 {
   return CToolBar::CalcDynamicLayout( nLength, dwMode );
 }*/
