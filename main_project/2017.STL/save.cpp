#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
using namespace std;

void save()
{
	//
	cout << "�ҽ��� ���Ͽ� ����մϴ�." << endl;

	// �Լ��� ȣ��� �ð��� ����Ѵ�.
	// chrono::system_clock::time_point -> auto
	auto tp = chrono::system_clock::now();
	time_t temp = chrono::system_clock::to_time_t(tp);
	string date = ctime(&temp); // temp�� ũ�⶧���� �ּҸ� ���ڷ� �Ѱ��ش�.

								// �ҽ�.cpp�� ����.
	ifstream in("�ҽ�.cpp");
	if (!in) {
		cout << "�ҽ�.cpp�� �� �� �����ϴ�." << endl;
		exit(0);
	}

	// ����� ������ ����.
	ofstream out("2017.1�б�.STL ��ȭ.txt", ios::app);

	// �ҽ��� ������ ��� �ϵ��ũ�� ����.
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