//**************************************************************************
#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include "dove.h"
#include "process.h"
#include "sbutton.h"
#include "efield.h"
//**************************************************************************


extern "C"
 {
   // This function must be exported so don't mangle the name

   MRESULT EXPENTRY StockDlgProc (HWND, USHORT, MPARAM, MPARAM);
 }
MRESULT EXPENTRY StockDlgProc (HWND    hwnd,
                                   USHORT  msg,
                                   MPARAM  mp1,
                                   MPARAM  mp2)

  {// Variable Declarations
    static Sbutton *psUnits = NULL;
    static Efield *peThickness = NULL,
                  *peLength = NULL;
   static char *units[] = {"IN","MM","CM"};
   static float to_mm[] = {25.4,1.,10.};
   static char *fmt[] = {"%.3f","%.2f","%.3f"};
   static float factor, fThick, fLen;
   static int unit;
   char *text;
   char newtext[32];

   switch(msg) {
     case WM_INITDLG:
        fThick=fThickness;
        fLen=fLength;
        peLength = NULL;
        factor=to_mm[old_unit];
        psUnits = new Sbutton(hwnd,ID_UNITS);
        psUnits->SetArray(units,3);
        psUnits->SetValue(old_unit);
        peThickness = new Efield(hwnd,ID_THICKNESS);
        sprintf(newtext,fmt[old_unit],fThick/factor);
        peThickness->SetText(newtext);
        peLength = new Efield(hwnd,ID_LENGTH);
        sprintf(newtext,fmt[old_unit],fLen/factor);
        peLength->SetText(newtext);
        unit=old_unit;
        break;
     case WM_COMMAND:
        switch(SHORT1FROMMP(mp1)) {
           case ID_OK:
             text = (char *) *peThickness;
             fThickness = atof(text)*to_mm[unit];
             delete [] text;
             delete peThickness;
             text = (char *) *peLength;
             fLength = atof(text)*to_mm[unit];
             delete [] text;
             delete peLength;
             old_unit=unit;
             delete psUnits;
             WinPostMsg(hMain,UM_RECALC,0,0);
             break;
           case ID_CANCEL:
             delete peThickness;
             delete peLength;
             delete psUnits;
             break;
         }
     case WM_CONTROL:
        switch(SHORT1FROMMP(mp1)) {
           case ID_UNITS:
             if(SHORT2FROMMP(mp1) == SPBN_CHANGE) {
                unit = (ULONG) *psUnits;
                if(to_mm[unit] != factor && peLength) { /* alter entry fields */
                   text = (char *) *peThickness;
                   fThick = atof(text)*factor;
                   delete [] text;
                   sprintf(newtext,fmt[unit],fThick/to_mm[unit]);
                   peThickness->SetText(newtext);
                   text = (char *) *peLength;
                   fLen = atof(text)*factor;
                   delete [] text;
                   sprintf(newtext,fmt[unit],fLen/to_mm[unit]);
                   peLength->SetText(newtext);
                   factor=to_mm[unit];
                }
             }
             break;
         }
         break;
     }
     return WinDefDlgProc(hwnd,msg,mp1,mp2);
 }
