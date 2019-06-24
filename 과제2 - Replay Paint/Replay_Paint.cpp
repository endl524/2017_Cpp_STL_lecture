#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>


using namespace std;

class Replay {	// 리플레이 데이터를 담을 객체
	int time;	// 시간
	int mx;		// 마우스 x좌표
	int my;		// 마우스 y좌표
	bool lClick;	// 좌클릭을 했는지?
	bool rClick;	// 우클릭을 했는지?
	bool is_clear;	// clear를 했는지?
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

//-----벡터와 이터레이터 선언-----
vector<Replay> v;
vector<Replay>::iterator beg;
vector<Replay> re_v;
vector<Replay>::iterator re_beg;
vector<Replay>::iterator re_end;
//------------------------------

void save() {	// 세이브 함수.
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
	wcex.lpszClassName = "STL 그림판";

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
	auto start = chrono::system_clock::now();	// 시간 측정 시작

	HDC hdc = GetDC(hWnd);

	static RECT rect = { 550,0,850,50 };

	static string s = "  clear  :  c   |   save  :  s   |   replay  :  r";
	static LPSTR str = const_cast<char*>(s.c_str());
	TextOut(hdc, 0, 0, str, s.length());

	// 그리기 관련 변수-------------
	HPEN MyPen, OldPen;
	static int mousePosX, mousePosY;
	static bool lClick = false;
	static bool rClick = false;
	bool cleared = false;
	// ---------------------------

	// 저장 및 리플레이 관련 변수---
	static bool do_Save = false;
	static bool do_Replay = false;
	static bool set_replay = false;
	static bool is_Pause = false;
	// ---------------------------


	if (set_replay) {	// 리플레이 하기전 세팅
		int tm;
		int mx, my;
		bool lc, rc, i_c;

		s = "데이터 로딩 중 입니다..";
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
					s = "일시정지!";
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

		// 리플레이 수행 부분.

		if (do_Replay) {
			s = "리플레이 중 입니다..     (일시정지 : p)";
			str = const_cast<char*>(s.c_str());
			TextOut(hdc, 550, 0, str, s.length());

			Sleep(re_beg->getTime());	// 재운다.

			if (re_beg->getIs_clear())
				InvalidateRect(hWnd, NULL, true);	// 중간에 clear를 했었다면..

			if (re_beg->getlClick()) {	// 좌클릭(그리기)일시..
				MyPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MoveToEx(hdc, re_beg->getX(), re_beg->getY(), NULL);	// 시작 포인트
				++re_beg;	// 이터레이터 넘긴 후
				LineTo(hdc, re_beg->getX(), re_beg->getY());	//다음 그리기 포인트
				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
			}
			else if (re_beg->getrClick()) {		// 우클릭(지우기)일시..
				MyPen = CreatePen(PS_SOLID, 30, RGB(255, 255, 255));
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MoveToEx(hdc, re_beg->getX(), re_beg->getY(), NULL);
				++re_beg;
				LineTo(hdc, re_beg->getX(), re_beg->getY());
				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
			}
			else
				++re_beg;	// 아무것도 안할 때도 이터레이터 이동.

			ReleaseDC(hWnd, hdc);

			if (re_beg == re_end - 1) {		// 벡터 내의 데이터로 모두 수행했을 때..
				do_Replay = false;			// false로 바꾸고
				InvalidateRect(hWnd, &rect, true);	// 텍스트 부분을 한번 무효화 시켜준다.
			}
		}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	}

	if (do_Save) {	// 녹화 중 이라면 매 호출시 마다 시간 측정하여 데이터 축적.
		auto end = chrono::system_clock::now();
		auto time = end - start;
		auto msec = chrono::duration_cast<chrono::milliseconds>(time);
		v.push_back(Replay(msec.count()+1, mousePosX, mousePosY, lClick, rClick, cleared));
		s = "녹화 중 입니다.. (s키를 누르면 녹화 종료!)";
		str = const_cast<char*>(s.c_str());
		TextOut(hdc, 0, 0, str, s.length());
	}

	else
		s = "  clear  :  c   |   save  :  s   |   replay  :  r";

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}