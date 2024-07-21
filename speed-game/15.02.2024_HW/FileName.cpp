#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <string>
#include "resource.h"
#include <commctrl.h>


int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);

void insertionSort(int arr[], int n);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

HMENU hMenu;
HWND hProgress1;
HWND hSpin;
int countSecond;
HWND hButtons[16];
int numbers2[16];
int numberInArray = 0;
int game = 0;

int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{
		//header
		TCHAR title[] = _T("Розпочнемо гру");
		SetWindowText(hWnd, title);

		//progress control
		srand(time(0));
		hProgress1 = GetDlgItem(hWnd, IDC_PROGRESS1);
		SendMessage(hProgress1, PBM_SETRANGE, 0, MAKELPARAM(0, 2));
		SendMessage(hProgress1, PBM_SETSTEP, 1, 0);
		SendMessage(hProgress1, PBM_SETPOS, 0, 0);
		SendMessage(hProgress1, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
		SendMessage(hProgress1, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0)));

		//menu
		hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hWnd, hMenu);

		//spin
		hSpin = GetDlgItem(hWnd, IDC_SPIN1);
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
		SendMessage(hSpin, UDM_SETRANGE32, 0, 100);
		SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);
		SetWindowText(hEdit, TEXT("30"));

		for (int i = 0; i < 16; ++i) {
			hButtons[i] = GetDlgItem(hWnd, IDC_BUTTON1 + i);
		}

		for (int i = 0; i < 16; ++i) {
			EnableWindow(hButtons[i], FALSE);
		}

		for (int i = 0; i < 16; ++i) {
			HWND hButton = GetDlgItem(hWnd, IDC_BUTTON1 + i);
			HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
			SendMessage(hButton, WM_SETFONT, WPARAM(hFont), TRUE);
		}

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wp))
		{
		case IDC_BUTTON1:
		case IDC_BUTTON2:
		case IDC_BUTTON3:
		case IDC_BUTTON4:
		case IDC_BUTTON5:
		case IDC_BUTTON6:
		case IDC_BUTTON7:
		case IDC_BUTTON8:
		case IDC_BUTTON9:
		case IDC_BUTTON10:
		case IDC_BUTTON11:
		case IDC_BUTTON12:
		case IDC_BUTTON13:
		case IDC_BUTTON14:
		case IDC_BUTTON15:
		case IDC_BUTTON16:
		{
			int controlId = LOWORD(wp);

			int buttonIndex = controlId - IDC_BUTTON1;
			int buttonValue;
			TCHAR buttonText[5];
			GetWindowText(hButtons[buttonIndex], buttonText, 5);
			buttonValue = _ttoi(buttonText);

			if (numbers2[numberInArray] == buttonValue) {
				HWND hList = GetDlgItem(hWnd, IDC_LIST1);
				TCHAR valueString[5];
				_stprintf_s(valueString, _T("%d"), buttonValue);
				SendMessage(hList, LB_ADDSTRING, 0, LPARAM(valueString));

				EnableWindow(hButtons[buttonIndex], FALSE);
				numberInArray++;

				if (numberInArray == 16) {
					KillTimer(hWnd, 1);
					SendMessage(hProgress1, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
					SendMessage(hProgress1, PBM_SETPOS, 0, 0);
					TCHAR title[] = _T("Гра завершилася");
					SetWindowText(hWnd, title);

					for (int i = 0; i < 16; ++i) {
						EnableWindow(hButtons[i], TRUE);
					}
					SendMessage(hList, LB_RESETCONTENT, 0, 0);

					MessageBox(0, L"Ти переміг", L":)", MB_OK);

					numberInArray = 0;
				}
			}
		}
		break;

		case IDC_BUTTON17:
		case ID_NewGame:
		{
			HWND hList = GetDlgItem(hWnd, IDC_LIST1);
			KillTimer(hWnd, 1);
			SendMessage(hProgress1, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
			SendMessage(hProgress1, PBM_SETPOS, 0, 0);
			TCHAR title[] = _T("Гра завершилася");
			SetWindowText(hWnd, title);
			for (int i = 0; i < 16; ++i) {
				EnableWindow(hButtons[i], TRUE);
			}
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			numberInArray = 0;

			if (game == 0) {
				for (int i = 0; i < 16; ++i) {
					EnableWindow(hButtons[i], TRUE);
				}
				game = 1;
			}
			HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
			int textLength = GetWindowTextLength(hEdit);
			TCHAR* buffer = new TCHAR[textLength + 1];
			GetWindowText(hEdit, buffer, textLength + 1);
			countSecond = _ttoi(buffer);
			delete[] buffer;

			SendMessage(hProgress1, PBM_SETRANGE, 0, MAKELPARAM(0, countSecond));
			
			const int amount = 16;
			int numbers1[amount];
			for (int i = 0; i < amount; ++i) {
				numbers2[i] = numbers1[i] = rand() % 101;
			}

			for (int i = 0; i < amount; ++i) {
				TCHAR buttonText[5];
				_stprintf_s(buttonText, _T("%d"), numbers1[i]);
				hButtons[i] = GetDlgItem(hWnd, IDC_BUTTON1 + i);
				SetWindowText(hButtons[i], buttonText);
			}

			insertionSort(numbers2, amount);

			SetTimer(hWnd, 1, 1000, NULL);
		}
		break;
		case ID_Exit:
			EndDialog(hWnd, 0);
			break;
		}
		break;
	}
	case WM_TIMER:
	{
		int nPos = SendMessage(hProgress1, PBM_GETPOS, 0, 0);
		if (nPos < countSecond)
		{
			SendMessage(hProgress1, PBM_STEPIT, 0, 0);

			TCHAR title[4];
			_stprintf_s(title, _T("%d"), countSecond - nPos - 1);
			SetWindowText(hWnd, title);
		}
		else
		{
			KillTimer(hWnd, 1);
			SendMessage(hProgress1, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
			SendMessage(hProgress1, PBM_SETPOS, 0, 0);
			TCHAR title[] = _T("Гра завершилася");
			SetWindowText(hWnd, title);
			countSecond = -1;
		}

		if (countSecond == -1) {

			for (int i = 0; i < 16; ++i) {
				EnableWindow(hButtons[i], TRUE);
			}
			HWND hList = GetDlgItem(hWnd, IDC_LIST1);
			SendMessage(hList, LB_RESETCONTENT, 0, 0);

			MessageBox(0, L"Ти програв", L":(", MB_OK);

			numberInArray = 0;
		}

		break;
	}
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}

void insertionSort(int arr[], int n) {
	for (int i = 1; i < n; ++i) {
		int key = arr[i];
		int j = i - 1;

		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}