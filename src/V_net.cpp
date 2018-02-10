#include "v_net.hpp"
#include "subj.hpp"
#include "dlgacess.hpp"
#include "docexp.hpp"
#include "mdiexp.hpp"
#include "globlmsg.hpp"

#include "viewexp.hpp"
#include "NodeNameDialog.h"
#include "QuestionDialog.h"
#include <afxcmn.h>
#include <afxpriv.h>

#include "vs.rh"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( TNetView, TBaseView )

BEGIN_MESSAGE_MAP( TNetView, TBaseView )	
  ON_WM_CREATE()  
  ON_WM_DESTROY()
  ON_WM_ERASEBKGND()

  ON_WM_HSCROLL()
  ON_WM_VSCROLL()

  ON_MESSAGE( WM_HELPHITTEST , OnCommandHelp )
  ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )

  ON_WM_LBUTTONDOWN()  
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONDOWN()  
  
  ON_WM_MOUSEMOVE()
  
  ON_UPDATE_COMMAND_UI( ICM_RUL_EXIT, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_RUL_OPT, OnUpdate_Always )  

  ON_UPDATE_COMMAND_UI( ICM_NET_SAVE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_ZOOMPL, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_ZOOMMI, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_NEWNODE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_NEWADGE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_ERASE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_MOVE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_NET_SELECT, OnUpdate_Always )  
  
  ON_COMMAND( ICM_RUL_EXIT,   OnRulExit )

  ON_COMMAND( ICM_RUL_OPT,   OnRulOpt )
  ON_COMMAND( ICM_NET_SAVE,   OnNetSave )
  ON_COMMAND( ICM_NET_ZOOMPL,   OnNetZoomPl )
  ON_COMMAND( ICM_NET_ZOOMMI,   OnNetZoomMi )
  ON_COMMAND( ICM_NET_NEWNODE,   OnNetNewNode )
  ON_COMMAND( ICM_NET_NEWADGE,   OnNetNewAdge )
  ON_COMMAND( ICM_NET_ERASE,   OnNetErase )
  ON_COMMAND( ICM_NET_MOVE,   OnNetMove )
  ON_COMMAND( ICM_NET_SELECT,   OnNetSelect )
  
  ON_COMMAND( ID_NEW, OnMenuNew )
  ON_COMMAND( ID_NEWLINE, OnMenuNewLine )
  ON_COMMAND( ID_DELETE, OnMenuDelete )
  ON_COMMAND( ID_NAME, OnMenuName )
  ON_COMMAND( ID_P, OnMenuProp )
  ON_COMMAND( ID_I, OnMenuInh )  
  ON_COMMAND( ID_T, OnMenuTrans )
  ON_COMMAND( ID_QUESTION, OnMenuQuestion )

END_MESSAGE_MAP()

LRESULT TNetView::OnCommandHelp( WPARAM wp, LPARAM lParam )
{
  return 12347;
}

LRESULT TNetView::OnCommandHelpF1( WPARAM wp, LPARAM lParam )
{
  return TRUE;
}



TNetView::TNetView(): TBaseView()
{     
    m_shiftX = m_shiftY = 0;
    m_dScale = 1;
    nodemoving = false;
    linedrawing = false;
    popupmenu = new CMenu();  
    popupmenu1 = new CMenu();
    net = NULL;

	DisableScroll();
}
 
TNetView::~TNetView()
 {   
    if (popupmenu) delete popupmenu;
    if (popupmenu1) delete popupmenu1;
    //if (net) delete net;
 }

BOOL TNetView::OnEraseBkgnd( CDC *pDC )
 {   
   //pDC->SetMapMode( MM_TEXT );
   //pDC->SetViewportOrg( 0, 0 );

   CRect r;
   GetClientRect( r );
   r = DRtoLR( r );
   pDC->FillSolidRect( r, RGB(255,255,255) );
   
   return TRUE;   
 }

void TNetView::SetupMapMode( CDC* pDC )
 {   
   CRect rect;
   double iCx, iCy;

   GetClientRect( rect );
   pDC->SetMapMode( MM_ISOTROPIC );

   pDC->SetWindowExt( (iCx = rect.right - rect.left + 1),
		              (iCy = rect.bottom - rect.top + 1) );

   pDC->SetViewportExt( iCx*m_dScale, -iCy*m_dScale );
   pDC->SetViewportOrg( iCx / 2.0 + m_shiftX, iCy / 2.0 + m_shiftY );
 }

void TNetView::OnLButtonDown( UINT nFlags, CPoint point )
 {
    point = DPtoLP( point );

    __int32 node=net->GetNodeByPos( point );

    switch (cursorstate) {

    case zoomin :{
    }; break;

    case zoomout:{
    }; break;

    case move   :{        
        if (node!=-1) {
            SetCapture();
            nodemoving = true;
            CaptureStartPoint = point;
            iNodeSelected = node;
            oldnodepos = NULL;
        }
        else {
            __int32 i = net->GetLineByPoint( point, &lineend );
            if (i!=-1) {
               CRgn rold;
               POINT p[4];

               net->CreatePointArrayFromLine( i, p );
               m_arcDC()->LPtoDP( p, 4 );
               rold.CreatePolygonRgn( p, 4, WINDING );

               CRect rectold = GetLineNameRect( i );
       
                net->MoveLine( i, lineend, point );                
       
               RedrawWindow( NULL, &rold );
               RedrawWindow( LRtoDR( rectold ) );
               RedrawLine( i );
            
                linedrawing = true;
                //lineend = _linened;
                iLineSelected = i;
                SetCapture();

            }
        }
    }; break;         

    case del    :{
        // simulate menu
        MenuMousePos = point;

        OnMenuDelete();
    }; break;

    case addnode:{
        __int32 i = net->AddNode( point );
        RedrawWindow( LRtoDR( net->nodes[i] ) );
    }; break;

    case addline:{
        MenuMousePos = point;

        OnMenuNewLine();
    }; break;

    };

    
    //CPoint temp( point );
   //m_arcDC()->DPtoLP( &temp, 1 );
   //m_lst.push_back( temp );
   //CRect r( temp.x - 10, temp.y + 10, temp.x + 10, temp.y - 10 );
   //InvalidateRect( r, TRUE );
   //InvalidateRect( NULL );
 }

void TNetView::OnLButtonUp( UINT, CPoint point )
{
   point = DPtoLP( point );


   if (linedrawing) {
       linedrawing = false;
       ReleaseCapture();
       return;
   };

   if (nodemoving) {
       /*CRgn rold;
       FillNodeRgn( iNodeSelected, &rold );
       

       CRgn rnew;
       FillNodeRgn( iNodeSelected, &rnew );
       */

       CRect rold;
       CreateRectFromNode( iNodeSelected, &rold );
       
       CPoint delta = point - CaptureStartPoint;       
       net->MoveNode( iNodeSelected, delta );

       CRect rnew;
       CreateRectFromNode( iNodeSelected, &rnew );

       // redraw old
       RedrawWindow( LRtoDR( rold ) );       
       //redraw new
       RedrawWindow( LRtoDR( rnew ) );

       //CaptureStartPoint = point;

       nodemoving = false;
       ReleaseCapture();

       if (oldnodepos) delete oldnodepos;
   };
}

void TNetView::OnRButtonDown( UINT nFlags, CPoint point )
{
    CPoint lpoint = DPtoLP( point );

    MenuMousePos = lpoint;

    __int32 i = net->GetNodeByPos( lpoint );
    __int32 j = net->GetLineByPoint( lpoint );
    if (i!=-1) {            
        popupmenu->EnableMenuItem( ID_NEW, MF_ENABLED );
        popupmenu->EnableMenuItem( ID_DELETE, MF_ENABLED );
        popupmenu->EnableMenuItem( ID_NAME, MF_ENABLED );
    }
    else {
        popupmenu->EnableMenuItem( ID_NEW, MF_ENABLED );
        if (j!=-1) popupmenu->EnableMenuItem( ID_DELETE, MF_ENABLED );
        else popupmenu->EnableMenuItem( ID_DELETE, MF_GRAYED );
        if (j!=-1) popupmenu->EnableMenuItem( ID_NAME, MF_ENABLED );
        else popupmenu->EnableMenuItem( ID_NAME, MF_GRAYED );
    };
    if (j!=-1) {
        popupmenu->EnableMenuItem( ID_P, MF_ENABLED );
        popupmenu->EnableMenuItem( ID_I, MF_ENABLED );
        popupmenu->EnableMenuItem( ID_T, MF_ENABLED );
        popupmenu->CheckMenuItem( ID_P, MF_UNCHECKED );
        popupmenu->CheckMenuItem( ID_I, MF_UNCHECKED );
        popupmenu->CheckMenuItem( ID_T, MF_UNCHECKED );
        switch (net->lines[j].type) {
        case CNetLine::prop  : popupmenu->CheckMenuItem( ID_P, MF_CHECKED );
            break;
        case CNetLine::inh   : popupmenu->CheckMenuItem( ID_I, MF_CHECKED );
            break;
        case CNetLine::trans :  popupmenu->CheckMenuItem( ID_T, MF_CHECKED );
            break;
        };
    }
    else {
        popupmenu->EnableMenuItem( ID_P, MF_GRAYED );
        popupmenu->EnableMenuItem( ID_I, MF_GRAYED );
        popupmenu->EnableMenuItem( ID_T, MF_GRAYED );
    };


    ClientToScreen( &point );
    popupmenu->TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this );
    //menu.DestroyMenu();
}

void TNetView::OnMouseMove( UINT nFlags, CPoint point )
 {
   TBaseView::OnMouseMove( nFlags, point );

   point = DPtoLP( point );

   if (nodemoving) {

       LPCRECT newnodepos = net->nodes[iNodeSelected] - CaptureStartPoint + point;

       newnodepos = LRtoDR( newnodepos );

       static CSize size( 2, 2 );

       m_arcDC()->DrawDragRect( newnodepos, size, oldnodepos, size );

       if (!oldnodepos) oldnodepos = new RECT;

       *oldnodepos = *newnodepos;
   };

   if (linedrawing) {
       
       CRgn rold;
       POINT p[4];

       net->CreatePointArrayFromLine( iLineSelected, p );
       m_arcDC()->LPtoDP( p, 4 );
       rold.CreatePolygonRgn( p, 4, WINDING );

       CRect rectold = GetLineNameRect( iLineSelected );
             
       // this check is needed to avoid flicker when cursor is moving throuh the node
       if (net->MoveLine( iLineSelected, lineend, point )) {
           // if line changed position
           RedrawWindow( NULL, &rold );
           RedrawWindow( LRtoDR( rectold ) );
           RedrawLine( iLineSelected );
       };
   };
/*   
#ifdef _DEBUG

   CString str;
   CPoint p = m_arcDC()->GetViewportOrg();
   str.Format( "x:%d y:%d      ", point.x, point.y );
   m_arcDC()->TextOut( -p.x, p.y, str );

#endif
*/
 }

void TNetView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   CRect r;
   int iDelta = 0;

   switch( nSBCode )
	{
	  case SB_LEFT:
	   m_shiftX -= 1;
       iDelta = -1;
	   break;

      case SB_ENDSCROLL:
	   break;

	  case SB_LINELEFT:
	   m_shiftX -= 10;
	   iDelta = -10;
	   break;

	  case SB_LINERIGHT:
	   m_shiftX += 10;
	   iDelta = 10;
	   break;

	  case SB_PAGELEFT:
	   GetClientRect( r );
	   m_shiftX -= r.right;
	   iDelta = -r.bottom;
	   break;

	  case SB_PAGERIGHT:
	   GetClientRect( r );
	   m_shiftX += r.right;
	   iDelta = r.bottom;
	   break;

      case SB_RIGHT:
	   m_shiftX += 1;
	   iDelta = 1;
	   break;

      case SB_THUMBPOSITION:
	   m_shiftX = nPos;
	   iDelta = -(GetScrollPos( SB_HORZ ) - nPos);
	   break;

      case SB_THUMBTRACK:
	   break;
	}
   InvalidateRect( NULL );
   //CView::OnHScroll( nSBCode, nPos, pScrollBar );
   SetScrollPos( SB_HORZ, GetScrollPos(SB_HORZ) - iDelta, TRUE );
 }

void TNetView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   CRect r;
   int iDelta = 0;

   switch( nSBCode )
	{
	case SB_LEFT:
	   iDelta = -1;
	   m_shiftY -= 1;	   
	   break;

      case SB_ENDSCROLL:
	   break;

	  case SB_LINELEFT:
	   m_shiftY -= 10;
	   iDelta = -10;
	   break;

	  case SB_LINERIGHT:
	   m_shiftY += 10;
	   iDelta = 10;
	   break;

	  case SB_PAGELEFT:
	   GetClientRect( r );
	   m_shiftY -= r.bottom;
	   iDelta = -r.bottom;
	   break;

	  case SB_PAGERIGHT:
	   GetClientRect( r );
	   m_shiftY += r.bottom;
       iDelta = r.bottom;
	   break;

      case SB_RIGHT:
	   m_shiftY += 1;
	   iDelta = 1;
	   break;

      case SB_THUMBPOSITION:
	   m_shiftY = nPos;
	   iDelta = -(GetScrollPos( SB_VERT ) - nPos);
	   break;

      case SB_THUMBTRACK:
	   break;
	}
   InvalidateRect( NULL );
   SetScrollPos( SB_VERT, GetScrollPos(SB_VERT) - iDelta, TRUE );
   //CView::OnVScroll( nSBCode, nPos, pScrollBar );
 }


TO_ObjTypes TNetView::IsObj() const
 {
   return TO_Net;
 }

LPCSTR TNetView::GetShortFileName()
 {
   if( !m_pBindSubj ) return "Пусто";
   //return ((TSubject*)m_pBindSubj)->GetPath();
   return "Семантическая сеть";
 }
LPCSTR TNetView::GetName()
 {
   if( !m_pBindSubj ) return "Пусто";
   return ((TSubject*)m_pBindSubj)->GetName();
 }


void TNetView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   TBaseView::OnUpdate( pSender, lHint, pHint );
 }

#define GRAD_TO_RAD(grad) (double(grad) / 180.0 * 3.14)
#define RAD_TO_GRAD(rad) (double(rad) * 180.0 / 3.14)


static void DrawLabetLink( CDC *pDC, POINT& rPt1, POINT& rPt2, LPCSTR lpTxt )
 {
   LOGFONT lf = { -12, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  VARIABLE_PITCH | FF_DONTCARE, "Arial Cyr"
	};

   /*char cBuf[512];
   memset( cBuf, 0, 512 );
   strncpy( cBuf, lpTxt, 511 );*/
   
   //ostrstream str( cBuf, 50 );
   //str << float(pL->dWeight);

   CString str = lpTxt;
   
   float fAngle;
   int sec;
   float dCos, dSin, dTan;

   float dX = rPt2.x - rPt1.x;
   float dY = rPt2.y - rPt1.y;
   float d = sqrt( dX*dX + dY*dY );
   dCos = dX / d; dSin = dY / d; dTan = dY / dX;

   if( fabs(rPt1.x - rPt2.x) < 0.01 )
     fAngle = 900;
   else if( fabs(rPt1.y - rPt2.x) < 0.01 )	
	 fAngle = 0;	
   else
	{	  	  
      fAngle = float( RAD_TO_GRAD(atan(dTan)) * 10 ); 

	  if( dCos > 0 && dSin > 0 ) sec = 1; 	   
	  else if( dCos < 0 && dSin > 0 ) sec = 2;           	   
	  else if( dCos < 0 && dSin < 0 ) sec = 3;	           	   	   
	  else sec = 4;
	}                

   lf.lfOrientation = lf.lfEscapement = 3600 - fAngle;	   

   CFont fnt;
   fnt.CreateFontIndirect( &lf );
	
   CFont *pOldFont = pDC->SelectObject( &fnt );

   int iLen = str.GetLength();
   //str.freeze( false );
   CSize sz = pDC->GetTextExtent( str );
   //str.freeze( false );
   

   float x, y;      
   /*if( pL->GetType() == TGrLink::LNK_Line )
	{*/
      x = d/2.0; y = (sec == 2 || sec == 3) ? -(sz.cy + 5):sz.cy + 5;       
	//}
   /*else
	{
	  if( pL->GetType() == TGrLink::LNK_C1 )
	   { x = d/2.0; y = (!(sec == 2 || sec == 3)) ? -25:-(sz.cy + 25); }     
	  else
	   { x = d/2.0; y = (sec == 2 || sec == 3) ? 25:sz.cy + 25; }     
	}*/
   
   dY = -dY;   
   dCos = dX / d; dSin = dY / d; 

   float kx = x;   
   x = x*dCos + y*dSin; y = -kx*dSin + y*dCos;
   
   x += rPt1.x; y += rPt1.y;   
   

   /*if( (void*)(pL->pOwner) == (void*)(pL->pVert) )
	{
      x = pL->cpSpl.x; y = pL->cpSpl.y + sz.cy + 5;
	}*/

   UINT aln = pDC->SetTextAlign( TA_CENTER | TA_TOP );
   pDC->TextOut( x, y, str ); 
   //str.freeze( false );
   pDC->SetTextAlign( aln );

   pDC->SelectObject( pOldFont );
 }


void TNetView::OnDraw( CDC* pDC )
 {   
   SetupMapMode( pDC );

   __int32 i;

   // draw nodes
   CPen p2( PS_SOLID, 1, RGB( 0, 0, 180 ) );
   CPen* orig = pDC->SelectObject( &p2 );
   pDC->SetTextColor(RGB( 0, 0, 255 ));
   for (i=net->nodes.GetSize()-1; i>=0; i--) 
   {
       static CPoint p ( 6, 6 );
       CRect rect = net->nodes[i];
       pDC->RoundRect( rect, p );       
       pDC->DrawText( net->nodes[i].name, n( rect ), DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX );
   };

   // draw lines
   CPen p3( PS_SOLID, 1, RGB( 0, 180, 0 ) );
   CPen pred( PS_SOLID, 1, RGB( 180, 0, 0 ) );
   for (i=net->lines.GetSize()-1; i>=0; i--) 
   {
       if (net->lines[i].red) {
          pDC->SelectObject( &pred );
          pDC->SetTextColor(RGB( 180, 0, 0 ));
       }
       else {
          pDC->SelectObject( &p3 );
          pDC->SetTextColor(RGB( 0, 255, 0 ));
       };
       pDC->MoveTo( net->lines[i].p[0] );
       pDC->LineTo( net->lines[i].p[1] );

       //draw arrow
       const static double alfa = .3; // 0.3 PI
       const static len = 10;
       // end point
       POINT p = net->lines[i].p[1];
       pDC->MoveTo( net->lines[i].p[1] );
       // vector direction angle
       double beta = atan2( p.y - net->lines[i].p[0].y,
                            p.x - net->lines[i].p[0].x );
       pDC->LineTo( p.x - cos( beta + alfa ) * len,
                    p.y - sin( beta + alfa ) * len );
       pDC->MoveTo( p );
       pDC->LineTo( p.x - cos( beta - alfa ) * len,
                    p.y - sin( beta - alfa ) * len );

       CRect rect( net->lines[i].p[0], net->lines[i].p[1] );
       rect.NormalizeRect();

//!!!!!!!!!!!!!!!!!!!!!!
	   DrawLabetLink( pDC, net->lines[i].p[0], net->lines[i].p[1], net->lines[i].GetName() );
       //pDC->DrawText( net->lines[i].GetName(), n( rect ), DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP );
   };

  
   /*
#ifdef _DEBUG
   // draw coordinate system
   CPen p1( PS_SOLID, 1, RGB( 0, 0, 0 ) );
   pDC->SelectObject( &p1 );
   pDC->MoveTo( 0, 0 );
   pDC->LineTo( 1000, 0 );//+x
   pDC->MoveTo( 0, 0 );
   pDC->LineTo( -1000, 0 );//-x
   pDC->MoveTo( 0, 0 );
   pDC->LineTo( 0, 1000 );//+y
   pDC->MoveTo( 0, 0 );
   pDC->LineTo( 0, -1000 );//-y
#endif
   */
   
   // restore original pen
   pDC->SelectObject( orig );
}

void TNetView::OnDestroy()
 {
   m_arcDC.FreeRc();

   TBaseView::OnDestroy();

   popupmenu->DestroyMenu( );
 }

BOOL TNetView::PreCreateWindow( CREATESTRUCT& cs )
{
   BOOL bRes = TBaseView::PreCreateWindow( cs );
   if( !bRes ) return bRes;

   static CString str;
   str = AfxRegisterWndClass(
	  CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 
	  LoadCursor(NULL, IDC_ARROW), 
	  (HBRUSH)(COLOR_WINDOW + 1) ); 	    

   cs.style |= WS_VSCROLL | WS_HSCROLL;
   cs.lpszClass = (LPCSTR)str;
   
   return TRUE;
}

int TNetView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if( TBaseView::OnCreate(lpCreateStruct) == -1 )
        return -1;
          
    m_arcDC = new CClientDC( this );
   
    //popupmenu->CreatePopupMenu( );
    //popupmenu->LoadMenu( IDR_TEST );

    return 0; 
}

void TNetView::OnInitialUpdate()
 {
   TBaseView::OnInitialUpdate();   

   CRect r; 
   GetDesktopWindow()->GetClientRect( r ); r.InflateRect( 2*r.Width(), 2*r.Height() );

   SetScrollRange( SB_VERT, -r.Height(), r.Height(), FALSE );
   SetScrollRange( SB_HORZ, -r.Width(), r.Width(), FALSE );
   SetScrollPos( SB_VERT, 0, FALSE );	
   SetScrollPos( SB_HORZ, 0, FALSE );	

    
    TExpertDoc* doc = GetDocument();
    TExpertSystem* es = doc->GetESystem();

    TSubject* subj = (TSubject*) m_pBindSubj;

    es->GetFullObjPath( filename, subj );

    
    try {
        CFile theFile;
        CFileException ex;
        if (!theFile.Open( filename, CFile::modeRead, &ex )) throw &ex;         
        CArchive archive( &theFile, CArchive::load );

        archive >> net;

        archive.Close();
        theFile.Close();
    }
    catch( CArchiveException* pE ) 
	 {
        pE->Delete();
        net = new CNet;
     }
    catch( CFileException* /*pE*/) 
	 {
	    //pE->Delete();
        net = new CNet;
     }
    

    popupmenu1->CreatePopupMenu();
    popupmenu1->AppendMenu( MF_STRING, ID_NEW, "Узел" );
    popupmenu1->AppendMenu( MF_STRING, ID_NEWLINE, "Дуга" );

    popupmenu->CreatePopupMenu();
    popupmenu->AppendMenu( MF_POPUP, (UINT)popupmenu1->m_hMenu, "Создать" );
    popupmenu->AppendMenu( MF_STRING, ID_DELETE, "Удалить" );
    popupmenu->AppendMenu( MF_SEPARATOR );    
    popupmenu->AppendMenu( MF_STRING, ID_NAME, "Переименовать" );
    popupmenu->AppendMenu( MF_SEPARATOR );    
    popupmenu->AppendMenu( MF_STRING, ID_P, "Свойство" );
    popupmenu->AppendMenu( MF_STRING, ID_I, "Наследование" );
    popupmenu->AppendMenu( MF_STRING, ID_T, "Транзит" );
    popupmenu->AppendMenu( MF_SEPARATOR );    
    popupmenu->AppendMenu( MF_STRING, ID_QUESTION, "Задать вопрос" );
}


TV_Views TNetView::IsA() const
 {
   return TV_NetsEdit;
 }

bool TNetView::CanClose()
 {
   return true;
 }
  

static TBtnTool btnHoriz[] =
 {
  { TB_Bmp, ICM_NET_SAVE, NULL, IDB_BMP_NET_SAVE, true, "Сохранить изменения в файле", BS_PUSHBUTTON|WS_GROUP },
  { TB_Bmp, ICM_NET_ZOOMPL, NULL, IDB_BMP_NET_ZOOMPL, false, "Увеличить изображение сети", BS_PUSHBUTTON/*BS_AUTORADIOBUTTON|BS_PUSHLIKE*/ },
  { TB_Bmp, ICM_NET_ZOOMMI, NULL, IDB_BMP_NET_ZOOMMI, true, "Уменьшение изображение сети", BS_PUSHBUTTON/*BS_AUTORADIOBUTTON|BS_PUSHLIKE*/ },

  { TB_Bmp, ICM_RUL_OPT, NULL, IDB_BMP_EXP_OPT, false, "Настройка редактора семантической сети", BS_PUSHBUTTON },    
  { TB_Bmp, ICM_RUL_EXIT, NULL, IDB_BMP_RUL_EXIT, true, "Выход из редактора семантической сети", BS_PUSHBUTTON },
  { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };

static TBtnTool btnVert[] =
 {
  { TB_Bmp, ICM_NET_NEWNODE, NULL, IDB_BMP_NET_NEWNODE, true, "Режим создания новых узлов сети", BS_AUTORADIOBUTTON|BS_PUSHLIKE|WS_GROUP },
  { TB_Bmp, ICM_NET_NEWADGE, NULL, IDB_BMP_NET_NEWADGE, true, "Режим создания новых дуг сети", BS_AUTORADIOBUTTON|BS_PUSHLIKE },
  { TB_Bmp, ICM_NET_ERASE, NULL, IDB_BMP_NET_ERASE, true, "Режим удаления объектов сети", BS_AUTORADIOBUTTON|BS_PUSHLIKE },

  { TB_Bmp, ICM_NET_SELECT, NULL, IDB_BMP_NET_SELECT, false, "Режим выделения объектов сети", BS_AUTORADIOBUTTON|BS_PUSHLIKE },    
  { TB_Bmp, ICM_NET_MOVE, NULL, IDB_BMP_NET_MOVE, true, "Режим перемещения объектов сети", BS_AUTORADIOBUTTON|BS_PUSHLIKE },
  { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };


void TNetView::SetupHorizBar( CMyDialogBar& rB )
 {     
   rB.SetCurVievClass( TV_NetsEdit );

   ClearBtnsLists(); //only here ! - first calls horizontal setup
   CreateBtns( rB, btnHoriz, false, m_lstBmp, m_lstBtn );
 }

void TNetView::SetupVerBar( CMyDialogBar& rB )
 {
   rB.SetCurVievClass( TV_NetsEdit );

   CreateBtns( rB, btnVert, false, m_lstBmp, m_lstBtn );

   if( rB.GetParent() && rB.GetParent()->IsWindowVisible() &&
       !rB.IsWindowVisible()
	 )
     rB.ShowWindow( SW_SHOW );   
 }


void TNetView::OnRulExit()
 {
   CWnd *pW = GetParent();
   if( !pW ) return;

   pW->PostMessage( globl_iMsgSwitchViews, TSWT_Explorer, 
	 (LPARAM)((TExpertDoc*)GetDocument())->GetESystem() );
 }

void TNetView::OnRulOpt()
 {
 }



void TNetView::OnNetSave()
{
    try {
        CFile theFile;
        CFileException ex;
        if (!theFile.Open( filename, CFile::modeWrite|CFile::modeCreate, &ex )) throw &ex;
        CArchive archive( &theFile, CArchive::store );

        archive << net;

        archive.Close();
        theFile.Close();
    }
    catch( CArchiveException* pE ) {
        MessageBox( "Ошибка при сохранении сети" );
		pE->Delete();
    }
    catch( CFileException* /*pE*/ ) {
        MessageBox( "Ошибка при сохранении сети" );
		//pE->Delete();
    }
}



void TNetView::OnNetZoomPl()
{
    //cursorstate = zoomin;
    m_dScale *= 2;
    InvalidateRect( NULL );
}
void TNetView::OnNetZoomMi()
{
    //cursorstate = zoomout;
    m_dScale /= 2;
    InvalidateRect( NULL );
}
void TNetView::OnNetNewNode() {
    cursorstate = addnode;
}
void TNetView::OnNetNewAdge()
 {
    cursorstate = addline;
 }
void TNetView::OnNetErase()
 {
    cursorstate = del;
 }
void TNetView::OnNetMove()
 {
    cursorstate = move;
 }
void TNetView::OnNetSelect()
 {
 }
void TNetView::OnMenuNew() 
{
    __int32 i = net->AddNode( MenuMousePos );
    RedrawWindow( LRtoDR( net->nodes[i] ) );    
}
void TNetView::OnMenuNewLine() 
{
    __int32 i = net->AddLine( MenuMousePos, MenuMousePos );
    RedrawWindow( LRtoDR( CRect( net->lines[i].p[0], net->lines[i].p[1] ) ) );

    linedrawing = true;
    lineend = 1;
    iLineSelected = i;

    SetCapture();

    //cursorstate = move;
}
void TNetView::OnMenuDelete()
{
    __int32 i;

    i = net->GetLineByPoint( MenuMousePos );
    if (i!=-1) { 
        POINT p[4];
        
        CRgn rold;

        net->CreatePointArrayFromLine( i, p );
        m_arcDC()->LPtoDP( p, 4 );
        rold.CreatePolygonRgn( p, 4, WINDING );
        
        net->DeleteLine( i );
        
        RedrawWindow( NULL, &rold );

        return;
    };

    i = net->GetNodeByPos( MenuMousePos );
    if (i!=-1) { 
        CRect rect = LRtoDR( net->nodes[i] );
        net->DeleteNode( i );
        RedrawWindow( rect );
    };
}




void TNetView::OnMenuName()
{
    __int32 i;

    i = net->GetLineByPoint( MenuMousePos );
    if (i!=-1) { 
        
        CRect r = GetLineNameRect( i );
        
        CNodeNameDialog dlg;
        dlg.m_nodename = net->lines[i].name;
        dlg.DoModal();
        strcpy( net->lines[i].name, dlg.m_nodename );

        r.UnionRect( r, GetLineNameRect( i ) );        

        RedrawWindow( LRtoDR( r ) );

        return;
    };
    
    i = net->GetNodeByPos( MenuMousePos );
    if (i!=-1) { //this is always true
        CNodeNameDialog dlg;

        dlg.m_nodename = net->nodes[i].name;

        dlg.DoModal();

        strcpy( net->nodes[i].name, dlg.m_nodename);

        CRect rect = LRtoDR( net->nodes[i] );
        RedrawWindow( rect );    
    };
}

void TNetView::OnMenuProp() {
    __int32 i;

    i = net->GetLineByPoint( MenuMousePos );
    if (i!=-1) { 
        
        CRect r = GetLineNameRect( i );
                
        net->lines[i].type = CNetLine::prop;

        r.UnionRect( r, GetLineNameRect( i ) );        

        RedrawWindow( LRtoDR( r ) );

        return;
    };
};
void TNetView::OnMenuInh() {
    __int32 i;

    i = net->GetLineByPoint( MenuMousePos );
    if (i!=-1) { 
        
        CRect r = GetLineNameRect( i );
                
        net->lines[i].type = CNetLine::inh;

        r.UnionRect( r, GetLineNameRect( i ) );        

        RedrawWindow( LRtoDR( r ) );

        return;
    };
}; 
void TNetView::OnMenuTrans() {
    __int32 i;

    i = net->GetLineByPoint( MenuMousePos );
    if (i!=-1) { 
        
        CRect r = GetLineNameRect( i );
                
        net->lines[i].type = CNetLine::trans;

        r.UnionRect( r, GetLineNameRect( i ) );        

        RedrawWindow( LRtoDR( r ) );

        return;
    };
};

void TNetView::OnMenuQuestion()
{
    CQuestionDialog dlg(this);
    dlg.DoModal();
};

CPoint TNetView::DPtoLP(CPoint p)
{
    m_arcDC()->DPtoLP( (tagPOINT*)&p );
    return p;
}

CRect TNetView::DRtoLR(CRect r)
{
    CRect ret( DPtoLP( r.TopLeft() ), DPtoLP( r.BottomRight() ) );
    //r = ret;
    ret.NormalizeRect();
    return ret;
}

CPoint TNetView::LPtoDP(CPoint p)
{
    m_arcDC()->LPtoDP( (tagPOINT*)&p );
    return p;
}

CRect TNetView::LRtoDR(CRect r)
{
    CRect ret( LPtoDP( r.TopLeft() ), LPtoDP( r.BottomRight() ) );
    //r = ret;
    ret.NormalizeRect();
    return ret;
}

CRect TNetView::n(CRect r)
{
    swap( r.top, r.bottom );
    return r;
}

void TNetView::FillNodeRgn(__int32 iNode, CRgn * rgn)
{
    CRgn r1;
    //CRgn r2;
    POINT p[4];
    net->CreatePointArrayFromNode( iNode, p );

    m_arcDC()->LPtoDP( p, 4 );
    rgn->CreatePolygonRgn( p, 4, WINDING );
    //r2.CreateRectRgn(0,0,1,1);
    
    //Combine is not working
    // wrong system coordinates-?
    //CombineRgn( (HRGN)rgn->GetSafeHandle(), (HRGN)rgn->GetSafeHandle(), (HRGN)r1.GetSafeHandle(), RGN_OR);    
}

// the function is used to calculate rectangle containing the node and everything connected to it
void TNetView::CreateRectFromNode(__int32 iNode, CRect * rect)
{
    *rect = net->nodes[iNode];

    for (__int32 i=0; i<net->lines.GetSize(); i++) 
        for (__int32 j=0; j<2; j++)
            if (net->lines[i].node[j] == iNode) {
                if (net->lines[i].node[!j]!=-1) {
                    rect->UnionRect( rect, net->nodes[net->lines[i].node[!j]] );
                }
                else {
                    CRect r( net->lines[i].p[j], net->lines[i].p[!j] );
                    r.NormalizeRect();

                    rect->UnionRect( rect, r );
                };

                rect->UnionRect( rect, GetLineNameRect( i ) );
            };
}


CRect TNetView::GetLineNameRect(__int32 iLine)
{
    // big rect containing whole line
    CRect r( net->lines[iLine].p[0], net->lines[iLine].p[1] );
    // size
    CSize size = m_arcDC()->GetTextExtent( net->lines[iLine].GetName() );
    // rect containing name
    CRect r1( r.CenterPoint(), size );
    r1.OffsetRect( -size.cx/2, -size.cy/2 );

    return r1;
}

void TNetView::RedrawLine(__int32 i)
{
   CRgn r;
   POINT p[4];

   net->CreatePointArrayFromLine( i, p );
   m_arcDC()->LPtoDP( p, 4 );
   r.CreatePolygonRgn( p, 4, WINDING );

   CRect rect = GetLineNameRect( i );
   
   RedrawWindow( NULL, &r );
   RedrawWindow( LRtoDR( rect ) );
}

void TNetView::AddChilds(__int32 n) {

    spathinfo pathinfo;
   
    // add all child nodes
    for (__int32 i=0; i<net->lines.GetSize(); i++)        
        if (net->lines[i].node[0]==n &&
            net->lines[i].type==CNetLine::inh) {
            // should check loops here
            pathinfo.p[0] = n;
            pathinfo.p[1] = net->lines[i].node[1];
            net->lines[i].red = true;
            path.Add( pathinfo );
            AddChilds( net->lines[i].node[1] );
        }
}

void TNetView::SolveQuestion(__int32 l, __int32 n)
{
   spathinfo pathinfo;

   path.RemoveAll();

   CString str;
   
   // add current node as start point
   pathinfo.p[0] = -1;
   pathinfo.p[1] = n;
   path.Add( pathinfo );

   AddChilds( n );

   // for all child nodes
   for (__int32 i=0; i<path.GetSize(); i++) {
     
       // get the answers for all outgoing lines like given one
       for (__int32 j=0; j<net->lines.GetSize(); j++) {       

           if (net->lines[j].node[0]==path[i].p[1] &&
               strcmp( net->lines[j].name, net->lines[l].name ) == 0) {

               // the answer is net->nodes[n].name   net->lines[l].name   net->nodes[net->lines[j].p[1]].name
               //               tree                             is in                forrest

               CString newans;
               newans.Format("%s %s %s", net->nodes[n].name, net->lines[l].name, net->nodes[net->lines[j].node[1]].name);
               str += newans + '\n';

               net->lines[j].red = true;

               // if not property
               if (net->lines[j].type != CNetLine::prop) {
                   pathinfo.p[0] = net->lines[j].node[0];
                   pathinfo.p[1] = net->lines[j].node[1];
                   path.Add( pathinfo );
                   AddChilds( net->lines[j].node[1] );
               };
           };
       }
   }

   if (!str.GetLength()) str = "Ответ не найден.";

   RedrawWindow();
   AfxMessageBox( str );
   for (__int32 k=0; k<net->lines.GetSize(); k++) net->lines[k].red = false;       
   RedrawWindow();   
}
