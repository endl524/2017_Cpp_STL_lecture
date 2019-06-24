#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>


using namespace std;

class Replay {	// ���÷��� �����͸� ���� ��ü
	int time;	// �ð�
	int mx;		// ���콺 x��ǥ
	int my;		// ���콺 y��ǥ
	bool lClick;	// ��Ŭ���� �ߴ���?
	bool rClick;	// ��Ŭ���� �ߴ���?
	bool is_clear;	// clear�� �ߴ���?
public:
	Replay(int t, int x, int y, bool lc, bool rc, bool is_c) : time(t), mx(x), my(y), lClick(lc), rClick(rc), is_clear(is_c) {  };
	int getTime() const { return time; }
	int getX() const { return mx; }
	int getY() const { return my; }
	bool getlClick() const { return lClick; }
	bool getrClick() const { return rClick; }
	bool getIs_clear() const { return is_clear; }
	~Replay() {}
};

//-----���Ϳ� ���ͷ����� ����-----
vector<Replay> v;
vector<Replay>::iterator beg;
vector<Replay> re_v;
vector<Replay>::iterator re_beg;
vector<Replay>::iterator re_end;
//------------------------------

void save() {	// ���̺� �Լ�.
	ofstream save("save.txt", ios::trunc);
	for (beg = v.begin(); beg != v.end(); ++beg) {
		save << beg->getTime();
		save << '\t' << beg->getX();
		save << '\t' << beg->getY();
		save << '\t' << beg->getlClick();
		save << '\t' << beg->getrClick();
		save << '\t' << beg->getIs_clear();
		save << endl;
	}
	save.close();
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR strCmdLine, int nShowCmd) {

	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIconSm = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = "STL �׸���";

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(wcex.lpszClassName, wcex.lpszClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, "can't create window", "error", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	auto start = chrono::system_clock::now();	// �ð� ���� ����

	HDC hdc = GetDC(hWnd);

	static RECT rect = { 550,0,850,50 };

	static string s = "  clear  :  c   |   save  :  s   |   replay  :  r";
	static LPSTR str = const_cast<char*>(s.c_str());
	TextOut(hdc, 0, 0, str, s.length());

	// �׸��� ���� ����-------------
	HPEN MyPen, OldPen;
	static int mousePosX, mousePosY;
	static bool lClick = false;
	static bool rClick = false;
	bool cleared = false;
	// ---------------------------

	// ���� �� ���÷��� ���� ����---
	static bool do_Save = false;
	static bool do_Replay = false;
	static bool set_replay = false;
	static bool is_Pause = false;
	// ---------------------------


	if (set_replay) {	// ���÷��� �ϱ��� ����
		int tm;
		int mx, my;
		bool lc, rc, i_c;

		s = "������ �ε� �� �Դϴ�..";
		re_v.clear();
		str = const_cast<char*>(s.c_str());
		TextOut(hdc, 550, 0, str, s.length());

		ifstream in("save.txt");
		while (!in.eof()) {
			in >> tm;
			in >> mx;
			in >> my;
			in >> lc;
			in >> rc;
			in >> i_c;
			re_v.push_back(Replay(tm, mx, my, lc, rc, i_c));
		}

		re_beg = re_v.begin();
		re_end = re_v.end();

		do_Replay = true;
		set_replay = false;
	}
	

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 1, NULL);
		SendMessage(hWnd, WM_TIMER, 0, 0);
		break;

	case WM_CHAR:
		if (do_Replay == false) {
			if (wParam == 'c') {
				InvalidateRect(hWnd, NULL, true);
				re_v.clear();
				cleared = true;
			}
			else if (wParam == 's') {
				if (do_Save == false) {
					do_Save = true;
					InvalidateRect(hWnd, NULL, true);
				}
				else {
					save();
					do_Save = false;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else if (wParam == 'r') {
				if (do_Save == false) {
					InvalidateRect(hWnd, NULL, true);
					set_replay = true;
				}
			}
		}
		else {
			if (wParam == 'p') {
				if (is_Pause == false) {
					is_Pause = true;
					InvalidateRect(hWnd, &rect, true);
					s = "�Ͻ�����!";
					str = const_cast<char*>(s.c_str());
					TextOut(hdc, 550, 0, str, s.length());
					KillTimer(hWnd, 0);
				}
				else {
					is_Pause = false;
					SetTimer(hWnd, 0, 0, NULL);
				}
			}

		}
		break;

	case WM_LBUTTONDOWN:
		if (do_Replay == false) {
			mousePosX = LOWORD(lParam);
			mousePosY = HIWORD(lParam);
			lClick = true;
		}
		break;
	case WM_LBUTTONUP:
		if (do_Replay == false)
			lClick = false;
		break;

	case WM_RBUTTONDOWN:
		if (do_Replay == false) {
			mousePosX = LOWORD(lParam);
			mousePosY = HIWORD(lParam);
			rClick = true;
		}
		break;
	case WM_RBUTTONUP:
		if (do_Replay == false)
			rClick = false;
		break;

	case WM_MOUSEMOVE:
		if (do_Replay == false) {
			if (lClick) {
				MyPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MoveToEx(hdc, mousePosX, mousePosY, NULL);
				mousePosX = LOWORD(lParam);
				mousePosY = HIWORD(lParam);
				LineTo(hdc, mousePosX, mousePosY);
				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
			}
			else if (rClick) {
				MyPen = CreatePen(PS_SOLID, 30, RGB(255, 255, 255));
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MoveToEx(hdc, mousePosX, mousePosY, NULL);
				mousePosX = LOWORD(lParam);
				mousePosY = HIWORD(lParam);
				LineTo(hdc, mousePosX, mousePosY);
				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
			}
			
			ReleaseDC(hWnd, hdc);
		}
		break;
		

	case WM_TIMER:

		// ���÷��� ���� �κ�.

		if (do_Replay) {
			s = "���÷��� �� �Դϴ�..     (�Ͻ����� : p)";
			str = const_cast<char*>(s.c_str());
			TextOut(hdc, 550, 0, str, s.length());

			Sleep(re_beg->getTime());	// ����.

			if (re_beg->getIs_clear())
				InvalidateRect(hWnd, NULL, true);	// �߰��� clear�� �߾��ٸ�..

			if (re_beg->getlClick()) {	// ��Ŭ��(�׸���)�Ͻ�..
				MyPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MoveToEx(hdc, re_beg->getX(), re_beg->getY(), NULL);	// ���� ����Ʈ
				++re_beg;	// ���ͷ����� �ѱ� ��
				LineTo(hdc, re_beg->getX(), re_beg->getY());	//���� �׸��� ����Ʈ
				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
			}
			else if (re_beg->getrClick()) {		// ��Ŭ��(�����)�Ͻ�..
				MyPen = CreatePen(PS_SOLID, 30, RGB(255, 255, 255));
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MoveToEx(hdc, re_beg->getX(), re_beg->getY(), NULL);
				++re_beg;
				LineTo(hdc, re_beg->getX(), re_beg->getY());
				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
			}
			else
				++re_beg;	// �ƹ��͵� ���� ���� ���ͷ����� �̵�.

			ReleaseDC(hWnd, hdc);

			if (re_beg == re_end - 1) {		// ���� ���� �����ͷ� ��� �������� ��..
				do_Replay = false;			// false�� �ٲٰ�
				InvalidateRect(hWnd, &rect, true);	// �ؽ�Ʈ �κ��� �ѹ� ��ȿȭ �����ش�.
			}
		}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	}

	if (do_Save) {	// ��ȭ �� �̶�� �� ȣ��� ���� �ð� �����Ͽ� ������ ����.
		auto end = chrono::system_clock::now();
		auto time = end - start;
		auto msec = chrono::duration_cast<chrono::milliseconds>(time);
		v.push_back(Replay(msec.count()+1, mousePosX, mousePosY, lClick, rClick, cleared));
		s = "��ȭ �� �Դϴ�.. (sŰ�� ������ ��ȭ ����!)";
		str = const_cast<char*>(s.c_str());
		TextOut(hdc, 0, 0, str, s.length());
	}

	else
		s = "  clear  :  c   |   save  :  s   |   replay  :  r";

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}