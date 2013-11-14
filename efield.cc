#include "efield.h"

Efield::Efield(HWND hParent, ULONG ulId) : Control(hParent, ulId)
{
}

Efield::Efield(HWND hParent, ULONG ulId,
        SHORT xp, SHORT yp, SHORT dx, SHORT dy, ULONG ulStyle) : Control()
   {
    //  Create a simple entry field.

   hWnd = WinCreateWindow(
              hParent,       /* Parent Window Handle */
              WC_ENTRYFIELD,     /* Entry Field Window class name */
              NULL,     /* No Window Text */
              ulStyle,
              xp,   /* Xcoord */
              yp,   /* Ycoord */
              dx,   /* Width */
              dy,    /* Height */
              hParent,       /* Owner Window Handle */
              HWND_TOP,      /* Sibling Window Handle */
              ulId,     /* Slider Window ID */
              (PVOID)NULL,     /* Control Data Structure */
              (PVOID)NULL);  /* no presentation parameters */
   fCreated = TRUE;
}

// return text of entry field
 Efield::operator char *()
{
  int length;
  char *text;

  length = WinQueryWindowTextLength(hWnd);
  text = new char[length+1];
  WinQueryWindowText(hWnd,length+1,(PSZ) text);
  return text;
}

VOID Efield::SetText(char *text)
{
  WinSetWindowText(hWnd,(PSZ) text);
}

VOID Efield::Clear()
{
   WinSendMsg(hWnd, EM_CLEAR, (PVOID) NULL,(PVOID) NULL);
}

VOID Efield::SetLength(ULONG ulLength)
{
   WinSendMsg(hWnd, EM_SETTEXTLIMIT, (MPARAM) ulLength,(MPARAM) 0);
}
