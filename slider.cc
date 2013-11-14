#include "slider.h"


Slider::Slider(HWND hParent, ULONG ulId) : Control(hParent, ulId)
{
    WNDPARAMS   wprm;                   // Window parameters ct;
    SLDCDATA    sldcd;                  // Slider control data

    //----------------------------------------------------------
    // Get original slider dimensions in increments for future
    // calls.  (My thanks to Wayne Kovsky for this example of 
    // using WM_QUERYWINDOWPARAMS.)
    //----------------------------------------------------------
    wprm.fsStatus   = WPM_CTLDATA;      // Request control data
    wprm.cbCtlData  = sizeof (SLDCDATA);
    wprm.pCtlData   = &sldcd;

    WinSendMsg (hWnd, WM_QUERYWINDOWPARAMS,
                     MPFROMP(&wprm), 0);
    ulNumTicks = sldcd.usScale1Increments;
    pfTickVals = new float[ulNumTicks];
    for(int i=0; i<ulNumTicks; i++)
       pfTickVals[i] = i;
}

Slider::Slider(HWND hParent, ULONG ulId, SHORT xp, SHORT yp, SHORT dx, SHORT dy,
     ULONG ulInc, ULONG ulSliderStyle) : Control()
   {
    //  Create a simple slider.
    SLDCDATA sldcData;

   sldcData.cbSize=sizeof(SLDCDATA);
   sldcData.usScale1Increments = ulInc;
   sldcData.usScale1Spacing = 0;
   ulNumTicks = ulInc;
   pfTickVals = new float[ulNumTicks];
   for(int i=0; i<ulNumTicks; i++)
      pfTickVals[i] = i;

   hWnd = WinCreateWindow(
              hParent,       /* Parent Window Handle */
              WC_SLIDER,     /* Slider Window class name */
              NULL,     /* No Window Text */
              ulSliderStyle &
              ~WS_VISIBLE, /* Slider style but not visible*/
              xp,   /* Xcoord */
              yp,   /* Ycoord */
              dx,   /* Width */
              dy,    /* Height */
              hParent,       /* Owner Window Handle */
              HWND_TOP,      /* Sibling Window Handle */
              ulId,     /* Slider Window ID */
              &sldcData,     /* Control Data Structure */
              (PVOID)NULL);  /* no presentation parameters */
   fCreated=TRUE;
}

Slider::~Slider()
{
delete pfTickVals;
}

// associate float values with each tick mark
void Slider::SetTickVals(float *vals)
{
for(int i=0; i<ulNumTicks; i++)
  pfTickVals[i] = vals[i];
}

// get interpolated value of slider arm
 Slider::operator float()
{
  ULONG ulv;
 
  ulv = (ULONG) WinSendMsg(
               hWnd,     /* my window handle */
               SLM_QUERYSLIDERINFO, /* Message for querrying slider */
               MPFROM2SHORT(
                 SMA_SLIDERARMPOSITION,
                 SMA_RANGEVALUE),
               NULL);
  float t =  (float)(ulNumTicks-1) *
          (float)(USHORT)ulv/(float)(USHORT)(ulv >> 16);
  int i= (int) t;
  t -= i;
  return pfTickVals[i] + t*(pfTickVals[i+1]-pfTickVals[i]);
}

// get slider arm position in terms of ticks
 Slider::operator ULONG()
{
  return (ULONG) WinSendMsg(
               hWnd,     /* my window handle */
               SLM_QUERYSLIDERINFO, /* Message for querrying slider */
               MPFROM2SHORT(
                 SMA_SLIDERARMPOSITION,
                 SMA_INCREMENTVALUE),
               NULL);
}

// set size of tick marks.  6 should be a variable, I suppose.
VOID Slider::SetTicks()
{
WinSendMsg(hWnd,SLM_SETTICKSIZE, MPFROM2SHORT(SMA_SETALLTICKS,6),NULL);
}

// set a detent at a given position
VOID Slider::SetDetent(float fWhere)
{
SHORT sPos;
ULONG ulv;
 
  ulv = (ULONG) WinSendMsg(
               hWnd,     /* my window handle */
               SLM_QUERYSLIDERINFO, /* Message for querrying slider */
               MPFROM2SHORT(
                 SMA_SLIDERARMPOSITION,
                 SMA_RANGEVALUE),
               NULL);
sPos = (SHORT)(fWhere*(float)(SHORT)(ulv>>16)/(float)(ulNumTicks-1));
WinSendMsg(hWnd,SLM_ADDDETENT,MPFROMSHORT(sPos),NULL);
}

// set text over a tick mark
VOID Slider::SetTickText(SHORT sPos,char * cpText)
{
WinSendMsg(hWnd,SLM_SETSCALETEXT,MPFROMSHORT(sPos),
           MPFROMP(cpText));
}

// set slider arm position in terms of float values
VOID Slider::SetArmPos(float fWhere)
{
SHORT sPos;
ULONG ulv;
float t;

if(fWhere < pfTickVals[0]) fWhere = pfTickVals[0];
if(fWhere > pfTickVals[ulNumTicks-1]) fWhere = pfTickVals[ulNumTicks-1];

for(int i=0; i<ulNumTicks-1; i++)
  if(fWhere < pfTickVals[i+1]) break;

t = (fWhere-pfTickVals[i])/(pfTickVals[i+1]-pfTickVals[i]);

fWhere = i+t;
 
  ulv = (ULONG) WinSendMsg(
               hWnd,     /* my window handle */
               SLM_QUERYSLIDERINFO, /* Message for querrying slider */
               MPFROM2SHORT(
                 SMA_SLIDERARMPOSITION,
                 SMA_RANGEVALUE),
               NULL);
sPos = (SHORT)(fWhere*(float)(SHORT)(ulv>>16)/(float)(ulNumTicks-1));
WinSendMsg(hWnd,SLM_SETSLIDERINFO,MPFROM2SHORT(SMA_SLIDERARMPOSITION,
                                           SMA_RANGEVALUE),
                              MPFROMSHORT(sPos));
}
