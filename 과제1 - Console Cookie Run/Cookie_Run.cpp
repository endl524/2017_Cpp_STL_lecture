#include "stdafx.h"
// STL ����1
// ���� ���� ���� ���α׷�

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;
typedef pair<int, vector<int>> player_Data;
typedef pair<int, int> Rank;
#define Num_of_V 20000

//------------------------global------------------------------
// Player���� Id, Score�� ���� Vector�� Iterator�� ����.
vector<player_Data> p_player(Num_of_V);
vector<player_Data>::iterator vp_iter = p_player.begin();
vector<player_Data>::iterator vp_iter_end = p_player.end();

// Player���� Rank�� �����ϱ� ���� Vector�� ����.
vector<Rank> player_A1_rank(Num_of_V);
vector<Rank>::iterator pRank_A1_iter = player_A1_rank.begin();
vector<Rank>::iterator pRank_A1_iter_end = player_A1_rank.end();
vector<Rank> player_A2_rank(Num_of_V);
vector<Rank>::iterator pRank_A2_iter = player_A2_rank.begin();
vector<Rank>::iterator pRank_A2_iter_end = player_A2_rank.end();
vector<Rank> player_A3_rank(Num_of_V);
vector<Rank>::iterator pRank_A3_iter = player_A3_rank.begin();
vector<Rank>::iterator pRank_A3_iter_end = player_A3_rank.end();
vector<Rank> player_A4_rank(Num_of_V);
vector<Rank>::iterator pRank_A4_iter = player_A4_rank.begin();
vector<Rank>::iterator pRank_A4_iter_end = player_A4_rank.end();
vector<Rank> player_A5_rank(Num_of_V);
vector<Rank>::iterator pRank_A5_iter = player_A5_rank.begin();
vector<Rank>::iterator pRank_A5_iter_end = player_A5_rank.end();
vector<Rank> player_Total_rank(Num_of_V);
vector<Rank>::iterator pRank_Total_iter = player_Total_rank.begin();
vector<Rank>::iterator pRank_Total_iter_end = player_Total_rank.end();

// �� Rank�� ���� Pair ����
// first�� ����, second�� Rate
Rank my_A1_Rank;
Rank my_A2_Rank;
Rank my_A3_Rank;
Rank my_A4_Rank;
Rank my_A5_Rank;
Rank my_Total_Rank;
Rank pre_Player_Rank;
Rank next_player_Rank;

// ���� ���� ����.
random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> ui_Player(0, Num_of_V - 1); // ������ �÷��̾ �����ϱ� ����..
uniform_int_distribution<> ui_score(0, 25000000); // play score�� 0~25,000,000���� �ʱ�ȭ

int pre_my_rank;


//--------------------�ʱ�ȭ/����/�ҷ�����---------------------

void createPlayer() {
	// ������ ��������.
	default_random_engine dre;
	uniform_int_distribution<> ui_id(100000,999999); // id�� �ʹ� ��� ���� �������� ���� ����.
	uniform_int_distribution<> ui_def_score(0, 15000000); // defult score�� 0~15,000,000���� �ʱ�ȭ
	int pre_id = 0;

	// vector�� 20000���� ���̵�� ���� ����.
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		int sum = 0;
		vp_iter->first = ui_id(dre);
		for (int i = 0; i < 6; ++i) {
			if (i == 5) {
				vp_iter->second.push_back(sum);
				//cout << sum << endl;
			}
			else {
				vp_iter->second.push_back(ui_def_score(dre));
				sum += vp_iter->second[i];
				//cout << vp_iter->second[i] << '\t' << sum << endl;
			}
		}
	}
	
	// ���� ����.
	vp_iter = p_player.begin();
	sort(vp_iter, vp_iter_end);
	
	// �ߺ����� �ʴ� ���̵� ������.
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		if (pre_id == vp_iter->first)
			vp_iter->first += 1;
		pre_id = vp_iter->first;
		//cout << vp_iter->first << '\0' << vp_iter->second[0] << "\t" << vp_iter->second[5] << endl;
	}
}

void loadData() {
	// ���̵�� ������ �ҷ�����.
	string str;
	// save�� ������ ����.
	ifstream in("save.txt");

	// ������ ���� �����.
	// = save.txt�� ��������� �˻�
	in >> str;
	if (str == "\0") {
		in.close();
		cout << "\n\n\nsave.txt�� �������� �ʰų� ������ �����ϴ�." << endl;
		cout << "PlayerData�� ���� �����մϴ�." << endl;
		createPlayer();
	}
	// �ִٸ� �ҷ�����.
	else {
		int cnt = 0;
		int p_cnt = 0;
		ifstream in("save.txt");
		vp_iter = p_player.begin();

		// ���Ͽ��� �о�� �����͸� ���Ϳ� �ٽ� ����.

		while (p_cnt != Num_of_V) {
			if (cnt == 0) {
				in >> str;
				vp_iter->first = stoi(str);
				cnt = 1;
			}
			else {
				for (int i = 0; i < 6; ++i) {
					in >> str;
					vp_iter->second.push_back(stoi(str));
				}
				cnt = 0;
				++p_cnt;
				++vp_iter;
			}
		}
		in.close();
	}
}

void saveData(){
	ofstream save("save.txt", ios::trunc);
	for (vp_iter = p_player.begin(); vp_iter != p_player.end(); ++vp_iter) {
		//cout << vp_iter->first << endl;
		save << vp_iter->first;
		for (int i = 0; i < 6; ++i) {
			//cout << vp_iter->second[i] << endl;
			save << '\t' << vp_iter->second[i];
		}
		save << endl;
	}
	save.close();

	cout << "Player�� Data�� �����մϴ�." << endl;
}




//-------------------------������------------------------------
// �޴� ���
int G_Menu(int select) {
	cout << "\n=================== Menu =====================\n" << endl;
	cout << "\t1. Game Play" << endl;
	cout << "\t2. Rank" << endl;
	cout << "\t3. Save & Exit" << endl;
	cout << "\n>> Select: ";
	cin >> select;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(256, '\n');
		system("cls");
		G_Menu(select);
	}
	return select;
}

// �Ʒ��� ����ȭ�� ���
int G_Select_Arena(int select) {
	cout << "\n================================================ Game Play ==================================================\n" << endl;
	cout << "\n\t=�� ��ü ����: " << p_player[0].second[5] << "=\t=���� ����: " << pre_my_rank - my_Total_Rank.first << "=\t=�� ��ü ����: " << my_Total_Rank.first << "=\t=���� " << my_Total_Rank.second << "%=" << endl;
	cout << "=============================================================================================================" << endl;
	cout << " =�� �� ID: " << player_Total_rank[my_Total_Rank.first - 1].second << ", ����: " << my_Total_Rank.first - 1 << "��, ����: " << player_Total_rank[my_Total_Rank.first - 1].first << "��="<< "\t=�� �Ʒ� ID: " << player_Total_rank[my_Total_Rank.first + 1].second << ", ����: " << my_Total_Rank.first + 1 <<"��, ����: " << player_Total_rank[my_Total_Rank.first + 1].first << "��=" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t1. Arena-1\t==�� ����: " << p_player[0].second[0] << "==\t==�� ����: " << my_A1_Rank.first << "��==\t==���� " << my_A1_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t2. Arena-2\t==�� ����: " << p_player[0].second[1] << "==\t==�� ����: " << my_A2_Rank.first << "��==\t==���� " << my_A2_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t3. Arena-3\t==�� ����: " << p_player[0].second[2] << "==\t==�� ����: " << my_A3_Rank.first << "��==\t==���� " << my_A3_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t4. Arena-4\t==�� ����: " << p_player[0].second[3] << "==\t==�� ����: " << my_A4_Rank.first << "��==\t==���� " << my_A4_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t5. Arena-5\t==�� ����: " << p_player[0].second[4] << "==\t==�� ����: " << my_A5_Rank.first << "��==\t==���� " << my_A5_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t�޴��� ���ư����� '1~5�� ������ ����'�� �Է��� �ּ���." << endl;
	cout << "\n\n>> Select: ";
	cin >> select;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(256, '\n');
		system("cls");
		G_Select_Arena(select);
	}
	return select;
}

// ��ŷ �˻�ȭ�� ���
void G_id_Search_Menu() {

	// id�� �Է¹޾Ƽ�
	int id;
	int x;
	cout << "\n===================================== Rank ========================================\n" << endl;
	cout << "\n>> Insert ID: ";
	cin >> id;
	system("cls");

	// ���� ���.
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		if (id == vp_iter->first) {
			cout << "'\n\t\t" << id << "���� ����ǥ�Դϴ�." << endl;
			cout << "\n\tArena-1\t==����: " << vp_iter->second[0] << "==\t==����: " << my_A1_Rank.first << "��==\t==���� " << my_A1_Rank.second << "%==" << endl;
			cout << "\tArena-2\t==����: " << vp_iter->second[1] << "==\t==����: " << my_A2_Rank.first << "��==\t==���� " << my_A2_Rank.second << "%==" << endl;
			cout << "\tArena-3\t==����: " << vp_iter->second[2] << "==\t==����: " << my_A3_Rank.first << "��==\t==���� " << my_A3_Rank.second << "%==" << endl;
			cout << "\tArena-4\t==����: " << vp_iter->second[3] << "==\t==����: " << my_A4_Rank.first << "��==\t==���� " << my_A4_Rank.second << "%==" << endl;
			cout << "\tArena-5\t==����: " << vp_iter->second[4] << "==\t==����: " << my_A5_Rank.first << "��==\t==���� " << my_A5_Rank.second << "%==" << endl;
			cout << "\n\tTotal\t==����: " << vp_iter->second[5] << "==\t==����: " << my_Total_Rank.first << "��==\t==���� " << my_Total_Rank.second << "%==" << endl;
		}
	}
	cout << "\n\n\t�޴��� ���ư����� '����'�� �Է����ּ���! >> ";
	cin >> x;

	if (cin.fail()) {
		cin.clear();
		cin.ignore(256, '\n');
		system("cls");
		G_id_Search_Menu();
	}
	system("cls");
}

void G_Play_Arena(int num_of_Arena) {
	int pre_id = 0;
	int index = 0;
	int new_score = 0;
	pre_my_rank = my_Total_Rank.first;

	// ���� �ݵ�� �÷���!
	new_score = ui_score(dre);
	if (p_player[0].second[num_of_Arena - 1] < new_score) {
		int pre_score = p_player[0].second[num_of_Arena - 1];
		p_player[0].second[num_of_Arena - 1] = new_score;
		p_player[0].second[5] = p_player[0].second[0] + p_player[0].second[1] + p_player[0].second[2] + p_player[0].second[3] + p_player[0].second[4];
	}
	// ���� 10% �÷��̾ �÷���!
	for(int i = 0; i < Num_of_V/10; ++i) {
		index = ui_Player(dre);
		new_score = ui_score(dre);
		if (p_player[index].second[num_of_Arena - 1] < new_score) {
			int pre_score = p_player[index].second[num_of_Arena - 1];
			p_player[index].second[num_of_Arena - 1] = new_score;
			p_player[index].second[5] = p_player[index].second[0] + p_player[index].second[1] + p_player[index].second[2] + p_player[index].second[3] + p_player[index].second[4];
		}
	}
	system("cls");
	cout << "\n\n\t������ �������Դϴ�. ��ø� ��ٷ� �ּ���..";
}

void G_Get_Player_Rank(int num_of_Arena) {

	switch (num_of_Arena) {

		// case 1~5�� ���� ����� �����Ѵ�.

	case 1:
		// rena1�� Rank�� ���� ��ü�� ���� ������ ����
		pRank_A1_iter = player_A1_rank.begin();
		for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
			pRank_A1_iter->first = vp_iter->second[num_of_Arena - 1];
			pRank_A1_iter->second = vp_iter->first;
			++pRank_A1_iter;
		}

		// Arena1�� Rank ��ü�� ����
		pRank_A1_iter = player_A1_rank.begin();
		sort(player_A1_rank.rbegin(), player_A1_rank.rend());

		// Arena1������ �� ������ ��´�.
		pRank_A1_iter = player_A1_rank.begin();
		for (int i = 0; i <= Num_of_V; ++i) {
			if (pRank_A1_iter->second == p_player[0].first) {
				my_A1_Rank.first = i;
				my_A1_Rank.second = i / (Num_of_V / 100);

				break;
			}
			else
				++pRank_A1_iter;
		}
		break;

	case 2:
		pRank_A2_iter = player_A2_rank.begin();
		for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
			pRank_A2_iter->first = vp_iter->second[num_of_Arena - 1];
			pRank_A2_iter->second = vp_iter->first;
			++pRank_A2_iter;
		}
		pRank_A2_iter = player_A2_rank.begin();
		sort(player_A2_rank.rbegin(), player_A2_rank.rend());
		pRank_A2_iter = player_A2_rank.begin();

		for (int i = 0; i <= Num_of_V; ++i) {
			if (pRank_A2_iter->second == p_player[0].first) {
				my_A2_Rank.first = i;
				my_A2_Rank.second = i / (Num_of_V / 100);
				break;
			}
			else
				++pRank_A2_iter;
		}
		break;

	case 3:
		pRank_A3_iter = player_A3_rank.begin();
		for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
			pRank_A3_iter->first = vp_iter->second[num_of_Arena - 1];
			pRank_A3_iter->second = vp_iter->first;
			++pRank_A3_iter;
		}
		pRank_A3_iter = player_A3_rank.begin();
		sort(player_A3_rank.rbegin(), player_A3_rank.rend());
		pRank_A3_iter = player_A3_rank.begin();
		for (int i = 0; i <= Num_of_V; ++i) {
			if (pRank_A3_iter->second == p_player[0].first) {
				my_A3_Rank.first = i;
				my_A3_Rank.second = i / (Num_of_V / 100);
				break;
			}
			else
				++pRank_A3_iter;
		}
		break;

	case 4:
		pRank_A4_iter = player_A4_rank.begin();
		for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
			pRank_A4_iter->first = vp_iter->second[num_of_Arena - 1];
			pRank_A4_iter->second = vp_iter->first;
			++pRank_A4_iter;
		}
		pRank_A4_iter = player_A4_rank.begin();
		sort(player_A4_rank.rbegin(), player_A4_rank.rend());
		pRank_A4_iter = player_A4_rank.begin();

		for (int i = 0; i <= Num_of_V; ++i) {
			if (pRank_A4_iter->second == p_player[0].first) {
				my_A4_Rank.first = i;
				my_A4_Rank.second = i / (Num_of_V / 100);
				break;
			}
			else
				++pRank_A4_iter;
		}
		break;

	case 5:
		pRank_A5_iter = player_A5_rank.begin();
		for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
			pRank_A5_iter->first = vp_iter->second[num_of_Arena - 1];
			pRank_A5_iter->second = vp_iter->first;
			++pRank_A5_iter;
		}
		pRank_A5_iter = player_A5_rank.begin();
		sort(player_A5_rank.rbegin(), player_A5_rank.rend());
		pRank_A5_iter = player_A5_rank.begin();

		for (int i = 0; i <= Num_of_V; ++i) {
			if (pRank_A5_iter->second == p_player[0].first) {
				my_A5_Rank.first = i;
				my_A5_Rank.second = i / (Num_of_V / 100);
				break;
			}
			else
				++pRank_A5_iter;
		}
		break;
	}

	// switch�� ��.
	// �̹��� ��ü ������ ��������.

	// ������ �ִ� ���� �����ͷ� Rank_Total ��ü�� �ʱ�ȭ
	pRank_Total_iter = player_Total_rank.begin();
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		pRank_Total_iter->first = vp_iter->second[5];
		pRank_Total_iter->second = vp_iter->first;
		++pRank_Total_iter;
	}

	// Rank_Total ��ü�� ��ü ���� ������� ����
	pRank_Total_iter = player_Total_rank.begin();
	sort(player_Total_rank.rbegin(), player_Total_rank.rend());
	
	// �� ��ü ������ ������ ���� �ޱ�
	pRank_Total_iter = player_Total_rank.begin();
	for (int i = 0; i <= Num_of_V; ++i) {
		if (pRank_Total_iter->second == p_player[0].first) {
			my_Total_Rank.first = i;
			my_Total_Rank.second = i / (Num_of_V / 100);
			break;
		}
		else
			++pRank_Total_iter;
	}
}



//-------------------------main-----------------------------

int main()
{
	// �����͸� �ҷ��´�. ������ �����ص� random default�� �����Ѵ�.
	cout << "\n\n\tNow Loading..";
	loadData();

	// Rank�� ��������. 
	for (int i = 1; i < 6; ++i)
		G_Get_Player_Rank(i);
	system("cls");

	// �޴��� ������.
	int select = 0;

	while (1) {
		switch (G_Menu(select)) {
		case 1:
			system("cls");
			switch (G_Select_Arena(select)) {
			case 1:
				system("cls");
				G_Play_Arena(1);
				G_Get_Player_Rank(1);
				break;
			case 2:
				system("cls");
				G_Play_Arena(2);
				G_Get_Player_Rank(2);
				break;
			case 3:
				system("cls");
				G_Play_Arena(3);
				G_Get_Player_Rank(3);
				break;
			case 4:
				system("cls");
				G_Play_Arena(4);
				G_Get_Player_Rank(4);
				break;
			case 5:
				system("cls");
				G_Play_Arena(5);
				G_Get_Player_Rank(5);
				break;
			default:
				system("cls");
				break;
			}
			break;
		case 2:
			system("cls");
			G_id_Search_Menu();
			break;
		case 3:
			system("cls");
			cout << "\n================= Good Bye ===================\n" << endl;
			saveData();
			return 0;
		default:
			system("cls");
			break;
		}
		system("cls");
	}
}