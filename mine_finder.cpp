#include <stdio.h>
#include <Windows.h>
int main()
{
	HWND hWinmine = FindWindowA(NULL, "扫雷");
	DWORD dwPid = 0;
	GetWindowThreadProcessId(hWinmine, &dwPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	PBYTE pByte = NULL;
	DWORD dwHight = 0, dwWidth = 0;
	DWORD dwAddr = 0x01005330;
	DWORD dwNum = 0;
	DWORD dwRead = 0;
	ReadProcessMemory(hProcess, (LPVOID)(dwAddr), &dwNum, sizeof(DWORD), &dwRead);
	ReadProcessMemory(hProcess, (LPVOID)(dwAddr + 4), &dwWidth, sizeof(DWORD), &dwRead);
	ReadProcessMemory(hProcess, (LPVOID)(dwAddr + 8), &dwHight, sizeof(DWORD), &dwRead);
	if (dwWidth != 30 || dwHight != 16)
	{
		return 0;
	}
	DWORD dwBoomAddr = 0x01005340;
	DWORD dwSize = dwWidth * dwHight + dwWidth * 2 + dwHight * 2 + 4;
	pByte = (PBYTE)malloc(dwSize);
	ReadProcessMemory(hProcess, (LPVOID)dwBoomAddr, pByte, dwSize, &dwRead);
	BYTE bClear = 0x8E;
	int i = 0;
	int n = dwNum;
	while (i < dwSize)
	{
		if (pByte[i] == 0x8F)//0x8F代表的是雷
		{
			DWORD dwAddr1 = 0x01005340 + i;
			WriteProcessMemory(hProcess, (LPVOID)dwAddr1, &bClear, sizeof(BYTE), &dwRead);
			n--;
		}
		i++;
	}
	RECT rt;
	GetClientRect(hWinmine, &rt);
	InvalidateRect(hWinmine, &rt, TRUE);
	free(pByte);
	printf("%d \r\n", n);
	CloseHandle(hProcess);
	getchar();
	return 0;
}
