#ifdef __unix__
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#elif defined(_WIN32) || defined(WIN32)
#include <iostream>
#include <string>
#include <sys/types.h>
#include <process.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#define OS_Windows
#endif
#ifdef OS_Windows
/* Running on Windows */
void CreateProcess(char* cmdArgs)
{
STARTUPINFO si;
PROCESS_INFORMATION pi;
ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));
std::cout << cmdArgs << std::endl;
if (!CreateProcess(
NULL, // No module name (use command line)
cmdArgs, // Command line
NULL, // Process handle not inheritable
NULL, // Thread handle not inheritable
FALSE, // Set handle inheritance to FALSE
CREATE_NEW_CONSOLE, // Create new console flag
NULL, // Use parent's environment block
NULL, // Use parent's starting directory
&si, // Pointer to STARTUPINFO structure
&pi) // Pointer to PROCESS_INFORMATION structure
)
{
std::cout << "CreateProcess failed: " << GetLastError() << std::endl;
return;
}
// Wait until child process exits.
WaitForSingleObject(pi.hProcess, INFINITE);
// Close process and thread handles.
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
}
int main(int argc, char* argv[])
{
while ((++argv)[0])
{
if (argv[0][0] == '-')
{
switch (argv[0][1]) {
case '1':
//Wait for 1 sec.
std::cout << "Process 1 with PID: " << GetCurrentProcessId() << " waiting for 1 seconds." << std::endl;
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
return 0;
case '2':
//Wait for 2 sec.
std::cout << "Process 2 with PID: " << GetCurrentProcessId() << " waiting for 2 seconds." << std::endl;
std::this_thread::sleep_for(std::chrono::milliseconds(2000));
return 0;
case '3':
//Wait for 3 sec.
std::cout << "Process 3 with PID: " << GetCurrentProcessId() << " waiting for 3 seconds." << std::endl;
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
return 0;
default:
std::cout << "Unknown option -" << argv[0][1] << std::endl;
break;
}
}
}
//Get path to app;
char pathBuffer[MAX_PATH];
GetModuleFileName(NULL, pathBuffer, MAX_PATH);
//Concatenate arg and the path to app;
char arg[] = " -1";
strncat(pathBuffer, arg, MAX_PATH - strlen(pathBuffer) - 1);
std::cout << "Wait for apps to close..." << std::endl;
for (int i = 1; i <= 3; i++)
{
//Call function to create process.
CreateProcess(pathBuffer);
//Increment the last char, arg + 1 => next arg.
pathBuffer[strlen(pathBuffer) - 1]++;
}
std::cout << "Done." << std::endl;
}
#else
/* Running on GNU/Linux */
int main(int argc, char* argv[])
{
pid_t child_pID, wpid;
int status = 0;
std::cout << "Waiting for child processes..." << std::endl;
//Create 3 child processes
for (int childP = 0; childP < 3; childP++)
{
child_pID = fork();
//pid = 0 => child executed code
if (child_pID == 0)
{
std::cout << "Process " << childP + 1 << ": " << getpid() << " waiting for " << childP + 1 << " sec." << std::endl;
sleep(childP + 1); //Sleep for sec.
exit(0); //We make sure the child won't escape this block
}
//pid < 0 => failed fork
else if (child_pID < 0)
{
std::cout << "Failed to fork" << std::endl;
}
//pid > 0 => parent executed code.
else if (child_pID > 0)
{
//Code for parent process.
}
}
//Parent process waits for all the child processes
while ((wpid = wait(&status)) > 0);
std::cout << "Done." << std::endl;
exit(0);
}
#endif
