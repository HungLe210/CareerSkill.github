#include<iostream>
#include<vector>
#include<conio.h>
#include<time.h>
#include<synchapi.h>
#include<algorithm>
#include <windows.h>
using namespace std;
namespace consoleforeground
{
	enum {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
}
namespace consolebackground
{
	enum {
		BLACK = 0,
		DARKBLUE = BACKGROUND_BLUE,
		DARKGREEN = BACKGROUND_GREEN,
		DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
		DARKRED = BACKGROUND_RED,
		DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
		DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
		DARKGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		GRAY = BACKGROUND_INTENSITY,
		BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
		RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
		MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
		YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	};
}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void setConsoleColour(unsigned short colour)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	SetConsoleTextAttribute(hOut, colour);
}
void setcursor(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}
vector<int> snake;
bool food_eaten = 1, colour = 1, kids = 0, self_hit = 1, reverse_snake = 0;
int width = 27, lenght = 118, food_x = -1, food_y = -2, hsc, time1 = 50;

void initialise_snake() {
	snake.clear();
	for (int i = 0;i < 2;i++) {
		snake.push_back(0);
		snake.push_back(i);
	}
}
void print_title(){
	setcursor(0,0);
	setConsoleColour(consoleforeground::DARKYELLOW);
	cout<<"YOUR SCORE:                     ";
	if(time1==20)		cout<<"HIGH";
	else if(time1==50)	cout<<"MEDIUM";
	else if(time1==80)	cout<<"LOW";
	else 				{
		cout<<"Custom: ";
		setConsoleColour(consoleforeground::WHITE);
		cout<<time1<<" ms";
	}
	cout<<"\n";
	if(colour){
		setConsoleColour(consolebackground::DARKGRAY);
		for(int i=0;i<lenght+2;i++)
		cout<<" ";
		cout<<"\n";
		for(int i=0;i<width;i++){
			for(int j=0;j<lenght+2;j++){
				if(j==0  || j==lenght+1)	setConsoleColour(consolebackground::DARKGRAY);
				else	 					setConsoleColour(consolebackground::BLACK);
				cout<<" ";
			}
			cout<<"\n";
		}
		setConsoleColour(consolebackground::DARKGRAY);
		for(int i=0;i<lenght+2;i++)
		cout<<" ";
	}
	else{
		setConsoleColour(consoleforeground::DARKGRAY);
		for(int i=0;i<lenght+2;i++)
		cout<<"5";
		cout<<"\n";
		for(int i=0;i<width;i++){
			for(int j=0;j<lenght+2;j++)
				if(j==0)				cout<<"5";
				else if(j==lenght+1)	cout<<"5";
				else 					cout<<" ";	
			cout<<"\n";
		}
		for(int i=0;i<lenght+2;i++)
		cout<<"5";
	}
}
void print_highscore(){
	int i,j;
	setcursor(11,0);
	setConsoleColour(consoleforeground::WHITE);
	cout<<snake.size()/2-3<<"  ";
}
void move_snake(int move,char direction){
	int i,j;
	if(direction=='v'){
		snake.push_back(snake[snake.end()-2-snake.begin()]+move);
		snake.push_back(snake[snake.end()-2-snake.begin()]);
	}
	else{
		snake.push_back(snake[snake.end()-2-snake.begin()]);
		snake.push_back(snake[snake.end()-2-snake.begin()]+move);
	}
	setcursor(snake[1]+1,snake[0]+2);
	if(colour) 				setConsoleColour(consolebackground::BLACK);
	cout<<" ";
	snake.erase(snake.begin());
	snake.erase(snake.begin());
	if(snake.back()==lenght)										snake.back()-=lenght;	
	else if(snake.back()==-1)										snake.back()+=lenght;
	else if(snake[snake.end()-snake.begin()-2]==-1)					snake[snake.end()-snake.begin()-2]+=width;
	else if(snake[snake.end()-snake.begin()-2]==width)				snake[snake.end()-snake.begin()-2]-=width;

	setcursor(snake.back()+1,snake[snake.end()-snake.begin()-2]+2);
	if(colour){
		setConsoleColour(consolebackground::BLUE);
		cout<<" ";
	}
	else{
		setConsoleColour(consoleforeground::BLUE);
		cout<<"O";
	}
	setcursor(snake[snake.end()-snake.begin()-3]+1,snake[snake.end()-snake.begin()-4]+2); 
	if(colour){
		setConsoleColour(consolebackground::BLUE);
		cout<<" ";
	}
	else{
		setConsoleColour(consoleforeground::BLUE);
		cout<<"o";
	}
}
void eat_food(){
	food_x=rand()%(width);
	food_y=rand()%(lenght);
	setcursor(food_y+1,food_x+2);
	if(colour){
		setConsoleColour(consolebackground::BLUE);
		cout<<" ";
	}
	else{
		setConsoleColour(consoleforeground::BLUE);
		cout<<"o";
	} 
	snake.insert(snake.begin(),snake[1]);
	snake.insert(snake.begin(),snake[1]);
}
int print_final_message(){
	setConsoleColour(consoleforeground::BLACK);
	system("cls");
	setConsoleColour(consoleforeground::RED);
	int a=snake.size()/2-3,k;
	if(a>hsc)
	hsc=a;
	cout<<"\n\n\n\n\n               GAME OVER\n";
	cout<<"               YOUR SCORE: "<<snake.size()/2-3<<endl;
	cout<<"               HIGH SCORE EVER: "<<hsc;
	cout<<endl<<endl<<endl;
	cout<<"               PRESS ANY KEY TO RESTART\n";
	cout<<"               Press S to open settings\n";
	cout<<"               esc to EXIT\n\n\n\n\n\n";
	a=getch();
	return a;
}
bool check_interbody_death(){
	int x,y,i;
	bool j=0;
	x=snake[snake.end()-snake.begin()-2];
	y=snake.back();						 
	for(i=0;i<snake.size()-2;i=i+2){	 
		if(x==snake[i] && y==snake[i+1]){
			j=1;
			break;
		}
	}
	return j;
}
bool check_interbody_death() {//to check if snake died by touching its own body or not
	int x, y, i;
	bool j = 0;
	x = snake[snake.end() - snake.begin() - 2];//x cordinate of snake mouth
	y = snake.back();						 //y cordinate of snake mouth
	for (i = 0; i < snake.size() - 2; i = i + 2) {	 //checking with body array
		if (x == snake[i] && y == snake[i + 1]) {
			j = 1;
			break;
		}
	}
	return j;
}
