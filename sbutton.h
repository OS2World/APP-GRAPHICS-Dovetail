/*
  Spin button.  Supports numeric only or array based spin buttons.
  the conversion LONG operator returns the array index for array
  based buttons, or the value for numeric ones.

  There is no support for getting the text out of an array based button,
  only the array index.
*/
#ifndef _SBUTTON_INC
#define _SBUTTON_INC
#include "control.h"


class Sbutton : public virtual Control
   {
    public:
                     Sbutton (HWND,ULONG);
                     Sbutton (HWND,ULONG,SHORT,SHORT,SHORT,SHORT,
              ULONG = SPBS_MASTER |
                      SPBS_NUMERICONLY |
                      SPBS_JUSTRIGHT |
                      SPBS_FASTSPIN);
                     operator LONG ();
           VOID      SetValue(LONG);
           VOID      SetRange(LONG,LONG);
           VOID      SetArray(char **, USHORT);
   };
#endif
