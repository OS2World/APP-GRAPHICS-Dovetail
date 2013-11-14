/*
  Radio button group.  This is handled by having an array of Controls.
  They are assumed to be within a GROUP, so that setting one automatically
  unsets the others, etc.
*/
#ifndef _RBUTTON_INC
#define _RBUTTON_INC
#include "control.h"


class Rbutton
   {
    private:
            Control *pControls;
            int num_buttons;
    public:
                     Rbutton (HWND,ULONG *);
                     Rbutton (HWND,ULONG *,char **, SHORT=0, SHORT=0,
                              SHORT=100, SHORT=100);
                    ~Rbutton ();
           BOOL      Hide();
           BOOL      Show();
           VOID      SetPos(SHORT, SHORT, SHORT, SHORT);
                     operator ULONG ();
           VOID      SetState(ULONG);
   };
#endif
