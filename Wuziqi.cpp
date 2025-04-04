#include<iostream>
#include<string>
#include<time.h>
#include<fstream>
#include"inireading/SimpleIni.h"
using namespace std;

//read setting.ini
CSimpleIniA ini;
SI_Error rc = ini.LoadFile("setting.ini");
bool setting_darkmode=ini.GetBoolValue("UI","darkmode",true);
bool setting_hints=ini.GetBoolValue("UI","hints",true);
bool setting_settlementscreen=ini.GetBoolValue("UI","settlement_screen",true);
bool setting_alwaysrewritesetting=ini.GetBoolValue("files","always_rewrite_setting",true);
bool setting_singlegamereview=ini.GetBoolValue("files","single_game_review",true);
bool setting_hindblanks=ini.GetBoolValue("games","hind_blanks",false);

const int BOARDCROSS=15;
int a[BOARDCROSS][BOARDCROSS]={};
int winner=0;//0:noun, 1:Black, 2:White;
clock_t starttime=clock(),endtime;
int steps=0;
pair<int,int> waysBoard[BOARDCROSS*BOARDCROSS]={};

void showBoard(){
	system("cls");
	cout<<"  ";
	for(int i=0;i<BOARDCROSS;i++){
		cout<<(i<10?(char)(i+'0'):(char)('a'+i-10))<<" ";
	}
	cout<<"\n";
	for(int i=0;i<BOARDCROSS;i++){
		cout<<(i<10?to_string(i)+" ":to_string(i));
		for(int j=0;j<BOARDCROSS;j++){
			cout<<(a[i][j]==0?(setting_hindblanks?"  ":"□"):(a[i][j]==1?"●":"○"));
		}
		cout<<"\n";
	}
}
void turn(bool isblack){
	showBoard();
	if(setting_hints) cout<<"(a = 10, b = 11, etc.)(input:horizontal line（横）, longitudinal line（纵）)\n";
	cout<<"\nTurn:"<<(isblack?"●(Black)":"○(White)")<<"\tWinner:"<<(winner==0?"None":(winner==1?"Black":"White"))<<"\n";
}
pair<int,int> inputPlaceChess(){
	cout<<"Play:";
	pair<int,int> wherePlaceChess;
	cin>>wherePlaceChess.first>>wherePlaceChess.second;
	if(wherePlaceChess.first>=BOARDCROSS||wherePlaceChess.first<0||wherePlaceChess.second>=BOARDCROSS||wherePlaceChess.second<0){
		cout<<"Wrong Place!\n";
		return inputPlaceChess();
	}
	else{
		return wherePlaceChess;
	}
}
void placeChess(int i,int j,bool isPlaceBack){
	if(a[i][j]==0){
		a[i][j]=(isPlaceBack?1:2);
	}
}
int checkWinner(){
	for(int i=0;i<BOARDCROSS;i++){
		for(int j=0;j<BOARDCROSS-4;j++){
			if(a[i][j]==a[i][j+1]&&a[i][j+1]==a[i][j+2]&&a[i][j+2]==a[i][j+3]&&a[i][j+3]==a[i][j+4]&&a[i][j]==1){
				return 1;
			}
			else if(a[i][j]==a[i][j+1]&&a[i][j+1]==a[i][j+2]&&a[i][j+2]==a[i][j+3]&&a[i][j+3]==a[i][j+4]&&a[i][j]==2){
				return 2;
			}
		}
	}
	for(int j=0;j<BOARDCROSS;j++){
		for(int i=0;i<BOARDCROSS-4;i++){
			if(a[i][j]==a[i+1][j]&&a[i+1][j]==a[i+2][j]&&a[i+2][j]==a[i+3][j]&&a[i+3][j]==a[i+4][j]&&a[i][j]==1){
				return 1;
			}
			else if(a[i][j]==a[i+1][j]&&a[i+1][j]==a[i+2][j]&&a[i+2][j]==a[i+3][j]&&a[i+3][j]==a[i+4][j]&&a[i][j]==2){
				return 2;
			}
		}
	}
	for(int i=0;i<BOARDCROSS-4;i++){
		for(int j=0;j<BOARDCROSS-4;j++){
			if(a[i][j]==a[i+1][j+1]&&a[i+1][j+1]==a[i+2][j+2]&&a[i+2][j+2]==a[i+3][j+3]&&a[i+3][j+3]==a[i+4][j+4]&&a[i][j]==1){
				return 1;
			}
			else if(a[i][j]==a[i+1][j+1]&&a[i+1][j+1]==a[i+2][j+2]&&a[i+2][j+2]==a[i+3][j+3]&&a[i+3][j+3]==a[i+4][j+4]&&a[i][j]==2){
				return 2;
			}
		}
	}
	for(int i=0;i<BOARDCROSS;i++){
		for(int j=4;j<BOARDCROSS;j++){
			if(a[i][j]==a[i+1][j-1]&&a[i+1][j-1]==a[i+2][j-2]&&a[i+2][j-2]==a[i+3][j-3]&&a[i+3][j-3]==a[i+4][j-4]&&a[i][j]==1){
				return 1;
			}
			else if(a[i][j]==a[i+1][j-1]&&a[i+1][j-1]==a[i+2][j-2]&&a[i+2][j-2]==a[i+3][j-3]&&a[i+3][j-3]==a[i+4][j-4]&&a[i][j]==2){
				return 2;
			}
		}
	}
	return 0;
}
void placeChessThatBelongToMain();
void settlementScreen(double spendtime,int allsteps,int winner);
bool isturnBlack=true;
int main(){
	
	//Rewrite setting.ini
	if(setting_alwaysrewritesetting){
		if(rc<0){
			ofstream outfile;
			//打开文件，设置文件打开模式为 std::ios::out，实现覆盖写入
			outfile.open("setting.ini",ios::out);
			//向文件写入数据
			outfile<<"[UI]\ndarkmode = true\nhints = true\nsettlement_screen = true\n\n[files]\nalways_rewrite_setting = true\nsingle_game_review = true\n\n[games]\nhind_blanks = false\nalways_ask_game_review = true\n";
			//关闭文件
			outfile.close();
		}
	}
	
	if(!setting_darkmode){
		system("color F0");
	}
	while(1){
		turn(isturnBlack);
		if(winner!=0){
			endtime=clock();
			settlementScreen((double)(endtime-starttime)/CLOCKS_PER_SEC,steps,winner);
			system("pause");
			break;
		}
		if(setting_settlementscreen) placeChessThatBelongToMain();
		winner=checkWinner();
		steps++;
		isturnBlack=!isturnBlack;
	}
	
	return 0;
}
void placeChessThatBelongToMain(){
	pair<int,int> playedPlace;
	playedPlace=inputPlaceChess();
	if(a[playedPlace.first][playedPlace.second]==0){
		waysBoard[steps].first=playedPlace.first;waysBoard[steps].second=playedPlace.second;
		placeChess(playedPlace.first,playedPlace.second,isturnBlack);
	}
	else{
		placeChessThatBelongToMain();
	}
}
void settlementScreen(double spendtime,int allsteps,int winner){
	cout<<"Game Over!\n";
	
	time_t timep;
	time(&timep); /*获取time_t类型当前时间*/   
	/*转换为常见的字符串：Fri Jan 11 17:04:08 2008*/
	//printf("%s",ctime(&timep));

	time_t curtime;
	time(&curtime);
	tm *nowtime = localtime(&curtime);
	const char* reviewFileName=(setting_singlegamereview?"lastBoardGame.txt":("BoardGame"+to_string(1+nowtime->tm_mon)+to_string(nowtime->tm_mday)+to_string(nowtime->tm_hour)+to_string(nowtime->tm_min)+to_string(nowtime->tm_sec)+".txt").c_str());
	/*
	    time_t curtime;
	    time(&curtime);
	    cout << "1970到目前经过秒数:" << time(&curtime) << endl;
	    cout << "本地日期和时间:" << ctime(&curtime) << endl;
	    tm *nowtime = localtime(&curtime);
	    // 输出tm结构的年月日
	    cout << "年: " << 1900 + nowtime->tm_year << endl;
	    cout << "月: " << 1 + nowtime->tm_mon << endl;
	    cout << "日: " << nowtime->tm_mday << endl;
	    cout << "时间: " << nowtime->tm_hour << ":";
	    cout << nowtime->tm_min << ":";
	    cout << nowtime->tm_sec << endl;
	*/
	
	ofstream outfile;
    //打开文件，设置文件打开模式为 std::ios::out，实现覆盖写入
    outfile.open(reviewFileName,ios::out);
    //向文件写入数据
    outfile<<ctime(&timep)<<"\n";
    for(int i=0;i<steps;i++){
    	outfile<<waysBoard[i].first<<" "<<waysBoard[i].second<<"\n";
	}
    //关闭文件
    outfile.close();
	    
	const string com="start settlementscreen.exe "+to_string(spendtime)+" "+to_string(allsteps)+" "+to_string(winner)+" "+to_string(1+nowtime->tm_mon)+to_string(nowtime->tm_mday)+to_string(nowtime->tm_hour)+to_string(nowtime->tm_min)+to_string(nowtime->tm_sec);
	system(com.c_str());
}
