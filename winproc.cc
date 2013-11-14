#define INCL_WIN
#define INCL_GPI
#define INCL_DEV
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_SPLERRORS
#define INCL_BASE
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dove.h"
#include "process.h"

extern "C"
 {
   // This function must be exported so don't mangle the name

   MRESULT ClientWndProc (HWND, USHORT, MPARAM, MPARAM);

   MRESULT EXPENTRY ParmDlgProc (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY StockDlgProc (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY FileDlgProc (HWND, USHORT, MPARAM, MPARAM);
 }
BOOL calc_dt(void);
void draw_dovetail(HPS hPS);
extern int ok_to_post;
float *pfPins = NULL;
float *pfTails = NULL;
static int print_count=0;
int read_write;
                               
MRESULT EXPENTRY ClientWndProc (HWND    hwnd,
                                   USHORT  msg,
                                   MPARAM  mp1,
                                   MPARAM  mp2)

  {// Variable Declarations

              RECTL    INVLDRECT;
              HPS      hPresSp;
       static HWND hWnd = 0;
       POINTL coords;
       MATRIXLF matrix;
       float width, height,bot,scale;
       ULONG whole, frac;
   //  Main Program Logic  

   switch (msg)
     {
      case WM_CREATE:

           return 0;

      case WM_DESTROY:
           if(hWnd) WinDestroyWindow(hWnd);
           return 0;

      case WM_PAINT:
           if(hWnd == 0)
//           hWnd=WinLoadDlg(HWND_DESKTOP,hwnd,ParmDlgProc,0,ID_PARAMS,0);
             hWnd=WinLoadDlg(hwnd,hwnd,ParmDlgProc,0,ID_PARAMS,0);
/*
  scale the graphics as follows:  1mm = 100 units in world space.
  Therefore the length of the graphics will be 100*fLength x 100*fWidth.
  Want this to fit within the window, with say %5 slop on each side.
*/
           if(pfPins) {
             hPresSp = WinBeginPaint (hwnd, 0, NULL);
             WinQueryWindowRect (hwnd, &INVLDRECT);
             WinFillRect (hPresSp, &INVLDRECT, CLR_WHITE);
             width = INVLDRECT.xRight - INVLDRECT.xLeft;
             height = INVLDRECT.yTop - INVLDRECT.yBottom;

             matrix.lM13 = matrix.lM23 = 0;
             matrix.lM33 = 1;
             matrix.fxM12 = matrix.fxM21 = MAKEFIXED(0,0);
             matrix.lM31 = INVLDRECT.xLeft+(ULONG)(width/20.);
             scale = .9*width/(100.*fLength);
             whole = (ULONG)scale;
             frac = (ULONG)(65536*(scale-whole));
             matrix.fxM22 = matrix.fxM11 = MAKEFIXED(whole,frac);
/*
  set y translation so that top of part will be 5% from top of window
*/
             bot = .95*height-100.*fThickness*scale;
             matrix.lM32 = INVLDRECT.yBottom+(ULONG) bot;

             GpiSetModelTransformMatrix(hPresSp,9,&matrix,TRANSFORM_REPLACE);
             draw_dovetail(hPresSp);
             WinEndPaint (hPresSp);
           }
           return 0;
      case WM_COMMAND:
        switch(SHORT1FROMMP(mp1))
          {
         case IDM_STOCK:
           WinDlgBox(HWND_DESKTOP,hwnd,StockDlgProc,0,ID_SDBOX,0);
           return 0;
         case IDM_FILE_OPEN:
           read_write=0;
           WinDlgBox(HWND_DESKTOP,hwnd,FileDlgProc,0,ID_FILEDB,0);
           return 0;
         case IDM_FILE_SAVE:
           read_write=1;
           WinDlgBox(HWND_DESKTOP,hwnd,FileDlgProc,0,ID_FILEDB,0);
           return 0;
         case IDM_FILE_DEL:
           read_write=2;
           WinDlgBox(HWND_DESKTOP,hwnd,FileDlgProc,0,ID_FILEDB,0);
           return 0;
         case IDM_PRINT:
/*
  want to print this sucker out.
*/
           extern Process Proc;
           DEVOPENSTRUC dop;
           HDC hdcPrinter;
           char msg[64];
/*
  first, get DC for default printer
*/
           SPLERR splerr;
           ULONG  cTotal;
           ULONG  cReturned ;
           ULONG  cbNeeded,cbBuf ;
           int i,j;
           PRQINFO3 *prq ;
           PBYTE pBuf;

        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)"Getting Queue",
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
           splerr = SplEnumQueue((PSZ)NULL, 3, pBuf, 0,
                              &cReturned, &cTotal,
                              &cbNeeded, NULL);
           if(splerr != ERROR_MORE_DATA && splerr != 2123) {  /* error */
              WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ)"Error getting printer info",
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
             return 0;
           }
           pBuf = (PBYTE) malloc(cbNeeded);
           cbBuf = cbNeeded;
           splerr = SplEnumQueue((PSZ)NULL, 3, pBuf, cbBuf,
                              &cReturned, &cTotal,
                              &cbNeeded, NULL);
           if(splerr != 0) {  /* error */
              WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ)"Error getting printer info",
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
             return 0;
           }
           prq = (PPRQINFO3) pBuf;
           for (i=j=0;i < cReturned ; i++)
             if ( (prq+i)->fsType & 4 ) {  /* get default printer */
               j=i;
               break;
             }
           char Dname[32];
           sprintf(Dname,"ret=%d, j=%d",cReturned,j);
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)Dname,
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
           HAB hab = Proc.AnchorBlock();
        prq += j;
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, prq->pszName,
                     (PSZ) "Device name", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
           dop.pszLogAddress = prq->pszName;
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, prq->pszDriverName,
                     (PSZ) "Copying name", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
           for(i=0; i<31 && prq->pszDriverName[i] &&
                            prq->pszDriverName[i] != '.'; i++)
                Dname[i]=prq->pszDriverName[i];
           Dname[i]=0;
           dop.pszDriverName = (PSZ) Dname;
//           dop.pdriv = (PDRIVDATA) NULL;
           dop.pdriv = prq->pDriverData;
           dop.pszDataType = (PSZ) "PM_Q_STD";
        free(pBuf);
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)Dname,
                     (PSZ) "Get PDC", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
           hdcPrinter = DevOpenDC(hab,OD_QUEUED,(PSZ) "*",4,
                         (PDEVOPENDATA)&dop,(HDC)NULL);
           if(hdcPrinter == 0) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)"Null printer DC",
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
           } else {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)"Got printer DC",
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
             SIZEL sizl={0,0};
             HPS hps=GpiCreatePS(hab,hdcPrinter,&sizl,
                        PU_HIMETRIC | GPIT_MICRO | GPIA_ASSOC);
             if(hps == 0) {
             sprintf(msg,"Null HPS: rt = %x",WinGetLastError(hab));
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)msg,
                     (PSZ) "Print", (USHORT)0,
                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
             } else {
             char name[8];
             sprintf(name,"DT%4.4d",print_count++);
             DevEscape(hdcPrinter,DEVESC_STARTDOC,7,(PSZ) name,0,NULL);
             matrix.lM13 = matrix.lM23 = 0;
             matrix.lM33 = 1;
             matrix.fxM12 = MAKEFIXED(1,0);
             matrix.fxM21 = MAKEFIXED(1,0);
             matrix.fxM22 = matrix.fxM11 = MAKEFIXED(0,0);
/*
  set x & y translation so that plot will be centered in paper
*/
             LONG paperwh[6];
             DevQueryCaps(hdcPrinter,4,6,paperwh);

             paperwh[0] *= 100000./(float) paperwh[4];  /* width in .01 mm*/
             paperwh[1] *= 100000./(float) paperwh[5];  /* height in .01 mm */

//        sprintf(msg,"Scale factors %d %d",paperwh[0],paperwh[1]);
//        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)msg,
//                     (PSZ) "Print", (USHORT)0,
//                     MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
             matrix.lM31 = (ULONG)(-50.*fThickness+paperwh[0]/2.);
             matrix.lM32 = (ULONG)(-50.*fLength+paperwh[1]/2.);

             GpiSetModelTransformMatrix(hps,9,&matrix,TRANSFORM_REPLACE);
             draw_dovetail(hps);
             USHORT rt;
             DevEscape(hdcPrinter,DEVESC_ENDDOC,0,NULL,2,(PSZ) &rt);
             GpiDestroyPS(hps);
             DevCloseDC(hdcPrinter);
             }
           }
           return 0;
          }
         break;
       case UM_UPDATE:  // pass through from file dialog proc to param dlg proc
           WinPostMsg(hWnd,UM_UPDATE,0,0);
           WinPostMsg(hwnd,UM_RECALC,0,0); // repaint new data
         break;
       case UM_RECALC:
           if(calc_dt()) WinPostMsg(hWnd,UM_UPDATE,0,0);
           WinInvalidateRect (hwnd,NULL,FALSE);
           ok_to_post = 1;
         break;
     }
   return WinDefWindowProc (hwnd, msg, mp1, mp2);
  }

#define MINSIZE 3.
BOOL calc_dt()
{
/*
  first, compute the minimum pin and tail sizes.  All units are in
  inches.
*/
// FILE *fp;
// fp=fopen("calclog","w");
BOOL need_update = FALSE;
float MinSize = fThickness/fSlope+MINSIZE;
int NumTails = (int) (fLength/MinSize/2.);
if(NumTails > iNumTails) NumTails = iNumTails;
// fprintf(fp,"Numtails = %d, iNumTails = %d\n",NumTails,iNumTails);
if(NumTails < iNumTails) need_update = TRUE;
iNumTails = NumTails;
float StockLeft = fLength - 2.*MinSize*NumTails;
// fprintf(fp,"Length=%f, MinSize=%f, StockLeft=%f\n",fLength,MinSize,StockLeft);
if(pfPins != NULL) delete pfPins;
pfPins = new float[NumTails+1];
if(pfTails != NULL) delete pfTails;
pfTails = new float[NumTails];
for(int i=0; i<NumTails; i++) {
  pfPins[i] = pfTails[i] = MinSize;
}
pfPins[0] /= 2.;
pfPins[NumTails] = pfPins[0];
/*
   The pfPins and pfTails arrays now hold the centerline width of each
   pin and tail, respectively.  These are the minimum possible sizes
   allowed.  We will now use the remaining stock to enforce the
   Tail to Pin ratio -- as much as the stock allows.
*/
float StockWant = NumTails*MinSize*(fTailPinRatio-1.);

if(StockWant > StockLeft) {
   StockWant = StockLeft;
   fTailPinRatio = StockWant/(NumTails*MinSize)+1.;
   need_update = TRUE;
}
// fprintf(fp,"PTRatio want,left =%f %f %f\n",fTailPinRatio,StockWant, StockLeft-StockWant);
for(i=0; i<NumTails; i++) {
  pfTails[i] += StockWant/NumTails;
}
StockLeft -= StockWant;
if(StockLeft <= 0.) {
    fMidEdgeRatio = 1.;
    fEndPinSize = .5;
    need_update = TRUE;
}
/*
   Now pad out the end pins to the proper ratio, if StockLeft allows
*/
StockWant = (fEndPinSize-.5)*MinSize*2.;
if(StockWant > StockLeft) {
   StockWant = StockLeft;
   fEndPinSize = StockWant/(2.*MinSize)+.5;
   need_update = TRUE;
}
pfPins[0] += StockWant/2.;
pfPins[NumTails] += StockWant/2.;
StockLeft -= StockWant;
// fprintf(fp,"EPSize want,left =%f %f %f\n",fEndPinSize,StockWant, StockLeft);

if(StockLeft <= 0.) {
    fMidEdgeRatio = 1.;
    need_update = TRUE;
}
/*
  Now, if there is stock left, expand the center pins to enforce the
  MidEdgeRatio.  First must figure out how much stock we want.
  At this point we assume the end pins and last tails are fixed in size.

  All of this is of course meaningless if there are fewer than 3 tails.
*/
if(NumTails < 3) goto expand;
if(iMultOrAdd || 1) {  /* Add --> constant difference between adjacent tails */
  int NumSteps = (NumTails-1)/2;
  float PinStep = MinSize*(fMidEdgeRatio-1.)/NumSteps;
  float Step = PinStep*fTailPinRatio;
  int NumPinSteps, NumTailSteps;
  if(NumTails %2 == 0) {
      NumTailSteps = (NumTails*(NumTails-2))/4;
      NumPinSteps = ((NumTails-2)*(NumTails-2))/4;
  } else {
      NumTailSteps = ((NumTails-1)*(NumTails-1))/4;
      NumPinSteps = ((NumTails-1)*(NumTails-3))/4;
  }
// fprintf(fp,"NPS=%d, PS=%f  NTS=%d  TS=%f\n",NumPinSteps,PinStep,NumTailSteps,Step);
  StockWant = NumPinSteps*PinStep+NumTailSteps*Step;
  if(StockWant > StockLeft) {
    StockWant = StockLeft;
    PinStep = StockWant/(NumPinSteps+fTailPinRatio*NumTailSteps);
    Step = PinStep*fTailPinRatio;
    fMidEdgeRatio = (float) (PinStep*NumSteps)/MinSize+1.;
    need_update = TRUE;
  }
  StockLeft -= StockWant;
// fprintf(fp,"MERatio want,left =%f %f %f\n",fMidEdgeRatio,StockWant, StockLeft);
  int j=NumTails-2;
  for(i=1;i<j; i++,j--) {
      pfTails[j] = pfTails[i] = pfTails[i-1]+Step;
  }
  if(NumTails%2) pfTails[NumTails/2] = pfTails[NumTails/2-1]+Step;
  for(i=2, j=NumTails-2; i<j; i++,j--) {
      pfPins[j] = pfPins[i] = pfPins[i-1]+PinStep;
  }
  if(NumTails%2 == 0) pfPins[NumTails/2] = pfPins[NumTails/2-1]+PinStep;
} else if(fMidEdgeRatio != 1.) {  /* Mult --> constant ratio between adjacent tails */
  int NumTailSteps = (NumTails-1)/2;
  int NumPinSteps = (NumTails-1)/2;
  double Ratio = pow((double)fMidEdgeRatio,1./(double)NumTailSteps);
  float StockWant = 2.*(1.+fTailPinRatio)*(Ratio*(1.-fMidEdgeRatio)/
                                      (1.-Ratio)-NumTailSteps);
  if(NumTails %2) {  /* One Extra Tail */
       StockWant += fTailPinRatio*(fMidEdgeRatio*Ratio-1.);
  } else {  /* One Extra Pin */
       StockWant += fMidEdgeRatio*Ratio-1.;
  }
  if(StockWant > StockLeft) {   /* Bummer!  Will have to use Newton */
    StockWant = StockLeft;


//    fMidEdgeRatio = Step*NumSteps/MinSize+1.;
//    need_update = TRUE;
  }
}

/*
  Now, if there is still any stock left (and there should be),
  expand the size of all pins and tails.
*/

expand:
float mult = fLength/(fLength-StockLeft);
// fprintf(fp,"At expand, StockLeft, mult= %f %f\n",StockLeft,mult);
for(i=0; i<NumTails; i++) {
   pfPins[i] *= mult;
   pfTails[i] *= mult;
}
pfPins[NumTails] = pfPins[0];
// fprintf(fp,"FinalData:\n");
// for(i=0; i<=NumTails; i++)
//   fprintf(fp,"Pin %d: %f\n",i,pfPins[i]);
// for(i=0; i<NumTails; i++)
//   fprintf(fp,"Tail %d: %f\n",i,pfTails[i]);
// fclose(fp);
/*
  must still do multiplication case (messy ??) and put in checks for
  knife edge pins.  If knife edge pins, just set the pin increments
  to 0 for the ME ratio part and be careful doing expansion.  Really,
  can even skip TailPin ratio in this case since it is meaningless.
*/
   return (need_update);
}
/*
  this routine does the actual drawing.  Units are in mm, and we will
  draw with 100 world units per mm, with the lower left corner of the
  image at (0,0)
*/
void draw_dovetail(HPS hPS)
{
  POINTL coords;
  int i;
  float curx;

  ULONG dy = (ULONG)(50.*fThickness);
  ULONG dx = (ULONG)(50.*fThickness/fSlope);
  coords.x = 0;
  coords.y = 0;
  GpiSetCurrentPosition(hPS,&coords);
  coords.x = (ULONG)(100*fLength);
  GpiLine(hPS,&coords);
  coords.y = (ULONG)(100*fThickness);
  GpiLine(hPS,&coords);
  coords.x = 0;
  GpiLine(hPS,&coords);
  coords.y = 0;
  GpiLine(hPS,&coords);
  curx=0;
  for(i=0; i<iNumTails; i++) {
    curx += pfPins[i];
    coords.x = (ULONG)(100.*curx)-dx;
    coords.y = 2*dy;
    GpiSetCurrentPosition(hPS,&coords);
    coords.x += 2*dx;
    coords.y = 0;
    GpiLine(hPS,&coords);
    curx += pfTails[i];
    coords.x = (ULONG)(100.*curx)-dx;
    GpiSetCurrentPosition(hPS,&coords);
    coords.x += 2*dx;
    coords.y = 2*dy;
    GpiLine(hPS,&coords);
  }
}
