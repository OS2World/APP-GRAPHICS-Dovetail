/*
   Combination box.  Basically it has NO new operations:  it is just a
   listbox plus entry field.  The only defined operation is just used to
   select between conflicting char * conversion operators in the bases
*/
#ifndef _COMBOBOX_INC
#define _COMBOBOX_INC
#include "efield.h"
#include "listbox.h"

class Combobox : public Efield, public Listbox
   {
    public:
                     Combobox (HWND,ULONG);
                     Combobox (HWND,ULONG,SHORT,SHORT,SHORT,SHORT,
                               ULONG=CBS_SIMPLE);
                     operator char * () { return Efield::operator char *(); }
   };
#endif
