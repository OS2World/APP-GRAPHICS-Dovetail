
#include "process.h"

// *******************************************************************************************************************

Process::Process ()
   {
    //  Register thread to use Presentation Manager function calls.

    hAnchrBlk = WinInitialize (0);

    //  Create messge queue for the thread.

    hMsgQue = WinCreateMsgQueue (hAnchrBlk,      // Anchor Block handle
                                 0);             // Queue size, use default
   }

// *******************************************************************************************************************

HAB        Process::AnchorBlock ()
                   {
                    return  hAnchrBlk;
                   }

// *******************************************************************************************************************

HMQ        Process::MsgQue ()
                    {
                     return  hMsgQue;
                    }

// *******************************************************************************************************************

