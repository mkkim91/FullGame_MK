/***********************************************************/
// 다음은 사용자 함수를 호출하는 루틴임 지우지 말것!
/***********************************************************/

#include "device_driver.h"
#include "macro.h"

#define printf 	Uart_Printf

void User_Main(void);

void Main(void)
{
	MMU_Init();
	Led_Init();
	Key_Poll_Init();
	Uart_Init(115200);
	Timer0_Init();
	Timer3_Buzzer_Init();
	Timer4_Init();
	ADC_Init();
	Nand_Init();
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	Uart_Printf("Welcome GBOX World!\n");
	User_Main();
	Uart_Printf("Good Bye~\n");
}

/*===== << 여기부터 사용자 프로그램을 작성함 >> =====*/

#include "./Image/img01.h"
#include "./Image/img02.h"
#include "./Image/img03.h"
#include "./Image/img04.h"
#include "./Image/img05.h"
//#include "./Image/shape.h"

//im

#include "./Image/titlescreen.h"
#include "./Image/opening1.h"
#include "./Image/opening2.h"
#include "./Image/stageclearoh.h"
#include "./Image/gamewin.h"
#include "./Image/gameover.h"

#include "./Image/level1.h"
#include "./Image/level2.h"
#include "./Image/level3.h"
#include "./Image/level4.h"
#include "./Image/level5.h"

#include "./Image/Nanaup.h"
#include "./Image/Nanadown.h"
#include "./Image/Nanaleft.h"
#include "./Image/Nanaright.h"
#include "./Image/Nanax.h"

#include "./Image/heartup.h"
#include "./Image/heartdown.h"
#include "./Image/heartleft.h"
#include "./Image/heartright.h"

#include "./Image/coin.h"
#include "./Image/fire.h"

#include "./Image/Mon1.h"
#include "./Image/Mon2.h"
#include "./Image/Mon3.h"
#include "./Image/Mon4.h"



const unsigned short * Disp[] = {img01, img02, img03, img04};


const unsigned short * Nanaim[] = {Nanaleft, Nanaright, Nanaup, Nanadown};
const unsigned short * Heartim[] = {heartleft, heartright, heartup, heartdown};
const unsigned short * Monim[] = {Mon1, Mon2, Mon3, Mon4};
const unsigned short * levelim[] = {level1, level2, level3, level4, level5};

/* 5:5:5:I Color Definition */

#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800


enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
enum note{N16, N8, N4, N2, N1};
const int song[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2},{0xff, 0xff}};


#define BACK_COLOR 	BLACK
#define WIDTH 		320
#define HEIGHT 		240


	///////////PROJECT
#include<stdlib.h>

/////Let's DEFINE

#define MAX_MON_NUM 10
#define MAX_FIRE_NUM 5
#define MAX_COIN_NUM 10
#define MAX_HEART_NUM 5

///////STRUCT DECLARATIONS/////////

//NANA
struct Ob1
{
	int move_flag;
	int now[2];
	int now_ini[2]; //초기
	int now_pre[2]; //X,Y 의 이전 좌표 값 (pre-x, pre-y 값)
	int size[2];
	int move_step;
	int life;
	int dir;
	unsigned short col;
	int img;
	int hit;
};

//MON AND FIRE
struct Ob2
{
	int timer;
	int moveflag;
	int pos[2];
	int ini[2];
	int pre[2];
	int size[2];
	unsigned short color;
	int speed; //same as speed_step
	int movestep;
	int movex;
	int movey;
	int life;
	int col;
	int hit;
};

//HEART
struct Ob3
{
	int timer;
	int moveflag;
	int pos[2];
	int ini[2];
	int pre[2];
	int size[2];
	unsigned short color;
	int speed;
	int movestep;
	int cnt;
	int dir;
	int a;
	int hit;
};


//COIN
struct Ob4
{
	int pos[2];
	int size[2];
	int col;
	int life;
	unsigned short color;
	int hit;
};

/////////STRUCT Initializations

struct Ob1 Nana={1, {100,100 }, {100,100}, {100,100}, {30,30}, 30, 3, 1, RED,0,0 };

struct Ob2 Fire[MAX_FIRE_NUM]={ {0,1, {20,20},{20,20}, {20,20}, {15,15}, BLUE, 3, 10, 1, 1, 1, 0}, {0,1, {30,30},{30,30}, {30,30}, {15,15}, BLUE, 3, 10, 1, 1, 1, 0}, {0,1, {40,40},{40,40}, {40,40}, {15,15}, BLUE, 3, 10, 1, 1, 1, 0}, {0,1, {50,50},{50,50}, {50,50}, {15,15}, BLUE, 3, 10, 1, 1, 1, 0}, {0,1, {60,60},{60,60}, {60,60}, {15,15}, BLUE, 3, 10, 1, 1, 1, 0}    };

struct Ob2 Mon[MAX_MON_NUM]={  {0,0,{30,20},{30,20},{30,20},{20,20},BLACK,4,20,1,1,1,0}, \
		{0,0,{40,30},{40,30},{40,30},{20,20},BLACK,4,20,1,1,1,0}, \
		{0,0,{50,40},{50,40},{50,40},{20,20},BLACK,4,20,1,1,1,0},\
		{0,0,{60,50},{60,50},{60,50},{20,20},BLACK,4,20,1,1,1,0}, \
		{0,0,{70,60},{70,60},{70,60},{20,20},BLACK,4,20,1,1,1,0},\
		{0,0,{80,70},{80,70},{80,70},{20,20},BLACK,4,20,1,1,1,0},\
		{0,0,{90,80},{90,80},{90,80},{20,20},BLACK,4,20,1,1,1,0},\
		{0,0,{100,90},{100,90},{100,90},{20,20},BLACK,4,20,1,1,1,0},\
		{0,0,{110,100},{110,100},{110,100},{20,20},BLACK,4,20,1,1,1,0}, \
		{0, 0,{110,100},{110,100},{110,100},{20,20},BLACK,4,20,1,1,1,0}};

struct Ob3 Heart={  0, 0, {100,100}, {110,120}, {100,120}, {15,15}, BLACK, 3, 15, 0, 1, 1, 0};

struct Ob4 Coin[MAX_COIN_NUM]={  {{40,70},{20,20},1,1,GREEN},\
		{{50,80},{25,25},1,1,GREEN}, \
		{{60,90},{25,25},1,1,GREEN}, \
		{{30,30},{25,25},1,1,GREEN},\
		{{40,50},{25,25},1,1,GREEN}, \
		{{200,100},{25,25},1,1,GREEN},\
		{{280,190},{25,25},1,1,GREEN}, \
		{{150,160},{25,25},1,1,GREEN},\
		{{190,200},{25,25},1,1,GREEN}, \
		{{300,100},{25,25},1,1,GREEN} };


/////////The VARIABLES////////////

int Game_Start_Flag=0; //may not use
int Stage_flag=0; //restart at 6 because there is no stage 6
int Stage_Num=0;

int Game_Over_Disp_Flag=0;
int Main_Disp_Flag=0;
int Game_Win_Disp=0;

//Game Sangtae

int sangtae=0;

//win and lose
int Lose_Flag=0;
int Win_Flag=0;

//shoot counter
int heartcnt=15;

//coin life temp/ mon life temp
int coinlife=2;
int monlife=1;

//score
int score=0;

//image changers
//background
int e=0;


////////////Main Fns////////
//void Game_Manager(void);
void Disp_Manager(void);
void Op_Manager(void);

//////////Sub Fns by Where you use them

//GM sfns
//void Init_Game_Data(void);
void Stage_Ini(void);

//OM sfns

void Op_Nana(void);
void Op_Fire (void);
void Op_Mon (void);
void Op_Heart(void);

//lim
void Op_Nana_Lim (void);
void Op_Fire_Lim (void);
void Op_Mon_Lim (void);
void Op_Heart_Lim (void);


void Op_Mon_Heart(void);
void Op_Fire_Nana(void);
void Op_Mon_Nana(void);
void Op_Coin_Nana(void);

//Op_Win/Lose Calculations
void Win_Calc(void);
void Lose_Calc(void);


//DM sfns

void Disp_Nana (void);
void Disp_Fire (void);
void Disp_Mon (void);
void Disp_Heart (void);
void Disp_Coin (void) ;
void Disp_Stage (void);

void Disp_Lose (void);
void Disp_Win (void);
void Disp_Ini (void);
void Disp_Info(void) ;

void lcddrawbackgr (int x, int y, int x2, int y2, const unsigned short int *fp);



////////////////GAME MANAGER///////////////
#define START 0
#define INPLAY 1
#define NEWSTAGE 2
//#define INPLAY 3
#define WIN 4
#define LOSE 5



#include <stdlib.h>

void User_Main(void)
{
	//Disp_Ini ();
	Timer0_Repeat(20);

	for (;;)
	{
		//if (sangtae==NEWSTAGE)
		//{
			//sangtae=INPLAY;
		//}
		Op_Manager();
		Disp_Manager ();

		//if (sangtae==NEWSTAGE)
		//{
			//sangtae=INPLAY;
		//}

		if (sangtae==WIN)
		{

			//Stage_Num=1;
			//sangtae=NEWSTAGE;
			break;

		}

		if (sangtae==LOSE)
		{
			//Stage_Num=1;
			//sangtae=NEWSTAGE;
			break;
		}
	}

}


void Op_Manager(void)
{
	printf("SANGTAE%d\n", sangtae);
	if (Game_Start_Flag==0)
	{
		if (sangtae==0) Main_Disp_Flag=1;
		else if (sangtae==4) Game_Over_Disp_Flag=1;

		if (Key_Get_Pressed())
		{
			Game_Start_Flag=1;
			sangtae=NEWSTAGE;
			Stage_Num++;
		}
	}

	else
	{
		printf("INI?%d",sangtae);
		if (sangtae==NEWSTAGE) Stage_Ini();

		else if (sangtae==INPLAY)
		{
			if(Timer0_Check_Expired())
			{
				Timer4_Delay(60);
				Heart.timer++;

				Op_Nana();
				Op_Fire ();
				Op_Mon ();
				Op_Heart();
			}

			//Limits
			Op_Nana_Lim ();
			Op_Fire_Lim ();
			Op_Mon_Lim ();
			Op_Heart_Lim ();

			Op_Mon_Heart();
			Op_Fire_Nana();
			Op_Mon_Nana();
			Op_Coin_Nana();

			Win_Calc();
			Lose_Calc();
		}

	}


}

///////////////DISP_MANAGER/////////////////

void Disp_Manager(void)
{
	if (sangtae==NEWSTAGE) Disp_Stage();

	if (Main_Disp_Flag==1) Disp_Ini();
	//else if (Game_Over_Disp_Flag==1) ;

	if (sangtae==INPLAY)
	{
		Disp_Nana ();
		Disp_Fire ();
		Disp_Mon ();
		Disp_Heart ();
		Disp_Coin () ;

	}

	//if (sangtae==NEWSTAGE) Disp_Stage();
	Disp_Info() ;

	if (Game_Over_Disp_Flag==1)Disp_Lose ();
	if (Game_Win_Disp==1)Disp_Win ();

}

void Stage_Ini (void)
{
	int i;
	printf("STAGINI");

	//Nana in
	Stage_Num++;

	monlife=(Stage_Num);
	coinlife=(Stage_Num*2);

	Nana.life=10;

	Nana.move_flag=1;
	Nana.now[0]=170;
	Nana.now[1]=200;
	Nana.now_ini[0]=170;
	Nana.now_ini[1]=200;
	Nana.now_pre[0]=170;
	Nana.now_pre[0]=200;

	//Heart ini

	heartcnt=30;

	Heart.pos[0]=310;
	Heart.pos[1]=230;


	for (i=0; i<Stage_Num; i++)
	{
		Fire[i].timer=0;
		Fire[i].moveflag=1;
		Fire[i].pos[0]=rand()%300;
		Fire[i].pos[1]=rand()%200;
		Fire[i].ini[0]=Fire[i].pos[0];
		Fire[i].ini[1]=Fire[i].pos[1];
		Fire[i].pre[0]=Fire[i].pos[0];
		Fire[i].pre[1]=Fire[i].pos[1];
		Fire[i].speed=rand()%30; //same as speed_step
		Fire[i].movestep=1;
		Fire[i].movex=1;
		Fire[i].movey=1;
		Fire[i].life=1;
		Fire[i].col=0;
		Fire[i].hit=0;

	}

	for (i=0; i<Stage_Num; i++)
	{
		Mon[i].timer=0;
		Mon[i].moveflag=0;
		Mon[i].pos[0]=rand()%300;
		Mon[i].pos[1]=rand()%200;
		Mon[i].ini[0]=Mon[i].pos[0];
		Mon[i].ini[1]=Mon[i].pos[1];
		Mon[i].pre[0]=Mon[i].pos[0];
		Mon[i].pre[1]=Mon[i].pos[1];
		Mon[i].speed=rand()%30; //same as speed_step
		Mon[i].movestep=1;
		Mon[i].movex=1;
		Mon[i].movey=1;
		Mon[i].life=1;
		Mon[i].col=0;
		Mon[i].hit=0;


	}

	for (i=0; i<(Stage_Num*2); i++)
	{
		Coin[i].pos[0]=rand()%300;
		Coin[i].pos[1]=rand()%200;
		Coin[i].col=1;
		Coin[i].life=1;
		Coin[i].hit=0;
	}

}


//////////GAME_MANAGER() SUB FNS


//////Op_Manager() Sub-fns

void Op_Nana(void)
{

	int key;
	key=Key_Get_Pressed();

	//tank control

	if (key==2) //left
	{
		Nana.now_pre[0] = Nana.now[0];
		Nana.now_pre[1] = Nana.now[1];
		Nana.now[0] = Nana.now[0] - Nana.move_step;
		Nana.dir=2;
		Heart.dir=2;
		Nana.move_flag = 1;
		Nana.img=0;
	}

	if(key==4)//right
	{

		Nana.now_pre[0] = Nana.now[0];
		Nana.now_pre[1] = Nana.now[1];
		Nana.now[0] = Nana.now[0] + Nana.move_step;
		Nana.dir=4;
		Heart.dir=4;
		Nana.move_flag = 1;
		Nana.img=1;
	}

	if (key==1) //up
	{
		Nana.now_pre[0] = Nana.now[0];
		Nana.now_pre[1] = Nana.now[1];
		Nana.now[1] = Nana.now[1] - Nana.move_step;
		Nana.dir=1;
		Heart.dir=1;
		Nana.move_flag = 1;
		Nana.img=2;
	}

	if(key==3)//down
	{
		Nana.now_pre[0] = Nana.now[0];
		Nana.now_pre[1] = Nana.now[1];
		Nana.now[1] = Nana.now[1] + Nana.move_step;
		Nana.dir=3;
		Heart.dir=3;
		Nana.move_flag = 1;
		Nana.img=3;
	}


}


void Op_Fire (void)
{
	int i;

	for (i=0; i<Stage_Num; i++)
	{
		Fire[i].timer++;
		if((Fire[i].timer >= Fire[i].speed))
		{
			if(Fire[i].movex==0)
			{
				Fire[i].timer = 0;
				Fire[i].pre[0] = Fire[i].pos[0];
				Fire[i].pos[0] = Fire[i].pos[0] + Fire[i].movestep;
			}
			else
			{
				Fire[i].timer = 0;
				Fire[i].pre[0] = Fire[i].pos[0];
				Fire[i].pos[0] = Fire[i].pos[0] - Fire[i].movestep;

			}

			if(Fire[i].movey==0)
			{
				Fire[i].timer = 0;
				Fire[i].pre[1] = Fire[i].pos[1];
				Fire[i].pos[1] = Fire[i].pos[1] + Fire[i].movestep;

			}
			else
			{
				Fire[i].timer = 0;
				Fire[i].pre[1] = Fire[i].pos[1];
				Fire[i].pos[1] = Fire[i].pos[1] - Fire[i].movestep;
			}

			Fire[i].moveflag = 1;

		}

	}

}


void Op_Mon (void)
{
	int i;
	for (i=0; i<Stage_Num; i++)
	{
		Mon[i].timer++;

		if((Mon[i].timer >= Mon[i].speed)&&(Mon[i].life>0))
		{
			if(Mon[i].movex==0)
			{
				Mon[i].timer = 0;
				Mon[i].pre[0] =Mon[i].pos[0];
				Mon[i].pos[0] = Mon[i].pos[0] + Mon[i].movestep;
			}
			else
			{
				Mon[i].timer = 0;
				Mon[i].pre[0] = Mon[i].pos[0];
				Mon[i].pos[0] = Mon[i].pos[0] - Mon[i].movestep;

			}

			if(Mon[i].movey==0)
			{
				Mon[i].timer = 0;
				Mon[i].pre[1] = Mon[i].pos[1];
				Mon[i].pos[1] = Mon[i].pos[1] + Mon[i].movestep;

			}
			else
			{
				Mon[i].timer = 0;
				Mon[i].pre[1] = Mon[i].pos[1];
				Mon[i].pos[1] = Mon[i].pos[1] - Mon[i].movestep;
			}

			Mon[i].moveflag = 1;

		}

	}

}


void Op_Heart(void)
{

	int key;
	key=Key_Get_Pressed();

	if(key==5) //shoot
	{
		if (Heart.cnt==0)
		{
			heartcnt--;
			Heart.cnt=1;

			if (Heart.dir==1)//UP
			{
				Heart.pos[1]= Nana.now[1] - 10;
				Heart.pos[0]= Nana.now[0] + 3;
				Heart.a=1;
				Heart.hit=2;
				Heart.moveflag=1;
			}
			if (Heart.dir==3)//DO
			{
				Heart.pos[1]= Nana.now[1] + 30;
				Heart.pos[0]= Nana.now[0] + 3;
				Heart.moveflag=1;
				Heart.a=3;
				Heart.hit=3;
				Heart.cnt=1;
			}
			if (Heart.dir==2)//LE
			{
				Heart.pos[1]= Nana.now[1] - 3;
				Heart.pos[0]= Nana.now[0] - 10;
				Heart.moveflag=1;
				Heart.a=2;
				Heart.hit=0;
				Heart.cnt=1;
			}
			if (Heart.dir==4)//RI
			{
				Heart.pos[1]= Nana.now[1] + 3;
				Heart.pos[0]= Nana.now[0] + 30;
				Heart.moveflag=1;
				Heart.a=4;
				Heart.hit=1;
				Heart.cnt=1;
			}
		}
	}

	if((Heart.timer>=Heart.speed)&&(Heart.cnt!=0))
	{
		if ((Heart.a==1))
		{
			Heart.pre[0]= Heart.pos[0];
			Heart.pre[1]= Heart.pos[1];
			Heart.pos[1] = Heart.pos[1] - Heart.movestep;
		}

		if ((Heart.a==3))
		{
			Heart.pre[0]=Heart.pos[0];
			Heart.pre[1]=Heart.pos[1];
			Heart.pos[1] = Heart.pos[1] + Heart.movestep;
		}
		if ((Heart.a==2))
		{
			Heart.pre[0]=Heart.pos[0];
			Heart.pre[1]=Heart.pos[1];
			Heart.pos[0] = Heart.pos[0] - Heart.movestep;
		}
		if((Heart.a==4))
		{
			//Heart.timer = 0;
			Heart.pre[0]=Heart.pos[0];
			Heart.pre[1]=Heart.pos[1];
			Heart.pos[0] = Heart.pos[0] + Heart.movestep;
		}

		Heart.moveflag=1;
	}

}


///LIM
void Op_Nana_Lim (void)
{
	if((Nana.now[0] >= 310))
	{
		Nana.now[0] = Nana.now[0]-30;
	}
	else if((Nana.now[0] <= 0))
	{
		Nana.now[0] = 0;
	}

	if((Nana.now[1] >= 220))
	{
		Nana.now[1] = Nana.now[1]-Nana.size[1];
	}
	else if((Nana.now[1] <= Nana.size[1]))
	{
		Nana.now[1] = Nana.size[1];
	}

}


void Op_Fire_Lim (void)
{
	int i;

	for (i=0; i<Stage_Num; i++)
	{
		if((Fire[i].pos[0] >= WIDTH-45))
		{
			Fire[i].movex=1;
			Fire[i].speed=rand()%8;
			Fire[i].col++;
			if (Fire[i].col>=4) Fire[i].col=0;
		}
		else if((Fire[i].pos[0] <= 0))
		{
			Fire[i].movex=0;
			Fire[i].speed=rand()%10;
			Fire[i].col++;
			if (Fire[i].col>=4) Fire[i].col=0;
		}

		if((Fire[i].pos[1] >= HEIGHT-30))
		{
			Fire[i].movey=1;
			Fire[i].speed=rand()%7;
			Fire[i].col++;
			if (Fire[i].col>=4) Fire[i].col=0;
		}
		else if((Fire[i].pos[1] <= 40))
		{
			Fire[i].movey=0;
			Fire[i].speed=rand()%9;
			Fire[i].col++;
			if (Fire[i].col>=4) Fire[i].col=0;
		}
	}

}

void Op_Mon_Lim (void)
{
	int i;

	for (i=0; i<Stage_Num; i++)
	{
		if((Mon[i].pos[0] >= WIDTH))
		{
			Mon[i].movex=1;
			Mon[i].speed=rand()%9;
			Mon[i].col++;
			if (Mon[i].col>=4) Mon[i].col=0;
			//lcddrawbackgr(Mon[i].pre[0], Mon[i].pre[1], Mon[i].pre[0] + Mon[i].size[0], Mon[i].pre[1] + Mon[i].size[1], levelim[e]);
			Mon[i].moveflag=0;
		}
		else if((Mon[i].pos[0] <= 10))
		{
			Mon[i].movex=0;
			Mon[i].speed=rand()%8;
			Mon[i].col++;
			if (Mon[i].col>=4) Mon[i].col=0;
			//lcddrawbackgr(Mon[i].pre[0], Mon[i].pre[1], Mon[i].pre[0] + Mon[i].size[0], Mon[i].pre[1] + Mon[i].size[1], levelim[e]);
		}

		if((Mon[i].pos[1] >= HEIGHT-10))
		{
			Mon[i].movey=1;
			Mon[i].speed=rand()%7;
			Mon[i].col++;
			if (Fire[i].col>=4) Mon[i].col=0;
			//lcddrawbackgr(Mon[i].pre[0], Mon[i].pre[1], Mon[i].pre[0] + Mon[i].size[0], Mon[i].pre[1] + Mon[i].size[1], levelim[e]);
		}
		else if((Mon[i].pos[1] <= 50))
		{
			Mon[i].movey=0;
			Mon[i].speed=rand()%9;
			Mon[i].col++;
			if (Mon[i].col>=4) Mon[i].col=0;
			//lcddrawbackgr(Mon[i].pre[0], Mon[i].pre[1], Mon[i].pre[0] + Mon[i].size[0], Mon[i].pre[1] + Mon[i].size[1], levelim[e]);
		}
	}

}

void Op_Heart_Lim (void)
{
	if(Heart.pos[1]<=25) // x-position is important, too
	{
		Heart.cnt=0;
		Heart.moveflag=0;
		lcddrawbackgr(Heart.pre[0], Heart.pre[1], Heart.pre[0] + Heart.size[0], Heart.pre[1] + Heart.size[1], levelim[e]);
	}

	else if(Heart.pos[1]>=220 )
	{
		Heart.cnt=0;
		Heart.moveflag=0;
		lcddrawbackgr(Heart.pre[0], Heart.pre[1], Heart.pre[0] + Heart.size[0], Heart.pre[1] + Heart.size[1], levelim[e]);
	}

	if (Heart.pos[0]<=25)
	{
		Heart.cnt=0;
		Heart.moveflag=0;
		lcddrawbackgr(Heart.pre[0], Heart.pre[1], Heart.pre[0] + Heart.size[0], Heart.pre[1] + Heart.size[1], levelim[e]);

	}
	else if (Heart.pos[0]>=300)
	{
		Heart.cnt=0;
		Heart.moveflag=0;
		lcddrawbackgr(Heart.pre[0], Heart.pre[1], Heart.pre[0] + Heart.size[0], Heart.pre[1] + Heart.size[1], levelim[e]);
	}
}


///end LIM

//CRASH
void Op_Mon_Heart(void)
{
	int i;
	for(i=0; i<Stage_Num; i++)
	{
		if(
			(((Heart.pos[0]>=Mon[i].pos[0])&&(Heart.pos[0]<=(Mon[i].pos[0]+Mon[i].size[0])))&&((Heart.pos[1]>=Mon[i].pos[1])&&(Heart.pos[1]<=(Mon[i].pos[1]+Mon[i].size[1]))))||
			((((Heart.pos[0]+Heart.size[0])>=Mon[i].pos[0])&&((Heart.pos[0]+Heart.size[0])<=(Mon[i].pos[0]+Mon[i].size[0])))&&((Heart.pos[1]>=Mon[i].pos[1])&&(Heart.pos[1]<=(Mon[i].pos[1]+Mon[i].size[1]))))||
			((((Heart.pos[0]+Heart.size[0])>=Mon[i].pos[0])&&((Heart.pos[0]+Heart.size[0])<=(Mon[i].pos[0]+Mon[i].size[0])))&&(((Heart.pos[1]+Heart.size[1])>=Mon[i].pos[1])&&((Heart.pos[1]+Heart.size[1])<=(Mon[i].pos[1]+Mon[i].size[1]))))||
			(((Heart.pos[0]>=Mon[i].pos[0])&&(Heart.pos[0]<=(Mon[i].pos[0]+Mon[i].size[0])))&&(((Heart.pos[1]+Heart.size[1])>=Mon[i].pos[1])&&((Heart.pos[1]+Heart.size[1])<=(Mon[i].pos[1]+Mon[i].size[1]))))
		)
		{
			Mon[i].life--;
			monlife--;
			score=(200*Stage_Num);
			lcddrawbackgr(Mon[i].pos[0], Mon[i].pos[1], Mon[i].pos[0] + Mon[i].size[0], Mon[i].pos[1] + Mon[i].size[1], levelim[e]);
			Mon[i].pos[0]=-100;
			Mon[i].pos[1]=-100;
		}

	}

}


void Op_Fire_Nana(void)
{
	int i;
	for(i=0; i<Stage_Num; i++)
	{
		if(
			(((Nana.now[0]>=Fire[i].pos[0])&&(Nana.now[0]<=(Fire[i].pos[0]+Fire[i].size[0])))&&((Nana.now[1]>=Fire[i].pos[1])&&(Nana.now[1]<=(Fire[i].pos[1]+Fire[i].size[1]))))||
			((((Nana.now[0]+Nana.size[0])>=Fire[i].pos[0])&&((Nana.now[0]+Nana.size[0])<=(Fire[i].pos[0]+Fire[i].size[0])))&&((Nana.now[1]>=Fire[i].pos[1])&&(Nana.now[1]<=(Fire[i].pos[1]+Fire[i].size[1]))))||
			((((Nana.now[0]+Nana.size[0])>=Fire[i].pos[0])&&((Nana.now[0]+Nana.size[0])<=(Fire[i].pos[0]+Fire[i].size[0])))&&(((Nana.now[1]+Nana.size[1])>=Fire[i].pos[1])&&((Nana.now[1]+Nana.size[1])<=(Fire[i].pos[1]+Fire[i].size[1]))))||
			(((Nana.now[0]>=Fire[i].pos[0])&&(Nana.now[0]<=(Fire[i].pos[0]+Fire[i].size[0])))&&(((Nana.now[1]+Nana.size[1])>=Fire[i].pos[1])&&((Nana.now[1]+Nana.size[1])<=(Fire[i].pos[1]+Fire[i].size[1]))))
		)
		{
			Nana.life--;
			lcddrawbackgr(Nana.now[0], Nana.now[1], Nana.now[0] + Nana.size[0], Nana.now[1] + Nana.size[1], levelim[e]);
			Nana.now[0]=160;
			Nana.now[1]=200;
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Nana.now[0], Nana.now[1], Nanax);
			Lcd_Set_Shape_Mode(0,0);

			lcddrawbackgr(Fire[i].pos[0], Fire[i].pos[1], Fire[i].pos[0] + Fire[i].size[0], Fire[i].pos[1] + Fire[i].size[1], levelim[e]);
			Fire[i].pos[0]=rand()%200;
			Fire[i].pos[1]=rand()%100;
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Fire[i].pos[0], Fire[i].pos[1], fire);
			Lcd_Set_Shape_Mode(0,0);
		}
	}

}


void Op_Mon_Nana(void)
{
	int i;
	for(i=0; i<Stage_Num; i++)
	{
		if(
			(((Nana.now[0]>=Mon[i].pos[0])&&(Nana.now[0]<=(Mon[i].pos[0]+Mon[i].size[0])))&&((Nana.now[1]>=Mon[i].pos[1])&&(Nana.now[1]<=(Mon[i].pos[1]+Mon[i].size[1]))))||
			((((Nana.now[0]+Nana.size[0])>=Mon[i].pos[0])&&((Nana.now[0]+Nana.size[0])<=(Mon[i].pos[0]+Mon[i].size[0])))&&((Nana.now[1]>=Mon[i].pos[1])&&(Nana.now[1]<=(Mon[i].pos[1]+Mon[i].size[1]))))||
			((((Nana.now[0]+Nana.size[0])>=Mon[i].pos[0])&&((Nana.now[0]+Nana.size[0])<=(Mon[i].pos[0]+Mon[i].size[0])))&&(((Nana.now[1]+Nana.size[1])>=Mon[i].pos[1])&&((Nana.now[1]+Nana.size[1])<=(Mon[i].pos[1]+Mon[i].size[1]))))||
			(((Nana.now[0]>=Mon[i].pos[0])&&(Nana.now[0]<=(Mon[i].pos[0]+Mon[i].size[0])))&&(((Nana.now[1]+Nana.size[1])>=Mon[i].pos[1])&&((Nana.now[1]+Nana.size[1])<=(Mon[i].pos[1]+Mon[i].size[1]))))
		)
		{
			Nana.life--;
			lcddrawbackgr(Nana.now[0], Nana.now[1], Nana.now[0] + Nana.size[0], Nana.now[1] + Nana.size[1], levelim[e]);
			Nana.now[0]=160;
			Nana.now[1]=200;
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Nana.now[0], Nana.now[1], Nanax );

			lcddrawbackgr(Mon[i].pos[0], Mon[i].pos[1], Mon[i].pos[0] + Mon[i].size[0], Mon[i].pos[1] + Mon[i].size[1], levelim[e]);
			Mon[i].pos[0]=rand()%200;
			Mon[i].pos[1]=rand()%100;
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Mon[i].pos[0], Mon[i].pos[1],Monim[Mon[i].col]);
			Lcd_Set_Shape_Mode(0,0);
		}
	}
}


void Op_Coin_Nana(void)
{
	int i;
	for(i=0; i<(Stage_Num*2); i++)
	{
		if(
			(((Nana.now[0]>=Coin[i].pos[0])&&(Nana.now[0]<=(Coin[i].pos[0]+Coin[i].size[0])))&&((Nana.now[1]>=Coin[i].pos[1])&&(Nana.now[1]<=(Coin[i].pos[1]+Coin[i].size[1]))))||
			((((Nana.now[0]+Nana.size[0])>=Coin[i].pos[0])&&((Nana.now[0]+Nana.size[0])<=(Coin[i].pos[0]+Coin[i].size[0])))&&((Nana.now[1]>=Coin[i].pos[1])&&(Nana.now[1]<=(Coin[i].pos[1]+Coin[i].size[1]))))||
			((((Nana.now[0]+Nana.size[0])>=Coin[i].pos[0])&&((Nana.now[0]+Nana.size[0])<=(Coin[i].pos[0]+Coin[i].size[0])))&&(((Nana.now[1]+Nana.size[1])>=Coin[i].pos[1])&&((Nana.now[1]+Nana.size[1])<=(Coin[i].pos[1]+Coin[i].size[1]))))||
			(((Nana.now[0]>=Coin[i].pos[0])&&(Nana.now[0]<=(Coin[i].pos[0]+Coin[i].size[0])))&&(((Nana.now[1]+Nana.size[1])>=Coin[i].pos[1])&&((Nana.now[1]+Nana.size[1])<=(Coin[i].pos[1]+Coin[i].size[1]))))
		)
		{
			Coin[i].life--;
			coinlife--;
			score+=(100*Stage_Num);
			lcddrawbackgr(Coin[i].pos[0], Coin[i].pos[1], Coin[i].pos[0] + Coin[i].size[0], Coin[i].pos[1] + Coin[i].size[1], levelim[e]);
			Coin[i].pos[0]=-300;
			Coin[i].pos[1]=-300;
		}

	}

}
//End Crash

//Win/Lose Calc

void Win_Calc(void)
{
	int i;

	if ((coinlife<=0)||(monlife<=0))
	{
		for (i=0; i<(Stage_Num*2); i++ )
		{
			Coin[i].life=0;
			Fire[i].moveflag=0;
			Mon[i].moveflag=0;
		}
		if (Stage_Num==5) Win_Flag=1;
		else sangtae=WIN;

	}
}


void Lose_Calc(void)
{
	if (Nana.life<=0)
	{
		Lose_Flag=1;
		sangtae=LOSE;
	}
}



//////////////////////// Disp_Man Sub fns

void Disp_Nana (void)
{
	if(Nana.move_flag != 0)
	{
		lcddrawbackgr(Nana.now_pre[0], Nana.now_pre[1], Nana.now_pre[0] + Nana.size[0], Nana.now_pre[1] + Nana.size[1], levelim[e]);
		Lcd_Set_Shape_Mode(1,WHITE);
		Lcd_Draw_BMP(Nana.now[0], Nana.now[1], Nanaim[Nana.img]);
		Lcd_Set_Shape_Mode(0,0);
		Nana.move_flag = 0;
	}
}


void Disp_Fire (void)
{
	int i;

	for (i=0; i<Stage_Num; i++)
	{
		if (Fire[i].moveflag !=0)
		{
			lcddrawbackgr(Fire[i].pre[0], Fire[i].pre[1], Fire[i].pre[0] + Fire[i].size[0], Fire[i].pre[1] + Fire[i].size[1], levelim[e]);
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Fire[i].pos[0], Fire[i].pos[1], fire);
			Lcd_Set_Shape_Mode(0,0);
			Fire[i].moveflag = 0;
		}
	}
}


void Disp_Mon (void)
{
	int i;

	for (i=0; i<Stage_Num; i++)
	{
		if(Mon[i].moveflag !=0)
		{
			lcddrawbackgr(Mon[i].pre[0], Mon[i].pre[1], Mon[i].pre[0] + Mon[i].size[0], Mon[i].pre[1] + Mon[i].size[1], levelim[e]);
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Mon[i].pos[0], Mon[i].pos[1], Monim[Mon[i].col]);
			Lcd_Set_Shape_Mode(0,0);
			Mon[i].moveflag = 0;
		}
		if (Mon[i].life <= 0)
		{
			lcddrawbackgr(Mon[i].pre[0], Mon[i].pre[1], Mon[i].pre[0] + Mon[i].size[0], Mon[i].pre[1] + Mon[i].size[1], levelim[e]);
			Mon[i].pos[0]=-250;
			Mon[i].pos[1]=-250;
		}
	}

}


void Disp_Heart (void)
{
	if((Heart.moveflag != 0) && (Heart.cnt !=0))
	{
		lcddrawbackgr(Heart.pre[0], Heart.pre[1], Heart.pre[0] + Heart.size[0], Heart.pre[1] + Heart.size[1], levelim[e]);
		Lcd_Set_Shape_Mode(1,WHITE);
		Lcd_Draw_BMP(Heart.pos[0], Heart.pos[1], Heartim[Heart.hit]);
		Lcd_Set_Shape_Mode(0,0);
		Heart.moveflag = 0;
	}
}


void Disp_Coin (void)
{
	int i;

	for(i=0; i<(Stage_Num*2); i++)
	{
		if(Coin[i].life>0)
		{
			Lcd_Set_Shape_Mode(1,WHITE);
			Lcd_Draw_BMP(Coin[i].pos[0], Coin[i].pos[1], coin);
			Lcd_Set_Shape_Mode(0,0);
		}
		else if(Coin[i].life<=0)
		{
			lcddrawbackgr(Coin[i].pos[0], Coin[i].pos[1], Coin[i].pos[0] + Coin[i].size[0], Coin[i].pos[1] + Coin[i].size[1], levelim[e]);
			Coin[i].pos[0]=-300;
			Coin[i].pos[1]=-300;
		}
	}

}


void Disp_Stage (void)
{
	if (sangtae==NEWSTAGE)
	{
		Lcd_Draw_BMP(0,0, levelim[e]);
		Disp_Nana();
		Disp_Fire();
		Disp_Mon();
		Disp_Coin();
		sangtae=INPLAY;
	}
	sangtae=INPLAY;
}

void Disp_Lose (void)
{
	//Lcd_Printf(100,100,RED,BLUE,1,1, "YOU LOSE");
	Lcd_Draw_BMP(0,0, gameover);
	Timer3_Buzzer_Beep(E1,N4);
	Timer3_Buzzer_Beep(D1,N4);
	Timer3_Buzzer_Beep(C1,N4);
	Timer4_Delay(900);
}


void Disp_Win (void)
{
	//Lcd_Printf(100,100,RED,BLUE,1,1, "STAGE %d WIN", Stage_Num);
	Lcd_Draw_BMP(0,0, gamewin);
	Timer3_Buzzer_Beep(C1,N16);
	Timer3_Buzzer_Beep(D1,N16);
	Timer3_Buzzer_Beep(E1,N16);
	Timer4_Delay(1000);
}


void Disp_Ini (void)
{
//Include some opening screen
	Lcd_Draw_BMP(0,0, titlescreen);
	Timer4_Delay(1000);
	Lcd_Draw_BMP(0,0, opening1);
	Timer4_Delay(2000);
	Lcd_Draw_BMP(0,0, opening2);
	Main_Disp_Flag=0;

}


void Disp_Info(void)
{

	Lcd_Printf(1,1,RED,BLUE,1,1, "ST %d", Stage_Num);
	Lcd_Printf(270,1,RED,BLUE,1,1, "LIFE %d", Nana.life);
	Lcd_Printf(180,2,RED,BLUE,1,1, "SHOT %d", heartcnt);
	Lcd_Printf(70,2,RED,BLUE,1,1, "SCORE %d", score);

}

void lcddrawbackgr (int x, int y, int x2, int y2, const unsigned short int *fp)
{
	register int width = fp[0];
	register int xx, yy;
	for (yy=y; yy<=y2; yy++)
	{
		for (xx=x; xx<=x2;xx++)
		{
			Lcd_Put_Pixel(xx,yy,(int)fp[yy*width+xx+2]);

		}
	}

}



