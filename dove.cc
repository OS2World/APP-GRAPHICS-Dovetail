
//**************************************************************************
#include <stdlib.h>
#include "process.h"
#include "dove.h"
//**************************************************************************

extern "C"
   {
    MRESULT ClientWndProc (HWND, USHORT, MPARAM, MPARAM);
   }

//  ********* Global Variables *********************************************

Process        Proc;
float fLength = 5.5*25.4,fThickness=.75*25.4;
float fSlope=8.,fTailPinRatio=1.,fMidEdgeRatio=1.,fEndPinSize=.75;
int iMultOrAdd=1,iNumTails=3;
int old_unit = 0;
HWND hMain=0;
int ok_to_post = 0;
HWND     hwndFrame;

// *************************************************************************

int     main (int       argc,
              char *    argv[])

  {//  Variable Declarations

   static  ULONG    flFrameFlags = FCF_TITLEBAR       |
                                   FCF_SYSMENU        |
                                   FCF_SIZEBORDER     |
                                   FCF_MINMAX         |
                                   FCF_SHELLPOSITION  |
                                   FCF_TASKLIST       |
                              //   FCF_VERTSCROLL     |
                              //   FCF_HORZSCROLL     |
                                   FCF_MENU           |
                                   FCF_ICON;
           HWND     hwndClient;
   static  CHAR     szTitleBarText[] = "Dovetail Designer: No name";
           QMSG     QueMsg;
   static  CHAR     szPrvtWinClass[] = "Private Class";

   //  Register a private window class for this application.

   WinRegisterClass (Proc.AnchorBlock(),  // Anchor Block handle
               (PSZ) szPrvtWinClass,      // Name of window class
             (PFNWP) ClientWndProc,       // Window procedure for class   
                     CS_SIZEREDRAW |
                     CS_CLIPCHILDREN,       // Set redraw on window resize
                     0);                  // Extra bytes to reserve       

   //  Create a standard window frame.

   hwndFrame = WinCreateStdWindow (HWND_DESKTOP,         // Create a parent window                
                                   WS_VISIBLE      |     // Make the window visible               
                            //         FS_ACCELTABLE |     // Use accelerator table
                                     FS_ICON,            // Use Icon                            
                                   &flFrameFlags,        // Creation flags                        
                             (PSZ) szPrvtWinClass,       // window class name                     
                             (PSZ) szTitleBarText,       // Title bar text                        
                                   0L,                   // Style of client window                
                         (HMODULE) NULL,                 // Resource is in application module     
                                   ID_RESOURCE,          // ID of resources for this frame window 
                                   &hwndClient);         // Returned client window handle         
    hMain = hwndClient;
   //  Standard message processing loop. WinGetMsg returns FALSE if the  
   //  returned message is WM_QUIT.  Returns TRUE for all other messages 

   while (WinGetMsg (Proc.AnchorBlock(),   // Anchor block handle                 
                     &QueMsg,            // Pointer to queue message structure  
                     0,               // No window message filter            
                     0,                  // Retrieve all messages               
                     0))                 // Retrieve all messages               
     {WinDispatchMsg(Proc.AnchorBlock(),   // Anchor block handle       
                     &QueMsg);           // structure holding message 
     }

   //  The WM_QUIT message has been received.  Destroy window.  Destroy  
   //  message queue.  Let go of anchor block.                           

   WinDestroyWindow(hwndFrame);

   //  Terminate program. 

   return 0;

  }

