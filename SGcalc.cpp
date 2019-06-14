#ifndef UNICODE
#define UNICODE
#endif 

#include "Calc2.h"
#include "stdafx.h"
#include "resource.h"
#include <windows.h>
#include <cstdlib>
#include <string>
#include <iostream>

enum mathOperators { null, add, subtract, multiply, divide, equals };
HWND editCtrl;

void getResolution(unsigned short &horizontal, unsigned short &vertical);
void addButtonControl(HWND);
void addEditControl(HWND);
void wmCommand(WPARAM);
void wmKDown(WPARAM);
char getNewChar(WPARAM);
bool addToStr(char newChar, wchar_t* displayStr, short strSize = 25);
double doMath(char, char*, double, mathOperators&, short = 25);
void setMathOperator(char, mathOperators&);
bool checkDecimal(wchar_t*, short);
void clearStr(char*, short);
void clearStr(wchar_t*, short);
void wStrToWchart(std::wstring, wchar_t*, short);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc{};
	//wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowProc;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.
	unsigned short hStart;//Horizontal Starting point
	unsigned short vStart;//Vertical starting point
	getResolution(hStart, vStart);
	hStart = hStart / 2.8;
	vStart = vStart / 4.3;
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles
		CLASS_NAME,                     // Window class
		L"",                    // Window text (Changed from "Learn to Program  Windows")
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX|WS_VISIBLE,            // Window style

		// Size and position
		hStart, vStart, 210, 278,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);



		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_ACTIVECAPTION + 1));

		EndPaint(hwnd, &ps);
	} return 0;

	case WM_COMMAND: {
		if (wParam == 1 ||
			wParam == 2 ||
			wParam == 3 ||
			wParam == 4 ||
			wParam == 5 ||
			wParam == 6 ||
			wParam == 7 ||
			wParam == 8 ||
			wParam == 9 ||
			wParam == 0 ||
			wParam == '+' ||
			wParam == '-' ||
			wParam == '*' ||
			wParam == '/' ||
			wParam == '=' ||
			wParam == '.' ||
			wParam == 'c' ||
			wParam == '<'
			) {
			wmCommand(wParam);
		}

	} return 0;

	case WM_KEYDOWN: wmKDown(wParam); return 0;


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE: {
		addButtonControl(hwnd);
		addEditControl(hwnd);
	} return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void getResolution(unsigned short &horizontal, unsigned short &vertical) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void addButtonControl(HWND hwnd) {
	struct handleID {
		const short button0{ 0 };
		const short button1{ 1 };
		const short button2{ 2 };
		const short button3{ 3 };
		const short button4{ 4 };
		const short button5{ 5 };
		const short button6{ 6 };
		const short button7{ 7 };
		const short button8{ 8 };
		const short button9{ 9 };
		const char buttonPlus{ '+' };
		const char buttonMinus{ '-' };
		const char buttonDivide{ '/' };
		const char buttonMultiply{ '*' };
		const char buttonEquals{ '=' };
		const char buttonDot{ '.' };
		const char buttonClr{ 'c' };//clear
		const char buttonBck{ '<' };
	};

	handleID buttonIDs;

	CreateWindowEx(0, L"Button", L"C", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1, 1, 35, 35, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonClr), NULL, NULL);
	CreateWindowEx(0, L"Button", L"<-", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 36, 1, 35, 35, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonBck), NULL, NULL);
	CreateWindowEx(0, L"Button", L"7", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1, 46, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button7), NULL, NULL);
	CreateWindowEx(0, L"Button", L"8", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 46, 46, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button8), NULL, NULL);
	CreateWindowEx(0, L"Button", L"9", BS_FLAT | WS_CHILD | WS_VISIBLE, 90, 46, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button9), NULL, NULL);
	CreateWindowEx(0, L"Button", L"4", BS_FLAT | WS_CHILD | WS_VISIBLE, 1, 91, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button4), NULL, NULL);
	CreateWindowEx(0, L"Button", L"5", BS_FLAT | WS_CHILD | WS_VISIBLE, 46, 91, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button5), NULL, NULL);
	CreateWindowEx(0, L"Button", L"6", BS_FLAT | WS_CHILD | WS_VISIBLE, 90, 91, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button6), NULL, NULL);
	CreateWindowEx(0, L"Button", L"1", BS_FLAT | WS_CHILD | WS_VISIBLE, 1, 136, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button1), NULL, NULL);
	CreateWindowEx(0, L"Button", L"2", BS_FLAT | WS_CHILD | WS_VISIBLE, 46, 136, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button2), NULL, NULL);
	CreateWindowEx(0, L"Button", L"3", BS_FLAT | WS_CHILD | WS_VISIBLE, 90, 136, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button3), NULL, NULL);
	CreateWindowEx(0, L"Button", L".", BS_FLAT | WS_CHILD | WS_VISIBLE, 1, 181, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonDot), NULL, NULL);
	CreateWindowEx(0, L"Button", L"0", BS_FLAT | WS_CHILD | WS_VISIBLE, 46, 181, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.button0), NULL, NULL);
	CreateWindowEx(0, L"Button", L"=", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 90, 181, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonEquals), NULL, NULL);
	CreateWindowEx(0, L"Button", L"/", BS_FLAT | WS_CHILD | WS_VISIBLE, 134, 46, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonDivide), NULL, NULL);
	CreateWindowEx(0, L"Button", L"x", BS_FLAT | WS_CHILD | WS_VISIBLE, 134, 91, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonMultiply), NULL, NULL);
	CreateWindowEx(0, L"Button", L"-", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 134, 136, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonMinus), NULL, NULL);
	CreateWindowEx(0, L"Button", L"+", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 134, 181, 40, 40, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonPlus), NULL, NULL);
}
void addEditControl(HWND hwnd) {
	wchar_t text[]{ L"0" };
	editCtrl = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", text,
		WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		73, 1, 120, 35, hwnd, NULL, NULL, NULL);
	//CreateWindowEx(0, L"Static", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 1, 1, 280, 55, hwnd, NULL, NULL, NULL);
}

void wmCommand(WPARAM wParam) {
	static wchar_t displayStr[25]{};
	static char mathStr[25]{};
	static double sum{ 0.00 };
	static mathOperators mOperator{ null };
	std::wstring stringBuffer{};

	char newChar = getNewChar(wParam);
	if (newChar != '<' || mOperator != equals) {
		sum = doMath(newChar, mathStr, sum, mOperator);
		if (!addToStr(newChar, displayStr)) {
			clearStr(displayStr, 25);
			stringBuffer = std::to_wstring(sum);
			wStrToWchart(stringBuffer, displayStr, 25);
			//_itow_s(sum, displayStr, 10);
		}
		SetWindowTextW(editCtrl, displayStr);
		if (newChar == '=' || newChar == 'c' || (newChar == '<'&&displayStr[0] == '0')) { clearStr(displayStr, 25); }
	}
}

void wmKDown(WPARAM wParam) {
	WPARAM vk{};
	switch (wParam) {
	case VK_NUMPAD0: vk = 0; wmCommand(vk); break;
	case VK_NUMPAD1: vk = 1; wmCommand(vk); break;
	case VK_NUMPAD2: vk = 2; wmCommand(vk); break;
	case VK_NUMPAD3: vk = 3; wmCommand(vk); break;
	case VK_NUMPAD4: vk = 4; wmCommand(vk); break;
	case VK_NUMPAD5: vk = 5; wmCommand(vk); break;
	case VK_NUMPAD6: vk = 6; wmCommand(vk); break;
	case VK_NUMPAD7: vk = 7; wmCommand(vk); break;
	case VK_NUMPAD8: vk = 8; wmCommand(vk); break;
	case VK_NUMPAD9: vk = 9; wmCommand(vk); break;
	case 0x30: vk = 0; wmCommand(vk); break;
	case 0x31: vk = 1; wmCommand(vk); break;
	case 0x32: vk = 2; wmCommand(vk); break;
	case 0x33: vk = 3; wmCommand(vk); break;
	case 0x34: vk = 4; wmCommand(vk); break;
	case 0x35: vk = 5; wmCommand(vk); break;
	case 0x36: vk = 6; wmCommand(vk); break;
	case 0x37: vk = 7; wmCommand(vk); break;
	case 0x38: vk = 8; wmCommand(vk); break;
	case 0x39: vk = 9; wmCommand(vk); break;
	case VK_ADD: vk = '+'; wmCommand(vk); break;
	case VK_OEM_PLUS: vk = '+'; wmCommand(vk); break;
	case VK_SUBTRACT: vk = '-'; wmCommand(vk); break;
	case VK_OEM_MINUS: vk = '-'; wmCommand(vk); break;
	case VK_MULTIPLY: vk = '*'; wmCommand(vk); break;
	case VK_DIVIDE: vk = '/'; wmCommand(vk); break;
	case VK_DECIMAL: vk = '.'; wmCommand(vk); break;
	case VK_DELETE: vk = 'c'; wmCommand(vk); break;
	case VK_RETURN: vk = '='; wmCommand(vk); break;
	case VK_BACK: vk = '<'; wmCommand(vk); break;
	}
}

char getNewChar(WPARAM newChar) {
	switch (newChar) {
	case 1:return '1';
	case 2:return '2';
	case 3:return '3';
	case 4:return '4';
	case 5:return '5';
	case 6:return '6';
	case 7:return '7';
	case 8:return '8';
	case 9:return '9';
	case 0:return '0';
	case '+':return '+';
	case '-':return '-';
	case '/':return '/';
	case '*':return '*';
	case '=':return '=';
	case '.':return '.';
	case 'c':return 'c';
	case '<':return '<';
	}
	return '\0';
}

bool addToStr(char newChar, wchar_t* displayStr,short strSize) {
	if (newChar == '=') {
		return 0;
	}
	else if (newChar == '+' || newChar == '-' || newChar == '*' || newChar == '/') {
		for (short c = 0; c < strSize; c++) {

			if (displayStr[c] == '\0') { displayStr[c] = newChar; return 1; }
		}
	}
	else if (newChar == 'c') {
		clearStr(displayStr, strSize);
		displayStr[0] = '0';
		return 1;
	}
	else if (newChar == '<') {
		for (short c = strSize; c >= 0; c--) {
			if (displayStr[c] != '\0') {
				if (displayStr[c] != '+'&&
					displayStr[c] != '-'&&
					displayStr[c] != '*'&&
					displayStr[c] != '/'&&
					displayStr[c] != '='
					) {
					displayStr[c] = '\0';
					if (displayStr[0] == '\0') { displayStr[0] = '0'; }
					break;
				}
				else { break; }
			}
		}		
		return 1;
	}
	else {
		for (short c = 0; c < strSize; c++) {
			if (displayStr[c] == '\0') { displayStr[c] = newChar; return 1; }
		}
		return 1;
	}
}

double doMath(char newChar, char* mathStr, double sum, mathOperators& mOperator,short strSize ) {
	static double numRight{ 0.00 };
	//static mathOperators mOperator{ null };

	if (newChar == '+' || newChar == '-' || newChar == '*' || newChar == '/') {
		if (mOperator == null|| mOperator == equals) {
			numRight = atof(mathStr);
			clearStr(mathStr, strSize);
			sum = numRight;
			numRight = 0.00;
			setMathOperator(newChar, mOperator);
			return sum;
		}
		else {
			numRight = atof(mathStr);
			clearStr(mathStr, strSize);
			switch (mOperator) {
			case add: sum = sum + numRight; break;
			case subtract: sum = sum - numRight; break;
			case multiply: sum = sum * numRight; break;
			case divide: sum = sum / numRight; break;
			}
			numRight = 0.00;
			setMathOperator(newChar, mOperator);
			return sum;
		}
	}
	else if (newChar == '=') {
		numRight = atof(mathStr);
		clearStr(mathStr, strSize);
		switch (mOperator) {
		case add: sum = sum + numRight; break;
		case subtract: sum = sum - numRight; break;
		case multiply: sum = sum * numRight; break;
		case divide: sum = sum / numRight; break;
		}
		numRight = 0.00;
		setMathOperator(newChar, mOperator);
		return sum;
	}
	else if (newChar == 'c') {
		clearStr(mathStr, strSize);
		sum = 0.00;
		numRight = 0.00;
		return sum;
	}
	else if (newChar == '<') {
		for (short c = strSize; c >= 0; c--) {
			if (mathStr[c] != '\0') {
				if (mathStr[c] != '+'&&
					mathStr[c] != '-'&&
					mathStr[c] != '*'&&
					mathStr[c] != '/'&&
					mathStr[c] != '='
					) {
					mathStr[c] = '\0'; break;
				}
				else { break; }
			}
		}
		return sum;
	}
	else {
		if (mOperator == equals) { mOperator = null; }
		for (short c = 0; c < strSize; c++) {
			if (mathStr[c] == '\0') { mathStr[c] = newChar; return sum; }
		}
		return sum;
	}
}

void setMathOperator(char newChar, mathOperators& mOperator) {
	switch (newChar) {
	case '+':mOperator = add; break;
	case '-':mOperator = subtract; break;
	case '*':mOperator = multiply; break;
	case '/':mOperator = divide; break;
	case '=':mOperator = equals; break;
	}
}

bool checkDecimal(wchar_t* displayStr, short length) {
	for (short c = length - 1; c >= 0; c--) {
		if (displayStr[c] == '.') { return 1; }
	}
	return 0;
}

void clearStr(char* str, short strSize) {
	for (short c = 0; c < strSize; c++) { str[c] = '\0'; }
}
void clearStr(wchar_t* str, short strSize) {
	for (short c = 0; c < strSize; c++) { str[c] = '\0'; }
}

void wStrToWchart(std::wstring str, wchar_t* charArray, short charArraySize) {
	for (short c = 0; c < str.length(); c++) {
		charArray[c] = str.at(c);
	}
	short d{ 0 };
	if (checkDecimal(charArray, charArraySize)) {
		for (short c = charArraySize - 1; c >= 0; c--) {
			if (charArray[c] == '.') d = c;
		}
		for (short c1 = charArraySize - 1; c1 >= d; c1--) {
			if (charArray[c1] != '0'&&charArray[c1] != '\0'&&charArray[c1] != '.') break;
			else charArray[c1] = '\0';
		}
	}
}