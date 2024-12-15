#include<SFML/Graphics.hpp> //ͼ����
#include<SFML/Audio.hpp> //��������
#include<time.h> //ʱ�䴦��
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 20
#define COL 10
#define SPEED_NORMAL 0.5
#define SPEED_FAST 0.05

using namespace sf; //���������ռ�

void newBlocks();
void drawBlocks(Sprite* spriteBlock, RenderWindow* window);
bool check();
void drop();
void moveLeft();
void moveRight();
void doRotate();
void keyEvent(RenderWindow* window);

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
//7.�Ż�:���֣���ͣ

//��Ϸ����ı�ʾ
int blocks[7][4] = {  //7�ַ��飬ÿ��4��
	{1,3,5,7}, //I��
	{0,2,3,5}, //S��
	{1,3,2,4}, //Z��
	{1,3,2,5}, //T��
	{0,1,3,5}, //L��
	{1,3,5,4}, //J��
	{0,1,2,3}  //����
};

int blockIndex; //��ʾ��ǰ���������

//��ʾ��ǰ��������,������
struct point
{
	int x;
	int y;
};
struct point curBlock[4],backBlock[4];

//���ɷ���
void newBlocks()
{
	//��ȡһ�����ֵ��0��6��
	blockIndex = rand() % 7;
	for (int i = 0;i < 4;i++)
	{
		//�Ѷ���˹����ת���������ʾ
		//��� %2 ����x����
		//��� /2 ����y����
		curBlock[i].x = blocks[blockIndex][i]%2;
		curBlock[i].y = blocks[blockIndex][i]/2;
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
				spriteBlock->setTextureRect(IntRect((table[i][j] - 1) * 36, 0, 36, 36)); //���û�������(��ȡ����)
				spriteBlock->setPosition(j * 36, i * 36); //����λ��
				spriteBlock->move(23, 16); //����ƫ����
				window->draw(*spriteBlock); //����
			}
		}
	}
	//2.���ڽ���ķ���
	for (int i = 0;i < 4;i++)
	{
		spriteBlock->setTextureRect(IntRect(blockIndex * 36, 0, 36, 36)); //���û�������(��ȡ����)
		spriteBlock->setPosition(curBlock[i].x * 36, curBlock[i].y * 36); //����λ��
		spriteBlock->move(23, 16); //����ƫ����
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
void doRotate()
{

}

//������(�����ƶ�����ת)
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
				doRotate();
				break;
			case Keyboard::Left:
				moveLeft();
				break;
			case Keyboard::Right:
				moveRight();
				break;
			default:
				break;
			}
		}

		//�����½�
		if (Keyboard::isKeyPressed(Keyboard::Down)) //һֱ����Down��
		{
			delay = SPEED_FAST;
		}
		else
			delay = SPEED_NORMAL;
	}
}

int main(void)
{
	srand((unsigned)time(NULL));//�����������

	//��Ϸ�������
	//1.������Ϸ����
	RenderWindow window(
		VideoMode(567,737),//����ģʽ:��Ƶ
		"TETRIS"); //���ڱ���

	//2.�����Ϸ����
	Texture t1; //�ѱ���ͼƬ�ļ����ص��ڴ�
	t1.loadFromFile("image/bg1.jpg");
	Sprite spriteBg(t1); //����ͼƬ�����쾫��

	Texture t2; //�ѷ���ͼƬ�ļ����ص��ڴ�
	t2.loadFromFile("image/block.jpg");
	Sprite spriteBlock(t2); //����ͼƬ�����쾫��

	//���ɵ�һ������
	newBlocks();

	//��ʱ��Clock(SFML)
	Clock clock;

	//��ʱ��
	float timer = 0; 

	//������Ϸѭ��
	while (window.isOpen())
	{
		//��ȡ��clock�������������������ڵ�ʱ��������ת��Ϊ����
		float time = clock.getElapsedTime().asSeconds();
		clock.restart(); //������ʱ��
		timer += time;

		if (timer > delay)
		{
			//����һ��λ��
			drop();

			timer = 0;
		}

		//�ȴ����°�����������
		keyEvent(&window);

		//������Ϸ����
		window.draw(spriteBg); //��Ⱦ����

		//���Ʒ���
		drawBlocks(&spriteBlock,&window);

		window.display(); //ˢ�²���ʾ����

	}

	system("pause");
	return 0;
}
