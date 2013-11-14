#define INCL_WIN
#include <os2.h>
#include <stdio.h>
#include "rbutton.h"

/*
  constructor to use in case the buttons already exist.
  Used by dialog boxes that are defined in the resource file
*/
Rbutton::Rbutton(HWND hDlg, ULONG *ulId)
   {
    //  Create an array of radiobuttons.
    int i;

    for(i=0; ulId[i]; i++)
        ;
    num_buttons = i;
    pControls = new Control [i];
    for(i=num_buttons-1; i>=0; i--)
      pControls[i].hWnd = WinWindowFromID(hDlg,ulId[i]);
}

/*
  constructor to call when the buttons must be created on the fly
*/
Rbutton::Rbutton(HWND hParent, ULONG *ulId, char **title, SHORT xp, SHORT yp,
                 SHORT dx, SHORT dy)
   {
    //  Create an array of radiobuttons.
  /*
    for some mysterious reason, everything works correctly
    if I create the buttons in reverse order.  I suspect the
    resource compiler must do this or something.  In any
    case, it works.  Two things appear weird about this:
    the LAST button created gets the WS_GROUP identifier,
    and it is index 0 when BM_QUERYCHECKINDEX is used.
  */
    int i;

    for(i=0; ulId[i]; i++)
        ;
    num_buttons = i;
    dy /= num_buttons;
    pControls = new Control [i];
    for(i=num_buttons-1; i>=0; i--) {

   pControls[i].hWnd = WinCreateWindow(
              hParent,       /* Parent Window Handle */
              WC_BUTTON,     /* Button Window class */
              (PSZ) title[i],         /* Button Title */
              BS_AUTORADIOBUTTON |
              ((i == 0) ? WS_GROUP : 0),       /* Button style */
              xp,   /* Xcoord */
              yp+i*dy,   /* Ycoord */
              dx,   /* Width */
              dy,    /* Height */
              hParent,       /* Owner Window Handle */
              HWND_TOP,      /* Top of Z order */
              ulId[i],          /* Window ID */
              (PVOID)NULL,   /* Control Data Structure */
              (PVOID)NULL);  /* no presentation parameters */
    pControls[i].fCreated = TRUE;
  }
}

// make invisible
BOOL Rbutton::Hide()
{
  int i;
  BOOL b=TRUE;
    for(i=0; i<num_buttons; i++)
       b = b && pControls[i].Hide();
return b;
}

// make visible
BOOL Rbutton::Show()
{
int i;
BOOL b = TRUE;
  for(i=0; i<num_buttons; i++)
     b = b && pControls[i].Show();
return b;
}

Rbutton::~Rbutton()
{
int i;
  delete []  pControls;
}

// return index of currently chosen radiobutton
Rbutton::operator ULONG()
{
  return (ULONG) WinSendMsg(
               pControls[0].hWnd,     /* my window handle */
               BM_QUERYCHECKINDEX, /* Message for querrying buttons */
               NULL,
               NULL);
}

// set current radio button
VOID Rbutton::SetState(ULONG ulWhich)
{
  if(ulWhich >= num_buttons) ulWhich = 0;
  WinSendMsg(pControls[ulWhich].hWnd,BM_SETCHECK, (MPARAM) TRUE,NULL);
}

// adjust position/sizes of windows to fit given box.
VOID Rbutton::SetPos(SHORT xp, SHORT yp, SHORT width, SHORT height)
{
int i,dy;
dy = height / num_buttons;
for(i=0; i<num_buttons; i++)
   pControls[i].SetPos(xp,yp+i*dy,width,dy);
}
