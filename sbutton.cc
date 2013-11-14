#include "sbutton.h"

Sbutton::Sbutton(HWND hParent, ULONG ulId) : Control(hParent, ulId)
{
}


Sbutton::Sbutton(HWND hParent, ULONG ulId, SHORT xp, SHORT yp, SHORT dx,
                 SHORT dy, ULONG ulStyle)
  : Control()
   {
    //  Create a spin button.

   hWnd = WinCreateWindow(
              hParent,       /* Parent Window Handle */
              WC_SPINBUTTON,     /* Button Window class */
              NULL,         /* Button Title */
              ulStyle,
              xp,
              yp,
              dx,
              dy,
              hParent,       /* Owner Window Handle */
              HWND_TOP,      /* Top of Z order */
              ulId,          /* Window ID */
              (PVOID)NULL,   /* Control Data Structure */
              (PVOID)NULL);  /* no presentation parameters */
  fCreated = TRUE;
}

// return value of spin button
Sbutton::operator LONG()
{
LONG val;
   WinSendMsg( hWnd,     /* my window handle */
               SPBM_QUERYVALUE, /* Message for querrying buttons */
               MPFROMP(&val),
               MPFROM2SHORT(0,SPBQ_DONOTUPDATE));
 return val;
}

// set range for numeric spin buttons
VOID Sbutton::SetRange(LONG lLow, LONG lHigh)
{
  WinSendMsg(hWnd,SPBM_SETLIMITS,MPFROMLONG(lHigh),MPFROMLONG(lLow));
}

// set current value of spin button
VOID Sbutton::SetValue(LONG lValue)
{
  WinSendMsg(hWnd,SPBM_SETCURRENTVALUE, MPFROMLONG(lValue),NULL);
}

// set array for array based spin buttons
VOID Sbutton::SetArray(char **data, USHORT num)
{
  WinSendMsg(hWnd,SPBM_SETARRAY,MPFROMP(data),(MPARAM)num);
}
