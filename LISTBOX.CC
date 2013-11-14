#include "listbox.h"


// Constructor for window created in resource file
Listbox::Listbox(HWND hParent, ULONG ulId) : Control(hParent, ulId)
{
}

// Constructor for window created on the fly
Listbox::Listbox(HWND hParent, ULONG ulId, SHORT xp, SHORT yp, SHORT dx,
                 SHORT dy, ULONG ulStyle) : Control()
   {

   hWnd = WinCreateWindow(
              hParent,       /* Parent Window Handle */
              WC_LISTBOX,     /* ListBox Window class name */
              NULL,     /* No Window Text */
              ulStyle, /* Listbox style */
              xp,   /* Xcoord */
              yp,   /* Ycoord */
              dx,   /* Width */
              dy,    /* Height */
              hParent,       /* Owner Window Handle */
              HWND_TOP,      /* Sibling Window Handle */
              ulId,     /* Window ID */
              (PVOID)NULL,     /* Control Data Structure */
              (PVOID)NULL);  /* no presentation parameters */
   fCreated = TRUE;
}

// return text of selection in listbox

 Listbox::operator char *()
{
  SHORT length;
  char *text;
  SHORT curitem;

  curitem = (SHORT) WinSendMsg( hWnd,
             LM_QUERYSELECTION,
             (PVOID) NULL,(PVOID) NULL);
  if(curitem == LIT_NONE)
      return (char *) NULL;
  length = (SHORT) WinSendMsg( hWnd,
             LM_QUERYITEMTEXTLENGTH,
             MPFROMSHORT(curitem),(PVOID) NULL);
  text = new char[length+1];
  WinSendMsg( hWnd,
             LM_QUERYITEMTEXT,
             MPFROM2SHORT(curitem,length+1),MPFROMP(text));
  return text;
}

// return index of current selection
 Listbox::operator SHORT ()
{

  return (SHORT) WinSendMsg( hWnd,
                   LM_QUERYSELECTION,
                   (PVOID) NULL,(PVOID) NULL);
}

// insert item
VOID Listbox::Insert(char * text,SHORT sWhere)
{
  WinSendMsg(hWnd,
             LM_INSERTITEM,
             MPFROMSHORT(sWhere),
             MPFROMP(text));
}

// delete item.  if sWhich = -1 (the default), delete currently selected item
VOID Listbox::Delete(SHORT sWhich)
{
  if(sWhich == -1)   // delete currently selected item
    sWhich = (SHORT) WinSendMsg( hWnd,
                       LM_QUERYSELECTION,
                       (PVOID) NULL,(PVOID) NULL);
  WinSendMsg(hWnd,
             LM_DELETEITEM,
             MPFROMSHORT(sWhich),
             NULL);
}

VOID Listbox::DeleteAll()
{
  WinSendMsg(hWnd,
             LM_DELETEALL,
             NULL,NULL);
}
