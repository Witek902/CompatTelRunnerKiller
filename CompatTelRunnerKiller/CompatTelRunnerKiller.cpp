#include <Windows.h>
#include <Psapi.h>

constexpr UINT maxProcesses = 4096;
constexpr UINT interval = 4000;

void Run()
{
    // Get the list of process identifiers.
    DWORD processes[maxProcesses], bytesReturned;
    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned))
    {
        return;
    }

    const DWORD numProcesses = bytesReturned / sizeof(DWORD);
    for (DWORD i = 0; i < numProcesses; ++i)
    {
        const HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processes[i]);

        // Get the process name.
        if (NULL != hProcess)
        {
            HMODULE module;
            DWORD cbNeeded;
            if (EnumProcessModules(hProcess, &module, sizeof(module), &cbNeeded))
            {
                TCHAR processName[MAX_PATH];
                GetModuleBaseName(hProcess, module, processName, sizeof(processName) / sizeof(TCHAR));

                if (0 == _wcsicmp(processName, L"CompatTelRunner.exe"))
                {
                    TerminateProcess(hProcess, 0);
                }
            }
        }

        CloseHandle(hProcess);
    }
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    for (;;)
    {
        Run();
        Sleep(interval);
    }

    return 0;
}
