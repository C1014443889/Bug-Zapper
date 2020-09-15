// Includes, namespace and prototypes
#include "template.h"
using namespace AGK;
app App;

//注意 精灵索引以及创建精灵 是有顺序的
//每次刷新纪录都会显示画面祝贺


//函数声明
void generateNewBug();
void displayIntro();
void hideIntro();
void playGame();
void closingScreen();
void checkHighScore();//检查最高分
void newHighScore();//更新记录

//分辨率设置
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

//精灵的索引（有顺序的，从1开始）
const int BUG_INDEX=3;
const int INTRO_INDEX=1;
const int GRASS_INDEX=2;

//声音的索引
const int SOUND_INDEX=1;

//定时最大值
const int MAX_TIME=10;

//动画帧
const int FRAME_WIDTH=64;
const int FRAME_HEIGTH=64;
const int FRAME_COUNT=2;

//消灭的虫子数
int bug_count=0;


/***************************/
//之前的最高分
int prev_HighScore=0;
//最高分数记录文本编号
const int HIGH_SCORE_FILE=1;
/***************************/


//游戏的状态
const int GAME_JUST_START=1;
const int GAME_IN_PLAY=2;
/***************************/
const int GAME_HIGH_CHECK=3;//游戏最高分检查
/***************************/
const int GAME_OVER=4;



int game_state=GAME_JUST_START;


// Begin app, called once at the start
void app::Begin( void )
{
	//窗口标题设置
	agk::SetWindowTitle("Bug Zapper");
	//屏幕设置
	agk::SetVirtualResolution(SCREEN_WIDTH,SCREEN_HEIGHT);

	//创建精灵(虫子、草地、背景)   有顺序，会覆盖
	agk::CreateSprite(GRASS_INDEX,"BugZapper/Grass.png");
	agk::CreateSprite(BUG_INDEX,"BugZapper/Bug.png");
	agk::CreateSprite(INTRO_INDEX,"BugZapper/Intro.png");

	//加载声音文件
	agk::LoadSound(SOUND_INDEX,"BugZapper/boink.wav");

	//随机位置产生虫子
	generateNewBug();

	//设置虫子动画
	agk::SetSpriteAnimation(BUG_INDEX,FRAME_WIDTH,FRAME_HEIGTH,FRAME_COUNT);

	//播放虫子动画
	agk::PlaySprite(BUG_INDEX);
}

// Main loop, called every frame
void app::Loop ( void )
{
	//判断游戏的状态
	switch(game_state)
	{
	case GAME_JUST_START:
		displayIntro();//显示介绍界面

		if(agk::GetRawLastKey())//是否有按键按下
		{
			hideIntro();//隐藏介绍界面

			game_state=GAME_IN_PLAY;//改变游戏状态

			agk::ResetTimer();//重置定时器
		}
		break;

	case GAME_IN_PLAY:
		playGame();//开始玩游戏
		break;

	case GAME_HIGH_CHECK:
		checkHighScore();
		break;

	case GAME_OVER:
		closingScreen();//关闭屏幕
		break;
	} 

	//更新屏幕
	agk::Sync();
}

// Called when the app ends
void app::End ( void )
{
}


//在随机位置产生虫子
void generateNewBug()
{
	//获取虫子的宽和高
	float bugWidth=agk::GetSpriteWidth(BUG_INDEX);
	float bugHeigth=agk::GetSpriteHeight(BUG_INDEX);

	//随机产生虫子位置
	int x=agk::Random(0,SCREEN_WIDTH-bugWidth);
	int y=agk::Random(0,SCREEN_HEIGHT-bugHeigth);

	//设置虫子的位置
	agk::SetSpritePosition(BUG_INDEX,x,y);
}

//显示介绍界面
void displayIntro()
{
	//虫子和草地不可见，介绍界面可见
	agk::SetSpriteVisible(GRASS_INDEX,0);
	agk::SetSpriteVisible(BUG_INDEX,0);

	agk::SetSpriteVisible(INTRO_INDEX,1);
}

//玩游戏
void playGame()
{
	float mouseX,mouseY;

	//判断游戏时间是否足够
	if(agk::Timer() < MAX_TIME)
	{
		if(agk::GetRawMouseLeftPressed())//鼠标左键是否按下
		{
			mouseX=agk::GetRawMouseX();//获取当前鼠标的位置
			mouseY=agk::GetRawMouseY();

			if(agk::GetSpriteHit(mouseX,mouseY) == BUG_INDEX)//是否点击虫子
			{
				agk::PlaySound(SOUND_INDEX);//播放声音

			    bug_count++;//虫子数加一

				generateNewBug();//在随机位置产生新虫子
			}
		}
	}
	else//时间结束
	{
		game_state=GAME_HIGH_CHECK;//改变游戏的状态
	}
}

//检查最高分
void checkHighScore()
{
	//获取上一次的记录
	
	if(agk::GetFileExists("BugZapperHighScore.dat"))//存储最高分的文件存在
	{
		//打开文件
		agk::OpenToRead(HIGH_SCORE_FILE,"BugZapperHighScore.dat");

		//读取最高分
		prev_HighScore=agk::ReadInteger(HIGH_SCORE_FILE);
		//关闭文件
		agk::CloseFile(HIGH_SCORE_FILE);
	}
	else//文件不存在
	{
		prev_HighScore=0;
	}

	//比较之前和现在的
	if(bug_count > prev_HighScore)//当前消灭的虫子数大于之前的
	{
		newHighScore();//更新记录

	}

	//改变游戏的状态

	game_state=GAME_OVER;
}

//更新记录（向文件中写入数据，覆盖之前的数据）
void newHighScore()
{
	agk::OpenToWrite(HIGH_SCORE_FILE,"BugZapperHighScore.dat");//打开文件

	agk::WriteInteger(HIGH_SCORE_FILE,bug_count);//写入文件

	agk::CloseFile(HIGH_SCORE_FILE);//关闭文件
}



//隐藏介绍界面
void hideIntro()
{
	//虫子和草地可见，介绍界面不可见
	agk::SetSpriteVisible(INTRO_INDEX,0);

	agk::SetSpriteVisible(BUG_INDEX,1);
	agk::SetSpriteVisible(GRASS_INDEX,1);
	
}

//关闭屏幕
void closingScreen()
{
	//隐藏草地和虫子
	agk::SetSpriteVisible(BUG_INDEX,0);
	agk::SetSpriteVisible(GRASS_INDEX,0);

	//打印出消灭的虫子数
	agk::PrintC("Number of bugs zapped:");
	agk::Print(bug_count);

	//如果打破记录，显示祝贺
	if(bug_count > prev_HighScore)
	{
		agk::Print("Congratulations!");
		agk::Print("You set a new high score!");
	}
}
