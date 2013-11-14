/*
  The basic control type.  This handles the basic, common functionality
  of all controls
*/
#ifndef _CONTROL_INC
#define _CONTROL_INC
#define INCL_WIN
#include <os2.h>  // determine min include needed for HWND


class Control // base class for all controls
   {
    friend class Rbutton; // for access to hWnd and fCreated.
    protected:
            HWND     hWnd;
            BOOL     fCreated;
    public:
                     Control (HWND,ULONG);
                     Control () {fCreated=TRUE; }
   virtual          ~Control ();
   virtual VOID      SetPos(SHORT, SHORT, SHORT, SHORT);
   virtual BOOL      Show();
   virtual BOOL      Hide();
   };
#endif
