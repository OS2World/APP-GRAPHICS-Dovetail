#include "combobox.h"

// constructor for attaching object to existing control
Combobox::Combobox(HWND hParent, ULONG ulId) : Control(hParent,ulId),
        Efield(0,0), Listbox(0,0)
{
}

// Constructor for window created on the fly
Combobox::Combobox(HWND hParent, ULONG ulId,SHORT xp, SHORT yp,
                   SHORT dx, SHORT dy,ULONG ulStyle) : Listbox(0,0),Efield(0,0)
   {

   hWnd = WinCreateWindow(
              hParent,       /* Parent Window Handle */
              WC_COMBOBOX,
              (PSZ)NULL,     /* No Window Text */
              ulStyle,
              xp,
              yp,
              dx,
              dy,
              hParent,       /* Owner Window Handle */
              HWND_TOP,      /* Sibling Window Handle */
              ulId,     /* Window ID */
              (PVOID)NULL,     /* Control Data Structure */
              (PVOID)NULL);  /* no presentation parameters */
   fCreated = TRUE;
}
