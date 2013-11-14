/********************************************/
/*                                          */
/* Standard Program Header File             */ 
/*                                          */
/********************************************/

/* Name Indentifier for the program icon. */

#define ID_RESOURCE      100
#define ID_OK            101
#define ID_CANCEL        102

/* And the Dialog box stuff */

#define ID_SDBOX         110
#define    ID_THICKNESS     111
#define    ID_LENGTH        112
#define    ID_UNITS         113

#define ID_PARAMS        120
#define    ID_SLOPE         121
#define    ID_TAILPINRATIO  122
#define    ID_MIDEDGERATIO  123
#define    ID_ENDPINSIZE    124
#define    ID_MULT          125
#define    ID_ADD           126
#define    ID_NUMTAILS      127

#define ID_FILEDB        130
#define    ID_NAME          131

/* Name Identifier for the Menu Bar Items. */

#define   IDM_STOCK         200
#define   IDM_PRINT         210
#define   IDM_FILE          220
#define   IDM_FILE_OPEN     221
#define   IDM_FILE_SAVE     222
#define   IDM_FILE_DEL      223

/* User Defined Messages */

#define UM_RECALC      (WM_USER+1)
#define UM_UPDATE      (WM_USER+2)

typedef struct _design {
  char name[32];
  float fLength, fThickness, fSlope, fTailPinRatio;
  float fMidEdgeRatio, fEndPinSize;
  int iMultOrAdd, iNumTails, unit;
} DESIGN;

/* global variables */

extern float fLength,fThickness;
extern float fSlope,fTailPinRatio,fMidEdgeRatio,fEndPinSize;
extern int iMultOrAdd,iNumTails;
extern int old_unit;
extern HWND hMain;
