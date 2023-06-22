#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <algorithm>
#include <tchar.h>

// Declare functions
int processFinder(const std::string &proc);
int StringCompare(std::string, std::string);
int createProc(TCHAR*);


// Main
int main( int argc, char* argv[] )
{
    if (argc != 2) 
    {
        std::cerr << "[-] SYNTAX ERROR: " << argv[0] << " <PROCESS NAME>" << std::endl;

        return 1;
    }

    int pid = processFinder(argv[1]);

    if (pid == 1)
    {
        pid = createProc(argv[1]);
    }

    std::cout << "[+] PROCESS ID: " << pid << std::endl;
    
    return 0;
}


// Functions
int processFinder(const std::string &proc)
{
    /*Describes an entry from a list of the processes residing in the system address space when a snapshot was taken
    typedef struct tagPROCESSENTRY32 {
    DWORD     dwSize;
    DWORD     cntUsage;
    DWORD     th32ProcessID;
    ULONG_PTR th32DefaultHeapID;
    DWORD     th32ModuleID;
    DWORD     cntThreads;
    DWORD     th32ParentProcessID;
    LONG      pcPriClassBase;
    DWORD     dwFlags;
    CHAR      szExeFile[MAX_PATH];
    } PROCESSENTRY32;
    */ 
    
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    // Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes
    /*
    HANDLE CreateToolhelp32Snapshot(
    [in] DWORD dwFlags,
    [in] DWORD th32ProcessID);
    */
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // First process (PID = 0) is System Idle (not a usable process, but still enumerable)
    if (!Process32First(snapshot, &entry))
    {
        std::cerr << "[-] PROCESS SNAPSHOT ERROR!" << std::endl;

        exit(0);
    }

    int found = FALSE;

    // Retrieves information about the next process recorded in a system snapshot (sends info to entry)
    while (Process32Next(snapshot, &entry))
    {
        // Compare the current process name with the entered process name
        if (StringCompare(entry.szExeFile, proc))
        {  
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

            DWORD processID = entry.th32ProcessID;

            CloseHandle(hProcess);

            found = TRUE;

            return processID;
        }
    }
    
    CloseHandle(snapshot);
    
    // Set condition for process not being found
    if (!found) 
    { 
        std::cerr << "[-] PROCESS NOT FOUND!" << std::endl; 
        return 1;
    }
}

// Convert strings to lower case for comparison
BOOL StringCompare(std::string s1, std::string s2)
{
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

    if(s1.compare(s2) == 0) {return TRUE;}

    return FALSE;
}


int createProc(TCHAR *proc)     
{
    /*
    BOOL CreateProcess(
            [in, optional]      LPCSTR                lpApplicationName,
            [in, out, optional] LPSTR                 lpCommandLine,
            [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
            [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
            [in]                BOOL                  bInheritHandles,
            [in]                DWORD                 dwCreationFlags,
            [in, optional]      LPVOID                lpEnvironment,
            [in, optional]      LPCSTR                lpCurrentDirectory,
            [in]                LPSTARTUPINFOA        lpStartupInfo,
            [out]               LPPROCESS_INFORMATION lpProcessInformation
            );
*/

    STARTUPINFO si;              
    PROCESS_INFORMATION pi;

    LPTSTR szCmdLine = _tcsdup(TEXT(proc));

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(                
        NULL,  
        szCmdLine,
        NULL,
        NULL,
        FALSE,
        BELOW_NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    )) 
    {
        std::cout << "[-] FAILED TO CREATE PROCESS!" << std::endl;
        exit(0);
    }

    std::cout << "[+] CREATING PROCESS..." << std::endl;

    return pi.dwProcessId;
}

