#if !defined(_MY_CHECKLISTBOX_)
#define _MY_CHECKLISTBOX_

#include "stdafx.h"

#include <afxwin.h>

class TMyChLBox: public CCheckListBox
 {
public:
  TMyChLBox(): CCheckListBox()
   {
     m_clrBkColor = ::GetSysColor( COLOR_WINDOW );
   }
  virtual ~TMyChLBox();

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
  virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

  void SetBkColor( COLORREF clr )
   {
     m_clrBkColor = clr;
   }

private:
  COLORREF m_clrBkColor;
 };

#endif