#include "stdafx.h"
// STL 과제1
// 게임 점수 관리 프로그램

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
// Player들의 Id, Score를 담을 Vector와 Iterator를 선언.
vector<player_Data> p_player(Num_of_V);
vector<player_Data>::iterator vp_iter = p_player.begin();
vector<player_Data>::iterator vp_iter_end = p_player.end();

// Player들의 Rank를 정렬하기 위한 Vector를 선언.
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

// 내 Rank를 담을 Pair 생성
// first는 순위, second는 Rate
Rank my_A1_Rank;
Rank my_A2_Rank;
Rank my_A3_Rank;
Rank my_A4_Rank;
Rank my_A5_Rank;
Rank my_Total_Rank;
Rank pre_Player_Rank;
Rank next_player_Rank;

// 난수 엔진 생성.
random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> ui_Player(0, Num_of_V - 1); // 임의의 플레이어를 선택하기 위해..
uniform_int_distribution<> ui_score(0, 25000000); // play score는 0~25,000,000으로 초기화

int pre_my_rank;


//--------------------초기화/저장/불러오기---------------------

void createPlayer() {
	// 난수를 생성하자.
	default_random_engine dre;
	uniform_int_distribution<> ui_id(100000,999999); // id가 너무 길면 보기 안좋으니 범위 지정.
	uniform_int_distribution<> ui_def_score(0, 15000000); // defult score는 0~15,000,000으로 초기화
	int pre_id = 0;

	// vector로 20000개의 아이디와 점수 생성.
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
	
	// 정렬 하자.
	vp_iter = p_player.begin();
	sort(vp_iter, vp_iter_end);
	
	// 중복되지 않는 아이디를 만들자.
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		if (pre_id == vp_iter->first)
			vp_iter->first += 1;
		pre_id = vp_iter->first;
		//cout << vp_iter->first << '\0' << vp_iter->second[0] << "\t" << vp_iter->second[5] << endl;
	}
}

void loadData() {
	// 아이디와 점수를 불러오자.
	string str;
	// save한 파일을 열자.
	ifstream in("save.txt");

	// 없으면 새로 만든다.
	// = save.txt가 빈공간인지 검사
	in >> str;
	if (str == "\0") {
		in.close();
		cout << "\n\n\nsave.txt가 존재하지 않거나 내용이 없습니다." << endl;
		cout << "PlayerData를 새로 생성합니다." << endl;
		createPlayer();
	}
	// 있다면 불러오자.
	else {
		int cnt = 0;
		int p_cnt = 0;
		ifstream in("save.txt");
		vp_iter = p_player.begin();

		// 파일에서 읽어온 데이터를 벡터에 다시 넣자.

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

	cout << "Player의 Data를 저장합니다." << endl;
}




//-------------------------구현부------------------------------
// 메뉴 출력
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

// 아레나 선택화면 출력
int G_Select_Arena(int select) {
	cout << "\n================================================ Game Play ==================================================\n" << endl;
	cout << "\n\t=내 전체 점수: " << p_player[0].second[5] << "=\t=순위 변동: " << pre_my_rank - my_Total_Rank.first << "=\t=내 전체 순위: " << my_Total_Rank.first << "=\t=상위 " << my_Total_Rank.second << "%=" << endl;
	cout << "=============================================================================================================" << endl;
	cout << " =내 위 ID: " << player_Total_rank[my_Total_Rank.first - 1].second << ", 순위: " << my_Total_Rank.first - 1 << "위, 점수: " << player_Total_rank[my_Total_Rank.first - 1].first << "점="<< "\t=내 아래 ID: " << player_Total_rank[my_Total_Rank.first + 1].second << ", 순위: " << my_Total_Rank.first + 1 <<"위, 점수: " << player_Total_rank[my_Total_Rank.first + 1].first << "점=" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t1. Arena-1\t==내 점수: " << p_player[0].second[0] << "==\t==내 순위: " << my_A1_Rank.first << "위==\t==상위 " << my_A1_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t2. Arena-2\t==내 점수: " << p_player[0].second[1] << "==\t==내 순위: " << my_A2_Rank.first << "위==\t==상위 " << my_A2_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t3. Arena-3\t==내 점수: " << p_player[0].second[2] << "==\t==내 순위: " << my_A3_Rank.first << "위==\t==상위 " << my_A3_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t4. Arena-4\t==내 점수: " << p_player[0].second[3] << "==\t==내 순위: " << my_A4_Rank.first << "위==\t==상위 " << my_A4_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t5. Arena-5\t==내 점수: " << p_player[0].second[4] << "==\t==내 순위: " << my_A5_Rank.first << "위==\t==상위 " << my_A5_Rank.second << "%==" << endl;
	cout << "=============================================================================================================" << endl;
	cout << "\t메뉴로 돌아가려면 '1~5를 제외한 숫자'를 입력해 주세요." << endl;
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

// 랭킹 검색화면 출력
void G_id_Search_Menu() {

	// id를 입력받아서
	int id;
	int x;
	cout << "\n===================================== Rank ========================================\n" << endl;
	cout << "\n>> Insert ID: ";
	cin >> id;
	system("cls");

	// 점수 출력.
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		if (id == vp_iter->first) {
			cout << "'\n\t\t" << id << "님의 점수표입니다." << endl;
			cout << "\n\tArena-1\t==점수: " << vp_iter->second[0] << "==\t==순위: " << my_A1_Rank.first << "위==\t==상위 " << my_A1_Rank.second << "%==" << endl;
			cout << "\tArena-2\t==점수: " << vp_iter->second[1] << "==\t==순위: " << my_A2_Rank.first << "위==\t==상위 " << my_A2_Rank.second << "%==" << endl;
			cout << "\tArena-3\t==점수: " << vp_iter->second[2] << "==\t==순위: " << my_A3_Rank.first << "위==\t==상위 " << my_A3_Rank.second << "%==" << endl;
			cout << "\tArena-4\t==점수: " << vp_iter->second[3] << "==\t==순위: " << my_A4_Rank.first << "위==\t==상위 " << my_A4_Rank.second << "%==" << endl;
			cout << "\tArena-5\t==점수: " << vp_iter->second[4] << "==\t==순위: " << my_A5_Rank.first << "위==\t==상위 " << my_A5_Rank.second << "%==" << endl;
			cout << "\n\tTotal\t==점수: " << vp_iter->second[5] << "==\t==순위: " << my_Total_Rank.first << "위==\t==상위 " << my_Total_Rank.second << "%==" << endl;
		}
	}
	cout << "\n\n\t메뉴로 돌아가려면 '숫자'를 입력해주세요! >> ";
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

	// 나는 반드시 플레이!
	new_score = ui_score(dre);
	if (p_player[0].second[num_of_Arena - 1] < new_score) {
		int pre_score = p_player[0].second[num_of_Arena - 1];
		p_player[0].second[num_of_Arena - 1] = new_score;
		p_player[0].second[5] = p_player[0].second[0] + p_player[0].second[1] + p_player[0].second[2] + p_player[0].second[3] + p_player[0].second[4];
	}
	// 랜덤 10% 플레이어도 플레이!
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
	cout << "\n\n\t게임이 진행중입니다. 잠시만 기다려 주세요..";
}

void G_Get_Player_Rank(int num_of_Arena) {

	switch (num_of_Arena) {

		// case 1~5는 같은 기능을 수행한다.

	case 1:
		// rena1의 Rank를 가진 객체에 현재 데이터 삽입
		pRank_A1_iter = player_A1_rank.begin();
		for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
			pRank_A1_iter->first = vp_iter->second[num_of_Arena - 1];
			pRank_A1_iter->second = vp_iter->first;
			++pRank_A1_iter;
		}

		// Arena1의 Rank 객체를 정렬
		pRank_A1_iter = player_A1_rank.begin();
		sort(player_A1_rank.rbegin(), player_A1_rank.rend());

		// Arena1에서의 내 순위를 얻는다.
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

	// switch문 끝.
	// 이번엔 전체 점수를 관리하자.

	// 가지고 있는 총점 데이터로 Rank_Total 객체를 초기화
	pRank_Total_iter = player_Total_rank.begin();
	for (vp_iter = p_player.begin(); vp_iter != vp_iter_end; ++vp_iter) {
		pRank_Total_iter->first = vp_iter->second[5];
		pRank_Total_iter->second = vp_iter->first;
		++pRank_Total_iter;
	}

	// Rank_Total 객체를 전체 점수 순서대로 정렬
	pRank_Total_iter = player_Total_rank.begin();
	sort(player_Total_rank.rbegin(), player_Total_rank.rend());
	
	// 내 전체 점수의 순위와 비율 받기
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
	// 데이터를 불러온다. 없으면 설정해둔 random default로 생성한다.
	cout << "\n\n\tNow Loading..";
	loadData();

	// Rank를 생성하자. 
	for (int i = 1; i < 6; ++i)
		G_Get_Player_Rank(i);
	system("cls");

	// 메뉴를 만들자.
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