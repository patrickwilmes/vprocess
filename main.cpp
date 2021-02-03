#include <windows.h>
#include <iostream>
#include <cstdio>
#include <tchar.h>
#include <psapi.h>

void printProcessNameAndId(DWORD processId) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

    PROCESS_MEMORY_COUNTERS pmc;

    if (nullptr != hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (K32EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
        }
    }
    if (strcmp(szProcessName, "<unknown>") != 0) {
        std::cout << szProcessName << " (PID: " << processId << ")" << '\n' << std::flush;
        if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) ) {
            printf( "\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );
            printf( "\tPeakWorkingSetSize: 0x%08X\n",
                    pmc.PeakWorkingSetSize );
            printf( "\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize );
            printf( "\tQuotaPeakPagedPoolUsage: 0x%08X\n",
                    pmc.QuotaPeakPagedPoolUsage );
            printf( "\tQuotaPagedPoolUsage: 0x%08X\n",
                    pmc.QuotaPagedPoolUsage );
            printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08X\n",
                    pmc.QuotaPeakNonPagedPoolUsage );
            printf( "\tQuotaNonPagedPoolUsage: 0x%08X\n",
                    pmc.QuotaNonPagedPoolUsage );
            printf( "\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage );
            printf( "\tPeakPagefileUsage: 0x%08X\n",
                    pmc.PeakPagefileUsage );
        }
    }
    CloseHandle(hProcess);
}

int main() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            printProcessNameAndId(aProcesses[i]);
        }
    }

    return 0;
}
