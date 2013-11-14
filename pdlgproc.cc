
//**************************************************************************
#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include "dove.h"
#include "process.h"
#include "slider.h"
#include "rbutton.h"
#include "sbutton.h"
//**************************************************************************


extern "C"
 {
   // This function must be exported so don't mangle the name

   MRESULT EXPENTRY ParmDlgProc (HWND, USHORT, MPARAM, MPARAM);
 }
MRESULT EXPENTRY ParmDlgProc (HWND    hwnd,
                                   USHORT  msg,
                                   MPARAM  mp1,
                                   MPARAM  mp2)

  {// Variable Declarations
    static Slider *psTailPin=NULL,
                  *psMidEdge=NULL,
                  *psEndPin =NULL,
                  *psSlope  =NULL;
    static Rbutton *prMultOrAdd = NULL;
    static Sbutton *psNumTails = NULL;
    extern HWND hMain;
    extern int ok_to_post;
    int invalid,i;

   invalid=0;
   switch(msg) {
     case UM_UPDATE:
        ok_to_post = 0;
        psNumTails->SetValue(iNumTails);
        psTailPin->SetArmPos(fTailPinRatio);
        psSlope->SetArmPos(fSlope);
        psMidEdge->SetArmPos(fMidEdgeRatio);
        psEndPin->SetArmPos(fEndPinSize);
        ok_to_post = 1;
        break;
     case WM_INITDLG:
        float *vals = new float[10];
        psTailPin = new Slider(hwnd,ID_TAILPINRATIO);
        psTailPin->SetTicks();
        psTailPin->SetTickText(0,".5");
        psTailPin->SetTickText(1,"1");
        psTailPin->SetTickText(2,"2");
        psTailPin->SetTickText(3,"3");
        psTailPin->SetTickText(4,"4");
        psTailPin->SetTickText(5,"K");
        psTailPin->SetDetent(1.);
        psTailPin->SetDetent(2.);
        vals[0] = .5;
        vals[1] = 1.;
        vals[2] = 2.;
        vals[3] = 3.;
        vals[4] = 4.;
        vals[5] = 5.;
        psTailPin->SetTickVals(vals);
        psTailPin->SetArmPos(fTailPinRatio);
        psSlope = new Slider(hwnd,ID_SLOPE);
        psSlope->SetTicks();
        psSlope->SetTickText(0,"4");
        psSlope->SetTickText(2,"6");
        psSlope->SetTickText(4,"8");
        psSlope->SetTickText(6,"10");
        vals[0] = 4.;
        vals[1] = 5.;
        vals[2] = 6.;
        vals[3] = 7.;
        vals[4] = 8.;
        vals[5] = 9.;
        vals[6] = 10.;
        psSlope->SetTickVals(vals);
        psSlope->SetDetent(2.);
        psSlope->SetDetent(4.);
        psSlope->SetArmPos(fSlope);
        psMidEdge = new Slider(hwnd,ID_MIDEDGERATIO);
        psMidEdge->SetTicks();
        psMidEdge->SetTickText(0,".5");
        psMidEdge->SetTickText(1,"1");
        psMidEdge->SetTickText(2,"2");
        psMidEdge->SetTickText(3,"4");
        vals[0] = .5;
        vals[1] = 1.;
        vals[2] = 2.;
        vals[3] = 4.;
        psMidEdge->SetTickVals(vals);
        psMidEdge->SetDetent(1.);
        psMidEdge->SetArmPos(fMidEdgeRatio);
        psEndPin = new Slider(hwnd,ID_ENDPINSIZE);
        psEndPin->SetTicks();
        psEndPin->SetTickText(0,"K");
        psEndPin->SetTickText(1,".5");
        psEndPin->SetTickText(2,".75");
        psEndPin->SetTickText(3,"1");
        psEndPin->SetTickText(4,"2");
        psEndPin->SetTickText(5,"4");
        vals[0] = 0.;
        vals[1] = .5;
        vals[2] = .75;
        vals[3] = 1.;
        vals[4] = 2.;
        vals[5] = 4.;
        psEndPin->SetTickVals(vals);
        delete vals;
        psEndPin->SetDetent(1.);
        psEndPin->SetDetent(2.);
        psEndPin->SetArmPos(fEndPinSize);
        ULONG ids[] = {ID_MULT,ID_ADD,0};
        prMultOrAdd = new Rbutton(hwnd,ids);
        prMultOrAdd->SetState(1);
        i=iNumTails;
        psNumTails = new Sbutton(hwnd,ID_NUMTAILS);
        psNumTails->SetRange(1,40); // this call resets iNumTails
        iNumTails = i;
        psNumTails->SetValue(iNumTails);
        ok_to_post = 1;
        invalid=1;
        break;
     case WM_CONTROL:
        switch(SHORT1FROMMP(mp1)) {
           case ID_TAILPINRATIO:
             if(SHORT2FROMMP(mp1) == SLN_CHANGE ||
                SHORT2FROMMP(mp1) == SLN_SLIDERTRACK) {
                float t = (float) *psTailPin;
                if(t != fTailPinRatio) {
                   fTailPinRatio = t;
                   invalid = 1;
                }
             }
             break;
           case ID_ENDPINSIZE:
             if(SHORT2FROMMP(mp1) == SLN_CHANGE ||
                SHORT2FROMMP(mp1) == SLN_SLIDERTRACK) {
                float t = (float) *psEndPin;
                if(t != fEndPinSize) {
                   fEndPinSize = t;
                   invalid = 1;
                }
             }
             break;
           case ID_MIDEDGERATIO:
             if(SHORT2FROMMP(mp1) == SLN_CHANGE ||
                SHORT2FROMMP(mp1) == SLN_SLIDERTRACK) {
                float t = (float) *psMidEdge;
                if(t != fMidEdgeRatio) {
                   fMidEdgeRatio = t;
                   invalid = 1;
                }
             }
             break;
           case ID_SLOPE:
             if(SHORT2FROMMP(mp1) == SLN_CHANGE ||
                SHORT2FROMMP(mp1) == SLN_SLIDERTRACK) {
                float t = (float) *psSlope;
                if(t != fSlope) {
                   fSlope = t;
                   invalid = 1;
                }
             }
             break;
           case ID_MULT:
           case ID_ADD:
             i = (int) (ULONG) *prMultOrAdd;
             if(i != iMultOrAdd) {
                iMultOrAdd = i;
                invalid = 1;
             }
             break;
           case ID_NUMTAILS:
             if(SHORT2FROMMP(mp1) == SPBN_CHANGE) {
               i = *psNumTails;
               if(i != iNumTails) {
                  iNumTails = i;
                  invalid = 1;
               }
             }
             break;
         }
         break;
     }
     if(invalid) {
       if(ok_to_post) WinPostMsg(hMain,UM_RECALC,0,0);
       ok_to_post = 0;
     }
     return WinDefDlgProc(hwnd,msg,mp1,mp2);
 }
