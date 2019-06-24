#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
using namespace std;

void save()
{
	//
	cout << "소스를 파일에 기록합니다." << endl;

	// 함수가 호출된 시간을 기록한다.
	// chrono::system_clock::time_point -> auto
	auto tp = chrono::system_clock::now();
	time_t temp = chrono::system_clock::to_time_t(tp);
	string date = ctime(&temp); // temp가 크기때문에 주소만 인자로 넘겨준다.

								// 소스.cpp를 연다.
	ifstream in("소스.cpp");
	if (!in) {
		cout << "소스.cpp를 열 수 없습니다." << endl;
		exit(0);
	}

	// 기록할 파일을 연다.
	ofstream out("2017.1학기.STL 월화.txt", ios::app);

	// 소스의 내용을 모두 하드디스크에 쓴다.
	out << endl << endl;
	out << "===================" << endl;

	date.resize(date.size() - 1);

	out << date << endl;
	out << "===================" << endl;
	out << endl << endl;

	char c;

	while (in.get(c))
		out.put(c);
}