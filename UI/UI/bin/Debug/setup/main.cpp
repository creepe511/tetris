#include<SFML/Graphics.hpp> //ͼ����
#include<SFML/Audio.hpp> //��������
#include<time.h> //ʱ�䴦��
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<chrono>
#include<ctime>
#include<iomanip>

#define ROW 24
#define COL 10
#define SPEED_NORMAL 0.5
#define SPEED_FAST 0.05
#define BLOCK_SIZE 36

using namespace sf; //���������ռ�

void newBlocks();
void drawBlocks(Sprite* spriteBlock, RenderWindow* window);
bool check();
void drop();
void moveLeft();
void moveRight();
void doRotate();
void keyEvent(RenderWindow* window);
bool isclearLine();
void clearLine();
bool isGameOver();

//�����ؼ�����
//1.�SFML����
//2.������
//3.���ɷ���
	//3.1.����ı�ʾ
		//0  1
		//2  3
		//4  5
		//6  7
//4.������ƶ�����ת
//5.����
//6.�ӷ�
//7.��Ϸ�����ж�
//8.�Ż�:���֣���Ч����ͣ��ʱ�䣬��һ�����䷽�����ʾ

//��Ϸ����ı�ʾ
int blocks[7][4] = {  //7�ַ��飬ÿ��4��
	{1,3,5,7}, //I��
	{0,3,2,5}, //S��
	{1,3,2,4}, //Z��
	{1,3,2,5}, //T��
	{0,3,1,5}, //L��
	{1,3,5,4}, //J��
	{2,3,4,5}  //����
};

int blockIndex; //��ʾ��ǰ���������
int nextblockIndex; //��ʾ��һ���������

//��ʾ��ǰ����һ��������,������
struct point
{
	int x;
	int y;
};
struct point curBlock[4], nextBlock[4], backBlock[4];

//���ɷ���
void newBlocks()
{
	blockIndex = nextblockIndex;
	nextblockIndex = rand() % 7; //��ȡһ�����ֵ��0��6��
	for (int i = 0;i < 4;i++)
	{
		//�Ѷ���˹����ת���������ʾ
		//��� %2 ����x����
		//��� /2 ����y����
		curBlock[i].x = blocks[blockIndex][i] % 2 + 4;
		curBlock[i].y = blocks[blockIndex][i] / 2 ;
		nextBlock[i].x = blocks[nextblockIndex][i] % 2 + 4;
		nextBlock[i].y = blocks[nextblockIndex][i] / 2;
	}

	//�����ת�·���
	for (int i = 0;i < nextblockIndex;i++)
	{
		doRotate();
	}
}

//��Ϸ����ı�ʾ
//table[i][j]==0 ��ʾ��i�е�j���ǿհ�
//table[i][j]==n ��ʾ��i�е�j���ǵ�n����ɫ�ķ���
int table[ROW][COL] = { 0 };

//���Ʒ���
void drawBlocks(Sprite *spriteBlock, RenderWindow *window)
{
	//1.�ѽ��䵽�ײ��ķ���
	for (int i = 0;i < ROW;i++)
	{
		for (int j = 0;j < COL;j++)
		{
			if (table[i][j] != 0)             
			{
				//������
				//��Ҫʹ��Sprite��ʾ�����ķ���ͼƬ
				spriteBlock->setTextureRect(IntRect((table[i][j] - 1) * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE)); //���û�������(��ȡ����)
				spriteBlock->setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE); //����λ��
				spriteBlock->move(23, -128); //����ƫ����
				window->draw(*spriteBlock); //����
			}
		}
	}
	//2.���ڽ���ķ���
	for (int i = 0;i < 4;i++)
	{
		spriteBlock->setTextureRect(IntRect(blockIndex * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE)); //���û�������(��ȡ����)
		spriteBlock->setPosition(curBlock[i].x * BLOCK_SIZE, curBlock[i].y * BLOCK_SIZE); //����λ��
		spriteBlock->move(23, -128); //����ƫ����
		window->draw(*spriteBlock); //����
	}

	//3.�������µķ���
	for (int i = 0;i < 4;i++)
	{
		spriteBlock->setTextureRect(IntRect(nextblockIndex * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE)); //���û�������(��ȡ����)
		if (nextblockIndex == 0)
		{
			spriteBlock->setPosition(nextBlock[i].x * BLOCK_SIZE + 300, nextBlock[i].y * BLOCK_SIZE + 60); //����λ��
		}
		else if (nextblockIndex == 6)
		{
			spriteBlock->setPosition(nextBlock[i].x * BLOCK_SIZE + 320, nextBlock[i].y * BLOCK_SIZE + 65); //����λ��
		}
		else
		{
			spriteBlock->setPosition(nextBlock[i].x * BLOCK_SIZE + 320, nextBlock[i].y * BLOCK_SIZE + 80); //����λ��
		}
		window->draw(*spriteBlock); //����
	}
}; 

//��鷽��λ���Ƿ�Ϸ������ң��½磨��ײ����
bool check()
{
	for (int i = 0;i < 4;i++)
	{
		if (curBlock[i].x<0
			|| curBlock[i].x>COL - 1
			|| curBlock[i].y > ROW - 1
			|| table[curBlock[i].y][curBlock[i].x] != 0)
			return false;
	}
	return true;
}

Sound sound1; //��Ч1

//��ʾ���������ӳ�
float delay = SPEED_NORMAL;

//��������
void drop()
{
	for (int i = 0;i < 4;i++)
	{
		backBlock[i] = curBlock[i];
		curBlock[i].y += 1;
	}
	if (check() == false)
	{
		//�̻�����
		for (int i = 0;i < 4;i++)
		{
			table[backBlock[i].y][backBlock[i].x] = blockIndex+1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
		}

		if (!isclearLine())
		{
			sound1.play();
		}
		else
		{
			clearLine();
		}
		
		//����һ���·���
		newBlocks();
	}
}

//��������
void moveLeft()
{
	for (int i = 0;i < 4;i++)
	{
		backBlock[i] = curBlock[i];
		curBlock[i].x -= 1;
	}
	if (check() == false)
	{
		for (int i = 0;i < 4;i++)
		{
			curBlock[i] = backBlock[i];
		}
	}
}

//��������
void moveRight()
{
	for (int i = 0;i < 4;i++)
	{
		backBlock[i] = curBlock[i];
		curBlock[i].x += 1;
	}
	if (check() == false)
	{
		for (int i = 0;i < 4;i++)
		{
			curBlock[i] = backBlock[i];
		}
	}
}

//������ת
//ƽ���У�һ����(x, y)�������(dx, dy)��ʱ����תa�Ⱥ������
//x�� = (x - dx) * cos(a) - (y - dy) * sin(a) + dx;
//y�� = (x - dx) * sin(a) + (y - dy) * cos(a) + dy;
//��ʵ�ʳ����У�˳ʱ���൱����ѧ�е���ʱ��
void doRotate()
{
	if (blockIndex == 6)
	{
		return; //��ֹ���ͷ�������ת���ƶ�
	}
	for (int i = 0;i < 4; i++)
	{
		backBlock[i] = curBlock[i];
	}
	for (int i = 0;i < 4; i++)
	{
		curBlock[i].x = -(backBlock[i].y - backBlock[1].y) + backBlock[1].x;
		curBlock[i].y = (backBlock[i].x - backBlock[1].x)  + backBlock[1].y;
	}
	if (check() == false)
	{
		for (int i = 0;i < 4;i++)
		{
			curBlock[i] = backBlock[i];
		}
	}
}

bool paused = false; //��ͣ����

//������(�����ƶ�����ת����ͣ����������)
void keyEvent(RenderWindow *window)
{
	Event event; //�¼�����
	//pollEvent ���¼�������ȡ��һ���¼�������event��
	//���¼�����true,���¼�����false
	while (window->pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window->close();
		}

		if (event.type == Event::KeyPressed) //���°���
		{
			switch (event.key.code)
			{
			case Keyboard::Up:
				if(!paused)
				{
					doRotate();
				}
				break;
			case Keyboard::Left:
				if (!paused)
				{
					moveLeft();
				}
				break;
			case Keyboard::Right:
				if (!paused)
				{
					moveRight();
				}
				break;
			case Keyboard::P:
				paused = !paused;
				break;
			default:
				break;
			}
		}

		//��������½�
		if (Keyboard::isKeyPressed(Keyboard::Down)&&!paused) //һֱ����Down��
		{
			delay = SPEED_FAST;
		}
		else
			delay = SPEED_NORMAL;
	}
}

Sound sound2; //��Ч2

int score = 0;

//�����ӷִ���
bool isclearLine() //�ж��Ƿ�����
{
	for (int i = ROW - 1;i >= 0;i--)
	{
		int count = 0;
		for (int j = 0;j < COL;j++)
		{
			if (table[i][j] != 0)
			{
				count++;
			}
		}
		if (count == COL)
		{
			return true;
		}
	}
	return false;
}

void clearLine()
{
	int k = ROW - 1;
	for (int i = ROW - 1;i >= 0;i--)
	{
		int count = 0;
		for (int j = 0;j < COL;j++)
		{
			if (table[i][j] != 0)
			{
				count++;
			}
			table[k][j] = table[i][j]; //������д����
		}
		if (count < COL)
		{
			k--;
		}
		else
		{
			sound2.play();
			score += 1;
		}
	}
}

//��Ϸ�����ж�
bool isGameOver()
{
	for (int j = 0;j < COL;j++)
	{
		if (table[3][j] != 0)
		{
			return true;
		}
	}
	return false;
}

int main(void)
{
	//��¼��Ϸ��ʼʱ��
	// ��ȡ��ǰʱ�䣨ϵͳʱ�䣬����ΪUTCʱ�䣩
	auto now = std::chrono::system_clock::now();

	// ת��Ϊʱ��㣨UTCʱ�䣩
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

	// ��ϵͳʱ��ת��Ϊ�ṹ����ʽ
	std::tm now_tm;
	gmtime_s(&now_tm, &now_time_t); // ʹ�� gmtime_s �õ� UTC ʱ��

	// ��ʱ��ṹת��Ϊ����ʱ�䣨UTC + 8 Сʱ��
	now_tm.tm_hour += 8; // ����8Сʱ

	// ���� mktime ʹ�ṹ���Զ�����
	std::mktime(&now_tm); // ����������⣨���磬�����Զ�������

	// �������ʱ��
	std::cout << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
	

	srand((unsigned)time(NULL));//�����������

	//��Ϸ�������
	//1.������Ϸ����
	RenderWindow window(
		VideoMode(576,746), //����ģʽ:��Ƶ
		"TETRIS"); //���ڱ���

	//2.�����Ϸ����
	Texture t1; //�ѱ���ͼƬ�ļ����ص��ڴ�
	if (!t1.loadFromFile("image/bg.jpg"))
	{
		return -1;
	}
	Sprite spriteBg(t1); //����ͼƬ�����쾫��
	
	Texture t2; //�ѷ���ͼƬ�ļ����ص��ڴ�
	if (!t2.loadFromFile("image/block.jpg"))
	{
		return -1;
	}
	Sprite spriteBlock(t2); //����ͼƬ�����쾫��

	//3.������ʾ
	Font font1;
	if (!font1.loadFromFile("font/ALGER.ttf"))
	{
		return -1;
	}
	Text scoreText;
	scoreText.setFont(font1);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(400,335); //�ı�λ��
	scoreText.setRotation(-5.f); //�ı��Ƕ�

	//4.ʱ����ʾ
	Text timeText;
	timeText.setFont(font1);
	timeText.setCharacterSize(25);
	timeText.setFillColor(Color::White);
	timeText.setPosition(390, 700);

	//5.��������
	Music music;
	if (!music.openFromFile("music/bgm.wav")) //�������ļ�,�ж��Ƿ���سɹ�
	{
		return -1;
	}
	music.setLoop(true); //ѭ������
	music.play();

	//6.��Ч
	SoundBuffer down;
	if (!down.loadFromFile("music/down.wav"))
	{
		return -1;
	}
	sound1.setBuffer(down); //����

	SoundBuffer xiaochu; 
	if (!xiaochu.loadFromFile("music/xiaochu.wav"))
	{
		return -1;
	}
	sound2.setBuffer(xiaochu); //����

	//���ɵ�һ������
	nextblockIndex = rand() % 7; //��ȡһ�����ֵ��0��6��
	newBlocks();

	//��ʱ��Clock(SFML)
	Clock clock;

	//��ʱ��
	float timer1 = 0;
	float timer2 = 0;

	//������Ϸѭ��
	while (window.isOpen())
	{
		//��ȡ��clock�������������������ڵ�ʱ��������ת��Ϊ����
		float time = clock.getElapsedTime().asSeconds();
		clock.restart(); //������ʱ��
		if (!paused)
		{
			timer1 += time; // ֻ������Ϸû����ͣʱ������ʱ������
			timer2 += time;
		}

		//��Ȼ����
		if (timer1 > delay&&!paused)
		{
			//����һ��λ��
			drop();

			timer1 = 0;
		}

		//���µ÷�
		scoreText.setString("Score: " + std::to_string(score));
		
		//����ʱ��
		timeText.setString("Time: " + std::to_string((int)timer2)+"s");

		//��Ϸ��������
		if (isGameOver())
		{
			paused = true;
			music.stop(); // ��Ϸ����ʱֹͣ����
			printf("Final Score: %d\n", score);
			printf("Game Time: %ds\n", (int)timer2);

			SoundBuffer gameover;
			if (!gameover.loadFromFile("music/gameover.wav"))
			{
				return -1;
			}
			Sound sound3;
			sound3.setBuffer(gameover);
			sound3.play(); // ������Ч

			Font font2;
			if (!font2.loadFromFile("font/ALGER.ttf"))
			{
				return -1;
			}
			Text gameOverText;
			gameOverText.setFont(font2);
			gameOverText.setCharacterSize(70);
			gameOverText.setFillColor(Color::White);
			gameOverText.setPosition(100, 300); //�ı�λ��
			gameOverText.setString("GAME OVER");

			Text scoreText2;
			scoreText2.setFont(font2);
			scoreText2.setCharacterSize(40);
			scoreText2.setFillColor(Color::White);
			scoreText2.setPosition(150, 370);
			scoreText2.setString("Final Score: " + std::to_string(score));

			// ��Ϸ����������Ⱦ
			while (sound3.getStatus() == Sound::Playing) // ��Ч����ʱ����ѭ��
			{
				keyEvent(&window);
				window.clear();
				window.draw(spriteBg);
				drawBlocks(&spriteBlock, &window);
				window.draw(timeText);
				window.draw(gameOverText);
				window.draw(scoreText2);
				window.display();
			}
			sound3.stop();
			window.close();

			break; // ��������ѭ������ֹ������������
		}

		//�ȴ����°�����������
		keyEvent(&window);

		//��Ⱦ����,������Ϸ����
		window.draw(spriteBg); 

		//���Ʒ���
		drawBlocks(&spriteBlock,&window);

		//���Ʒ����ı���ʱ���ı�
		window.draw(scoreText);
		window.draw(timeText);

		//������Ϸ��ͣ��ʾ
		if (paused&&!isGameOver())
		{
			Font font3;
			if (!font3.loadFromFile("font/ALGER.ttf"))
			{
				return -1;
			}
			Text pauseText;
			pauseText.setFont(font3);
			pauseText.setCharacterSize(70);
			pauseText.setFillColor(Color::White);
			pauseText.setPosition(170, 300); //�ı�λ��
			pauseText.setString("PAUSED");
			window.draw(pauseText);
		}

		window.display(); //ˢ�²���ʾ����

	}
	system("pause");
	return 0;
}
