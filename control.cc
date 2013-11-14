#include "control.h"

// basic functions shared by all controls

Control::Control(HWND hParent, ULONG ulId)
{
    fCreated=FALSE;
    if(ulId > 0)
      hWnd = WinWindowFromID(hParent,ulId);
}
BOOL Control::Hide()
{
      return (BOOL) WinShowWindow(hWnd, FALSE);
}
BOOL Control::Show()
{
     return (BOOL) WinShowWindow(hWnd, TRUE);
}

Control::~Control()
{

// destroy window if we created it.  All subclasses are expected
// to have an option to create the window.

     if(fCreated) WinDestroyWindow(hWnd);
}
VOID Control::SetPos(SHORT xp, SHORT yp, SHORT width, SHORT height)
{
   WinSetWindowPos(
     hWnd,
     0,
     xp, yp,
     width, height,
     SWP_MOVE | SWP_SIZE);
}
