/*
  Entry field.  Basic support only
*/
#ifndef _EFIELD_INC
#define _EFIELD_INC
#include "control.h"


class Efield : public virtual Control
   {
    public:
                     Efield(HWND, ULONG);
                     Efield(HWND, ULONG, SHORT, SHORT, SHORT, SHORT,
                     ULONG=(ES_AUTOSCROLL | ES_MARGIN));
    virtual          operator char * ();
            VOID     SetText(char *);
            VOID     SetLength(ULONG);
            VOID     Clear();
   };
#endif
