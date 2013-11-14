/*
  Somewhat advanced slider control
  Floating point values can be assigned to each tick mark.  The
  float conversion operator then returns a linearly interpolated value
  based on the values of the tick marks the slider is between.  The
  default tickmark values are 0,1,2...

  The SetArmPos and SetDetent methods are also based on the tick mark
  values.
*/
#ifndef _SLIDER_INC
#define _SLIDER_INC
#include "control.h"

#define DEFSTYLE  SLS_HORIZONTAL|SLS_CENTER|SLS_HOMELEFT|SLS_PRIMARYSCALE1

class Slider : public virtual Control
   {
    private:
            ULONG    ulNumTicks;    //  Number of Tick Marks in control
            float *  pfTickVals;
    public:
                     Slider (HWND,ULONG,SHORT, SHORT, SHORT,SHORT,ULONG,
                             ULONG=DEFSTYLE);
                     Slider (HWND,ULONG);
                    ~Slider();
                     operator float ();
                     operator ULONG ();
           VOID      SetArmPos(float);
           VOID      SetTicks();
           VOID      SetTickVals(float *);
           VOID      SetDetent(float);
           VOID      SetTickText(SHORT,char *);
   };
#undef DEFSTYLE
#endif
