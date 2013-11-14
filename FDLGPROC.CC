//**************************************************************************
#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include "dove.h"
#include "process.h"
#include "combobox.h"
//**************************************************************************


extern "C"
 {
   // This function must be exported so don't mangle the name

   MRESULT EXPENTRY FileDlgProc (HWND, USHORT, MPARAM, MPARAM);
 }
MRESULT EXPENTRY FileDlgProc (HWND    hwnd,
                                   USHORT  msg,
                                   MPARAM  mp1,
                                   MPARAM  mp2)

  { // Variable Declarations
    static Combobox *pcbName = NULL;
    static int n, cur_des=0;
    static DESIGN *pDesign;
    extern int read_write;
    FILE *fp;
    int i;
    char * new_name;
    extern HWND     hwndFrame;

   switch(msg) {
     case WM_INITDLG:
        pcbName = new Combobox(hwnd,ID_NAME);
        pcbName->SetLength(31);
        fp=fopen("dovetail.dat","rb");
        if(fp == NULL) {
          n=0;
          pDesign = new DESIGN [1];  // one for newly created design
        } else {
          fread(&n,sizeof(int),1,fp);
          pDesign = new DESIGN [n+1];  // one more for newly created design
          fread(pDesign,sizeof(DESIGN),n,fp);
          fclose(fp);
        }
        for(i=0; i<n; i++)
          pcbName->Insert(pDesign[i].name);
        if(n > 0) pcbName->SetText(pDesign[cur_des].name);
        break;
     case WM_COMMAND:
        switch(SHORT1FROMMP(mp1)) {
           case ID_OK:
             new_name = (char *) *pcbName;
// if reading
             if(read_write == 0) {
               for(i=0; i<n; i++)
                 if(strcmp(pDesign[i].name,new_name)==0) {
                   fLength = pDesign[i].fLength;
                   fThickness = pDesign[i].fThickness;
                   fSlope = pDesign[i].fSlope;
                   fTailPinRatio = pDesign[i].fTailPinRatio;
                   fMidEdgeRatio = pDesign[i].fMidEdgeRatio;
                   fEndPinSize = pDesign[i].fEndPinSize;
                   iNumTails = pDesign[i].iNumTails;
                   iMultOrAdd = pDesign[i].iMultOrAdd;
                   old_unit = pDesign[i].unit;
                   cur_des = i;
                   WinPostMsg(hMain,UM_UPDATE,0,0);
                   char *text = new char [48];
                   sprintf(text,"Dovetail Designer: %s",new_name);
                   WinSetWindowText(hwndFrame,(PSZ) text);
                   delete[] text;
                 }
// if saving
               } else if(read_write == 1) {
                 for(i=0; i<n; i++)
                   if(strcmp(pDesign[i].name,new_name)==0) break;
                 if(i < n) {
                   if(WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                         (PSZ)"Overwrite existing design?",
                         (PSZ) "Design Save", (USHORT)0,
                         MB_YESNO | MB_APPLMODAL | MB_MOVEABLE) != MBID_YES) {
                     delete [] new_name;
                     return 0;
                   }
                 } else {
                   n++;
                 }
                 strcpy(pDesign[i].name,new_name);
                 pDesign[i].fLength = fLength;
                 pDesign[i].fThickness = fThickness;
                 pDesign[i].fSlope = fSlope;
                 pDesign[i].fTailPinRatio = fTailPinRatio;
                 pDesign[i].fMidEdgeRatio = fMidEdgeRatio;
                 pDesign[i].fEndPinSize = fEndPinSize;
                 pDesign[i].iNumTails = iNumTails;
                 pDesign[i].iMultOrAdd = iMultOrAdd;
                 pDesign[i].unit = old_unit;
                 fp=fopen("dovetail.dat","wb");
                 fwrite(&n,sizeof(int),1,fp);
                 fwrite(pDesign,sizeof(DESIGN),n,fp);
                 cur_des = i;
                 fclose(fp);
                 char *text = new char [48];
                 sprintf(text,"Dovetail Designer: %s",new_name);
                 WinSetWindowText(hwndFrame,(PSZ) text);
                 delete[] text;
               } else if(read_write == 2) {
                 for(i=0; i<n; i++)
                   if(strcmp(pDesign[i].name,new_name)==0) break;
                 if(i >= n) {
                   delete [] new_name;
                   return 0;
                 }
                 if(WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                       (PSZ)"Delete this design?",
                       (PSZ) "Design Delete", (USHORT)0,
                       MB_YESNO | MB_APPLMODAL | MB_MOVEABLE) != MBID_YES) {
                   delete [] new_name;
                   return 0;
                 }
                 if(cur_des == i)
                   cur_des=0;
                 else if(cur_des > i)
                   cur_des--;
                 n--;
                 for(; i<n; i++)
                   pDesign[i]=pDesign[i+1];
                 fp=fopen("dovetail.dat","wb");
                 fwrite(&n,sizeof(int),1,fp);
                 fwrite(pDesign,sizeof(DESIGN),n,fp);
                 fclose(fp);
                 pcbName->Delete(); // delete currently selected item
                 pcbName->Clear();  // Clear entry field text
                 delete[] new_name;
                 WinSetWindowText(hwndFrame,(PSZ) "Dovetail Designer: No name");
                 return 0;
               }
               delete[] new_name;
               delete[] pDesign;
               delete pcbName;
             break;
           case ID_CANCEL:
             delete[] pDesign;
             delete pcbName;
             break;
         }
/*
  in case of enter, send myself OK message
*/
     case WM_CONTROL:
        switch(SHORT1FROMMP(mp1)) {
           case ID_NAME:
             if(SHORT2FROMMP(mp1) == CBN_ENTER)
                WinPostMsg(hwnd,WM_COMMAND,MPFROMSHORT(ID_OK),0);
             break;
         }
         break;
     }
     return WinDefDlgProc(hwnd,msg,mp1,mp2);
 }
