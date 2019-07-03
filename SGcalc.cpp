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
#include <fstream>

enum mathOperators { null, add, subtract, multiply, divide, equals };
HWND editCtrl;
struct controls {
	HWND button0;
	HWND button1;
	HWND button2;
	HWND button3;
	HWND button4;
	HWND button5;
	HWND button6;
	HWND button7;
	HWND button8;
	HWND button9;
	HWND buttonPlus;
	HWND buttonMinus;
	HWND buttonDivide;
	HWND buttonMultiply;
	HWND buttonEquals;
	HWND buttonDot;
	HWND buttonClr;
	HWND buttonBck;
};
controls control;

void getResolution(unsigned short &horizontal, unsigned short &vertical);
void addButtonControl(HWND, RECT&);
void addEditControl(HWND,RECT&);
void moveControls(RECT&);
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
//void writeFile(RECT&,bool);

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
		WS_CAPTION | WS_SYSMENU |WS_SIZEBOX| WS_MINIMIZEBOX | WS_MAXIMIZEBOX|WS_VISIBLE,            // Window style

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

	//bool startAppend{ 0 };
	RECT mainWindow;
	GetClientRect(hwnd, &mainWindow);
	//writeFile(mainWindow,startAppend);

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



		FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_ACTIVECAPTION + 1));

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
	
	case WM_SIZING: {
		//bool startAppend{ 1 };
		RECT mainWindow;
		GetClientRect(hwnd, &mainWindow);
		moveControls(mainWindow);
		//writeFile(mainWindow,startAppend);
	} return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE: {
		RECT mainWindow;
		GetClientRect(hwnd, &mainWindow);
		addButtonControl(hwnd,mainWindow);
		addEditControl(hwnd,mainWindow);
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

void addButtonControl(HWND hwnd, RECT& mainWindow) {
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
	double firstX{ 1.0 };
	double firstY{ 2.0 };
	double X{ (mainWindow.right / 5.5) };
	double Y{ (mainWindow.bottom / 6.7) };
	double Y2{ mainWindow.bottom / 6.0 };
	double X2{ mainWindow.right / 4.85 };
	double xratio{};

	control.buttonClr = CreateWindowEx(0, L"Button", L"C", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1, 1, X, Y, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonClr), NULL, NULL);
	control.buttonBck = CreateWindowEx(0, L"Button", L"<-", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (X+1), 1, X, Y, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonBck), NULL, NULL);
	control.button7 = CreateWindowEx(0, L"Button", L"7", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1, (Y+11), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button7), NULL, NULL);
	control.button8 = CreateWindowEx(0, L"Button", L"8", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (X2+8), (Y+11), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button8), NULL, NULL);
	control.button9 = CreateWindowEx(0, L"Button", L"9", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2+X2+16), (Y+11), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button9), NULL, NULL);
	control.button4 = CreateWindowEx(0, L"Button", L"4", BS_FLAT | WS_CHILD | WS_VISIBLE, 1, (Y+Y2+16), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button4), NULL, NULL);
	control.button5 = CreateWindowEx(0, L"Button", L"5", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + 8), (Y + Y2 + 16), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button5), NULL, NULL);
	control.button6 = CreateWindowEx(0, L"Button", L"6", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + X2 + 16), (Y + Y2 + 16), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button6), NULL, NULL);
	control.button1 = CreateWindowEx(0, L"Button", L"1", BS_FLAT | WS_CHILD | WS_VISIBLE, 1, (Y + Y2 + Y2 + 21), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button1), NULL, NULL);
	control.button2 = CreateWindowEx(0, L"Button", L"2", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + 8), (Y + Y2 + Y2 + 21), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button2), NULL, NULL);
	control.button3 = CreateWindowEx(0, L"Button", L"3", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + X2 + 16), (Y + Y2 + Y2 + 21), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button3), NULL, NULL);
	control.buttonDot = CreateWindowEx(0, L"Button", L".", BS_FLAT | WS_CHILD | WS_VISIBLE, 1, (Y + Y2 + Y2 + Y2 + 26), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonDot), NULL, NULL);
	control.button0 = CreateWindowEx(0, L"Button", L"0", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + 8), (Y + Y2 + Y2 + Y2 + 26), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.button0), NULL, NULL);
	control.buttonEquals = CreateWindowEx(0, L"Button", L"=", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (X2 + X2 + 16), (Y + Y2 + Y2 + Y2 + 26), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonEquals), NULL, NULL);
	control.buttonDivide = CreateWindowEx(0, L"Button", L"/", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + X2 + X2 + 24), (Y+11), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonDivide), NULL, NULL);
	control.buttonMultiply = CreateWindowEx(0, L"Button", L"x", BS_FLAT | WS_CHILD | WS_VISIBLE, (X2 + X2 + X2 + 24), (Y + Y2 + 16), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonMultiply), NULL, NULL);
	control.buttonMinus = CreateWindowEx(0, L"Button", L"-", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (X2 + X2 + X2 + 24), (Y + Y2 + Y2 + 21), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonMinus), NULL, NULL);
	control.buttonPlus = CreateWindowEx(0, L"Button", L"+", BS_FLAT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (X2 + X2 + X2 + 24), (Y + Y2 + Y2 + Y2 + 26), X2, Y2, hwnd, reinterpret_cast<HMENU>(buttonIDs.buttonPlus), NULL, NULL);
}
void addEditControl(HWND hwnd, RECT& mainWindow) {
	double X{ (mainWindow.right / 5.5) };
	double Y{ (mainWindow.bottom / 6.7) };
	double XeditControl{ X * 3.5 };
	wchar_t text[]{ L"0" };
	editCtrl = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", text,
		WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		(X + X + 2), 1, XeditControl, Y, hwnd, NULL, NULL, NULL);
	//CreateWindowEx(0, L"Static", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 1, 1, 280, 55, hwnd, NULL, NULL, NULL);
}

void moveControls(RECT& mainWindow) {
	double X{ (mainWindow.right / 5.5) };
	double Y{ (mainWindow.bottom / 6.7) };
	double Y2{ mainWindow.bottom / 6.0 };
	double X2{ mainWindow.right / 4.85 };
	double XeditControl{ X * 3.5 };

	MoveWindow(control.buttonClr, 1, 1, X, Y, 1);
	MoveWindow(control.buttonBck, (X + 1), 1, X, Y, 1);
	MoveWindow(control.button7, 1, (Y + 11), X2, Y2, 1);
	MoveWindow(control.button8, (X2 + 8), (Y + 11), X2, Y2, 1);
	MoveWindow(control.button9, (X2 + X2 + 16), (Y + 11), X2, Y2, 1);
	MoveWindow(control.button4, 1, (Y + Y2 + 16), X2, Y2, 1);
	MoveWindow(control.button5, (X2 + 8), (Y + Y2 + 16), X2, Y2, 1);
	MoveWindow(control.button6, (X2 + X2 + 16), (Y + Y2 + 16), X2, Y2, 1);
	MoveWindow(control.button1, 1, (Y + Y2 + Y2 + 21), X2, Y2, 1);
	MoveWindow(control.button2, (X2 + 8), (Y + Y2 + Y2 + 21), X2, Y2, 1);
	MoveWindow(control.button3, (X2 + X2 + 16), (Y + Y2 + Y2 + 21), X2, Y2, 1);
	MoveWindow(control.buttonDot, 1, (Y + Y2 + Y2 + Y2 + 26), X2, Y2, 1);
	MoveWindow(control.button0, (X2 + 8), (Y + Y2 + Y2 + Y2 + 26), X2, Y2, 1);
	MoveWindow(control.buttonEquals, (X2 + X2 + 16), (Y + Y2 + Y2 + Y2 + 26), X2, Y2, 1);
	MoveWindow(control.buttonDivide, (X2 + X2 + X2 + 24), (Y+11), X2, Y2, 1);
	MoveWindow(control.buttonMultiply, (X2 + X2 + X2 + 24), (Y + Y2 + 16), X2, Y2, 1);
	MoveWindow(control.buttonMinus, (X2 + X2 + X2 + 24), (Y + Y2 + Y2 + 21), X2, Y2, 1);
	MoveWindow(control.buttonPlus, (X2 + X2 + X2 + 24), (Y + Y2 + Y2 + Y2 + 26), X2, Y2, 1);
	MoveWindow(editCtrl,(X+X+2),1,XeditControl,Y,1);
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

//void writeFile(RECT& rect, bool startAppend) {
//	if (startAppend) {
//		std::string file{ "log.txt" };
//		std::fstream fileStream{ file,std::ios::app };
//		fileStream << rect.left << " ; " << rect.top << " ; " << rect.right << " ; "
//			<< rect.bottom << std::endl;
//		fileStream.close();
//	}
//	else {
//		std::string file{ "log.txt" };
//		std::ofstream fileStream{ file };
//		fileStream << rect.left << " ; " << rect.top << " ; " << rect.right << " ; "
//			<< rect.bottom << std::endl;
//		fileStream.close();
//	}
//}