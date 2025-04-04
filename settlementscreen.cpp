#include<iostream>
#include"inireading/SimpleIni.h"
using namespace std;

//read setting.ini
CSimpleIniA ini;
SI_Error rc = ini.LoadFile("setting.ini");
bool setting_alwaysaskgamereview=ini.GetBoolValue("games","always_ask_game_review",true);
bool setting_singlegamereview=ini.GetBoolValue("files","single_game_review",true);

int main(int argc,char* argv[]){
	argc=argc;
	cout<<"Game Time(��ʱ):"<<argv[1]<<"s\n";
	cout<<"Steps(����):"<<argv[2]<<"\n";
	cout<<"Winner(ʤ��):"<<(argv[3]=="1"?"��(Black)":"��(White)")<<"\n";
	cout<<"Review this game(�ع�)(y/n):";
	if(setting_alwaysaskgamereview){
		char isReview=getchar();
		if(isReview=='y'){
			if(setting_singlegamereview){
				system("start lastBoardGame.txt");
			}
			else{
				const char* com=((string)"start BoardGame"+argv[4]+".txt").c_str();
				system(com);
			}
		}
	}
	
	return 0;
}
