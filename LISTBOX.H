/*
  Listbox.  Supports only insertion, deletion, and getting of text &
            selection index
*/
#ifndef _LISTBOX_INC
#define _LISTBOX_INC
#include "control.h"


class Listbox : virtual public Control
   {
    public:
                     Listbox (HWND,ULONG,SHORT,SHORT,SHORT,SHORT,ULONG=0);
                     Listbox (HWND,ULONG);
    virtual          operator char * ();
                     operator SHORT ();
            VOID     Insert(char *,SHORT=LIT_SORTASCENDING);
            VOID     Delete(SHORT = -1);
            VOID     DeleteAll();
   };
#endif
