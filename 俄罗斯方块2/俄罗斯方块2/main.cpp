#include<SFML/Graphics.hpp> //图像处理
#include<SFML/Audio.hpp> //声音处理
#include<time.h> //时间处理
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 20
#define COL 10
#define SPEED_NORMAL 0.5
#define SPEED_FAST 0.05

using namespace sf; //声明命名空间

void newBlocks();
void drawBlocks(Sprite* spriteBlock, RenderWindow* window);
bool check();
void drop();
void moveLeft();
void moveRight();
void doRotate();
void keyEvent(RenderWindow* window);

//开发关键步骤
//1.搭建SFML环境
//2.处理按键
//3.生成方块
	//3.1.方块的表示
		//0  1
		//2  3
		//4  5
		//6  7
//4.方块的移动与旋转
//5.消除
//6.加分
//7.优化:音乐，暂停

//游戏方块的表示
int blocks[7][4] = {  //7种方块，每个4格
	{1,3,5,7}, //I型
	{0,2,3,5}, //S型
	{1,3,2,4}, //Z型
	{1,3,2,5}, //T型
	{0,1,3,5}, //L型
	{1,3,5,4}, //J型
	{0,1,2,3}  //田型
};

int blockIndex; //表示当前方块的种类

//表示当前方块坐标,并备份
struct point
{
	int x;
	int y;
};
struct point curBlock[4],backBlock[4];

//生成方块
void newBlocks()
{
	//获取一个随机值【0，6】
	blockIndex = rand() % 7;
	for (int i = 0;i < 4;i++)
	{
		//把俄罗斯方块转换成坐标表示
		//序号 %2 就是x坐标
		//序号 /2 就是y坐标
		curBlock[i].x = blocks[blockIndex][i]%2;
		curBlock[i].y = blocks[blockIndex][i]/2;
	}
}

//游戏区域的表示
//table[i][j]==0 表示第i行第j列是空白
//table[i][j]==n 表示第i行第j列是第n种颜色的方块
int table[ROW][COL] = { 0 };

//绘制方块
void drawBlocks(Sprite *spriteBlock, RenderWindow *window)
{
	//1.已降落到底部的方块
	for (int i = 0;i < ROW;i++)
	{
		for (int j = 0;j < COL;j++)
		{
			if (table[i][j] != 0)             
			{
				//画方块
				//需要使用Sprite表示完整的方块图片
				spriteBlock->setTextureRect(IntRect((table[i][j] - 1) * 36, 0, 36, 36)); //设置绘制区域(截取区域)
				spriteBlock->setPosition(j * 36, i * 36); //绘制位置
				spriteBlock->move(23, 16); //设置偏移量
				window->draw(*spriteBlock); //绘制
			}
		}
	}
	//2.正在降落的方块
	for (int i = 0;i < 4;i++)
	{
		spriteBlock->setTextureRect(IntRect(blockIndex * 36, 0, 36, 36)); //设置绘制区域(截取区域)
		spriteBlock->setPosition(curBlock[i].x * 36, curBlock[i].y * 36); //绘制位置
		spriteBlock->move(23, 16); //设置偏移量
		window->draw(*spriteBlock); //绘制
	}

}; 

//检查方块位置是否合法（左，右，下界（碰撞））
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

//表示方块下落延迟
float delay = SPEED_NORMAL;

//方块下落
void drop()
{
	for (int i = 0;i < 4;i++)
	{
		backBlock[i] = curBlock[i];
		curBlock[i].y += 1;
	}
	if (check() == false)
	{
		//固化处理
		for (int i = 0;i < 4;i++)
		{
			table[backBlock[i].y][backBlock[i].x] = blockIndex+1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
		}
		//产生一个新方块
		newBlocks();
	}
}

//方块左移
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

//方块右移
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

//方块旋转
void doRotate()
{

}

//处理按键(左右移动，旋转)
void keyEvent(RenderWindow *window)
{
	Event event; //事件变量
	//pollEvent 从事件队列中取出一个事件保存在event中
	//有事件返回true,无事件返回false
	while (window->pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window->close();
		}

		if (event.type == Event::KeyPressed) //按下按键
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

		//处理下降
		if (Keyboard::isKeyPressed(Keyboard::Down)) //一直按下Down键
		{
			delay = SPEED_FAST;
		}
		else
			delay = SPEED_NORMAL;
	}
}

int main(void)
{
	srand((unsigned)time(NULL));//生成随机种子

	//游戏界面绘制
	//1.创建游戏窗口
	RenderWindow window(
		VideoMode(567,737),//窗口模式:音频
		"TETRIS"); //窗口标题

	//2.添加游戏背景
	Texture t1; //把背景图片文件加载到内存
	t1.loadFromFile("image/bg1.jpg");
	Sprite spriteBg(t1); //根据图片来创造精灵

	Texture t2; //把方块图片文件加载到内存
	t2.loadFromFile("image/block.jpg");
	Sprite spriteBlock(t2); //根据图片来创造精灵

	//生成第一个方块
	newBlocks();

	//计时器Clock(SFML)
	Clock clock;

	//总时间
	float timer = 0; 

	//进入游戏循环
	while (window.isOpen())
	{
		//获取从clock被启动（重启）到现在的时间间隔，并转换为秒数
		float time = clock.getElapsedTime().asSeconds();
		clock.restart(); //重启计时器
		timer += time;

		if (timer > delay)
		{
			//降落一个位置
			drop();

			timer = 0;
		}

		//等待按下按键，并处理
		keyEvent(&window);

		//绘制游戏背景
		window.draw(spriteBg); //渲染精灵

		//绘制方块
		drawBlocks(&spriteBlock,&window);

		window.display(); //刷新并显示窗口

	}

	system("pause");
	return 0;
}
