// Includes, namespace and prototypes
#include "template.h"
using namespace AGK;
app App;

//ע�� ���������Լ��������� ����˳���
//ÿ��ˢ�¼�¼������ʾ����ף��


//��������
void generateNewBug();
void displayIntro();
void hideIntro();
void playGame();
void closingScreen();
void checkHighScore();//�����߷�
void newHighScore();//���¼�¼

//�ֱ�������
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

//�������������˳��ģ���1��ʼ��
const int BUG_INDEX=3;
const int INTRO_INDEX=1;
const int GRASS_INDEX=2;

//����������
const int SOUND_INDEX=1;

//��ʱ���ֵ
const int MAX_TIME=10;

//����֡
const int FRAME_WIDTH=64;
const int FRAME_HEIGTH=64;
const int FRAME_COUNT=2;

//����ĳ�����
int bug_count=0;


/***************************/
//֮ǰ����߷�
int prev_HighScore=0;
//��߷�����¼�ı����
const int HIGH_SCORE_FILE=1;
/***************************/


//��Ϸ��״̬
const int GAME_JUST_START=1;
const int GAME_IN_PLAY=2;
/***************************/
const int GAME_HIGH_CHECK=3;//��Ϸ��߷ּ��
/***************************/
const int GAME_OVER=4;



int game_state=GAME_JUST_START;


// Begin app, called once at the start
void app::Begin( void )
{
	//���ڱ�������
	agk::SetWindowTitle("Bug Zapper");
	//��Ļ����
	agk::SetVirtualResolution(SCREEN_WIDTH,SCREEN_HEIGHT);

	//��������(���ӡ��ݵء�����)   ��˳�򣬻Ḳ��
	agk::CreateSprite(GRASS_INDEX,"BugZapper/Grass.png");
	agk::CreateSprite(BUG_INDEX,"BugZapper/Bug.png");
	agk::CreateSprite(INTRO_INDEX,"BugZapper/Intro.png");

	//���������ļ�
	agk::LoadSound(SOUND_INDEX,"BugZapper/boink.wav");

	//���λ�ò�������
	generateNewBug();

	//���ó��Ӷ���
	agk::SetSpriteAnimation(BUG_INDEX,FRAME_WIDTH,FRAME_HEIGTH,FRAME_COUNT);

	//���ų��Ӷ���
	agk::PlaySprite(BUG_INDEX);
}

// Main loop, called every frame
void app::Loop ( void )
{
	//�ж���Ϸ��״̬
	switch(game_state)
	{
	case GAME_JUST_START:
		displayIntro();//��ʾ���ܽ���

		if(agk::GetRawLastKey())//�Ƿ��а�������
		{
			hideIntro();//���ؽ��ܽ���

			game_state=GAME_IN_PLAY;//�ı���Ϸ״̬

			agk::ResetTimer();//���ö�ʱ��
		}
		break;

	case GAME_IN_PLAY:
		playGame();//��ʼ����Ϸ
		break;

	case GAME_HIGH_CHECK:
		checkHighScore();
		break;

	case GAME_OVER:
		closingScreen();//�ر���Ļ
		break;
	} 

	//������Ļ
	agk::Sync();
}

// Called when the app ends
void app::End ( void )
{
}


//�����λ�ò�������
void generateNewBug()
{
	//��ȡ���ӵĿ�͸�
	float bugWidth=agk::GetSpriteWidth(BUG_INDEX);
	float bugHeigth=agk::GetSpriteHeight(BUG_INDEX);

	//�����������λ��
	int x=agk::Random(0,SCREEN_WIDTH-bugWidth);
	int y=agk::Random(0,SCREEN_HEIGHT-bugHeigth);

	//���ó��ӵ�λ��
	agk::SetSpritePosition(BUG_INDEX,x,y);
}

//��ʾ���ܽ���
void displayIntro()
{
	//���ӺͲݵز��ɼ������ܽ���ɼ�
	agk::SetSpriteVisible(GRASS_INDEX,0);
	agk::SetSpriteVisible(BUG_INDEX,0);

	agk::SetSpriteVisible(INTRO_INDEX,1);
}

//����Ϸ
void playGame()
{
	float mouseX,mouseY;

	//�ж���Ϸʱ���Ƿ��㹻
	if(agk::Timer() < MAX_TIME)
	{
		if(agk::GetRawMouseLeftPressed())//�������Ƿ���
		{
			mouseX=agk::GetRawMouseX();//��ȡ��ǰ����λ��
			mouseY=agk::GetRawMouseY();

			if(agk::GetSpriteHit(mouseX,mouseY) == BUG_INDEX)//�Ƿ�������
			{
				agk::PlaySound(SOUND_INDEX);//��������

			    bug_count++;//��������һ

				generateNewBug();//�����λ�ò����³���
			}
		}
	}
	else//ʱ�����
	{
		game_state=GAME_HIGH_CHECK;//�ı���Ϸ��״̬
	}
}

//�����߷�
void checkHighScore()
{
	//��ȡ��һ�εļ�¼
	
	if(agk::GetFileExists("BugZapperHighScore.dat"))//�洢��߷ֵ��ļ�����
	{
		//���ļ�
		agk::OpenToRead(HIGH_SCORE_FILE,"BugZapperHighScore.dat");

		//��ȡ��߷�
		prev_HighScore=agk::ReadInteger(HIGH_SCORE_FILE);
		//�ر��ļ�
		agk::CloseFile(HIGH_SCORE_FILE);
	}
	else//�ļ�������
	{
		prev_HighScore=0;
	}

	//�Ƚ�֮ǰ�����ڵ�
	if(bug_count > prev_HighScore)//��ǰ����ĳ���������֮ǰ��
	{
		newHighScore();//���¼�¼

	}

	//�ı���Ϸ��״̬

	game_state=GAME_OVER;
}

//���¼�¼�����ļ���д�����ݣ�����֮ǰ�����ݣ�
void newHighScore()
{
	agk::OpenToWrite(HIGH_SCORE_FILE,"BugZapperHighScore.dat");//���ļ�

	agk::WriteInteger(HIGH_SCORE_FILE,bug_count);//д���ļ�

	agk::CloseFile(HIGH_SCORE_FILE);//�ر��ļ�
}



//���ؽ��ܽ���
void hideIntro()
{
	//���ӺͲݵؿɼ������ܽ��治�ɼ�
	agk::SetSpriteVisible(INTRO_INDEX,0);

	agk::SetSpriteVisible(BUG_INDEX,1);
	agk::SetSpriteVisible(GRASS_INDEX,1);
	
}

//�ر���Ļ
void closingScreen()
{
	//���زݵغͳ���
	agk::SetSpriteVisible(BUG_INDEX,0);
	agk::SetSpriteVisible(GRASS_INDEX,0);

	//��ӡ������ĳ�����
	agk::PrintC("Number of bugs zapped:");
	agk::Print(bug_count);

	//������Ƽ�¼����ʾף��
	if(bug_count > prev_HighScore)
	{
		agk::Print("Congratulations!");
		agk::Print("You set a new high score!");
	}
}
