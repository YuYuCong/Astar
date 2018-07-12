//-----------------------------------------------------------------------------
// DEBUGLOG LIBRARY
// This include file contains functions that write to the External
//	debuglog.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Structs and Globals
//-----------------------------------------------------------------------------
typedef struct tagMYMESSAGE //Declare outgoing data structure
{
   char myString[200];
}  MYMESSAGE;

COPYDATASTRUCT     g_CDS; //Copy Data Struct (CDS)
MYMESSAGE          g_SentString;//outgoing message structure
HWND               g_hwDebuglog=NULL; //debuglog handle


//-----------------------------------------------------------------------------
// Name: ClearLog
// Desc: Clears the External Debuglog.
//-----------------------------------------------------------------------------
int ClearLog(HWND hwnd)
{
	g_hWndGame = hwnd;
    g_hwDebuglog = FindWindow(NULL,"External Debuglog");
	if (g_hwDebuglog == NULL) return -2;

    g_CDS.dwData = 1;  // integer based identifier
    if( g_hwDebuglog != NULL )
    SendMessage(
	    g_hwDebuglog, // handle to destination control
	    WM_COPYDATA, // message ID
		(WPARAM)(HWND) g_hWndGame, //Handle to the window passing the data.
		(LPARAM)(LPVOID) &g_CDS//Pointer to COPYDATASTRUCT structure
		);

	return 1;
}


//-----------------------------------------------------------------------------
// Name: Debug
// Desc: Called by each of the following overloaded functions.
//-----------------------------------------------------------------------------
void Debug (char* pDebuglog)
{
	if (g_hWndGame == NULL) return ;
	if (g_hwDebuglog == NULL) return;
	strcpy (g_SentString.myString,pDebuglog);
	if (strlen(pDebuglog) >= 200) g_SentString.myString [200] = NULL;
	g_CDS.dwData = 0;  // integer based identifier
	g_CDS.cbData = sizeof(g_SentString);  // size of data
	g_CDS.lpData = &g_SentString; // address of sent data structure
	if( g_hwDebuglog != NULL )
		SendMessage(
           g_hwDebuglog, // handle of debuglog
           WM_COPYDATA, // message ID
           (WPARAM)(HWND) g_hWndGame, //Handle to the window passing the data.
           (LPARAM)(LPVOID) &g_CDS);//Pointer to COPYDATASTRUCT structure
}


//-----------------------------------------------------------------------------
// Name: Debuglog (overloaded char* version)
// Desc: Writes a string to the External Debuglog.
//-----------------------------------------------------------------------------
void Debuglog (char* pDebuglog)
{
	Debug (pDebuglog);
}


//-----------------------------------------------------------------------------
// Name: Debuglog (overloaded integer version)
// Desc: Writes a number to the External Debuglog.
//-----------------------------------------------------------------------------
void Debuglog (int number)
{
	char stringNum [33];
	itoa(number,stringNum,10);
	Debug (stringNum);
}


//-----------------------------------------------------------------------------
// Name: Debuglog (overloaded string version)
// Desc: Writes a string to the External Debuglog.
//-----------------------------------------------------------------------------
void Debuglog (CSTR pStringDebuglog)
{
	char* pDebuglog = pStringDebuglog.actualString;
	Debug (pDebuglog);
}
