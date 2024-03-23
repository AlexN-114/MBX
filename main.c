#include <windows.h>
#include <stdlib.h>

typedef struct 
{
    int zeit;
}ThrdParam;
ThrdParam thrdParam;

DWORD ThrdID;

char titel[100] = "Mitteilung";
char nachricht[400] = "keine Nachricht";

int GetString(LPSTR src, LPSTR dst, int size)
{
    int cnt = 0;

    while (cnt < size)
    {
        dst[cnt] = src[cnt];
        if ( 0  == src[cnt]) break;
        if ('"' == src[cnt]) break;
        cnt++;
    }
    dst[cnt] = 0;
    
    return cnt;
}

void GetParams(LPSTR lpCmdLine)
{
    char *pos;
    int found = 0;
    
    if ((0 != (pos = strstr(lpCmdLine,"/t\""))) ||
        (0 != (pos = strstr(lpCmdLine,"/T\""))) ||
        (0 != (pos = strstr(lpCmdLine,"-t\""))) ||
        (0 != (pos = strstr(lpCmdLine,"-T\""))))
    {
        GetString(pos+3,titel,sizeof(titel));
        found = 1;
    }

    if ((0 != (pos = strstr(lpCmdLine,"/n\""))) ||
        (0 != (pos = strstr(lpCmdLine,"/N\""))) ||
        (0 != (pos = strstr(lpCmdLine,"/m\""))) ||
        (0 != (pos = strstr(lpCmdLine,"/M\""))) ||
        (0 != (pos = strstr(lpCmdLine,"-n\""))) ||
        (0 != (pos = strstr(lpCmdLine,"-N\""))) ||
        (0 != (pos = strstr(lpCmdLine,"-m\""))) ||
        (0 != (pos = strstr(lpCmdLine,"-M\""))))
    {
        GetString(pos+3,nachricht,sizeof(nachricht));
        found = 1;
    }
    
    if((0 != (pos = strstr(lpCmdLine,"/z"))) ||
       (0 != (pos = strstr(lpCmdLine,"/Z"))) ||
       (0 != (pos = strstr(lpCmdLine,"-z"))) ||
       (0 != (pos = strstr(lpCmdLine,"-Z"))) )
    {
        thrdParam.zeit = atoi(pos+2);
    }
    
    if (!found)
    {
        strcpy_s(nachricht,sizeof(nachricht),lpCmdLine);
    }
}

DWORD WINAPI EndIn(void* pParam)
{
    ThrdParam *tp = (ThrdParam *)pParam;
    
    Sleep(1000*tp->zeit);
    
    exit (0);
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */
    
    GetParams(lpCmdLine);
    
    if (0 < thrdParam.zeit)
    {
        CreateThread(NULL,0,EndIn,(void*)&thrdParam,0,&ThrdID);        
    }

    MessageBox(NULL,nachricht,titel,MB_OK);
	return 0;
}
