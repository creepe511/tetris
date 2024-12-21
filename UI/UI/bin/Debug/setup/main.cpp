#include<SFML/Graphics.hpp> //图像处理
#include<SFML/Audio.hpp> //声音处理
#include<time.h> //时间处理
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

using namespace sf; //声明命名空间

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
//7.游戏结束判定
//8.优化:音乐，音效，暂停，时间，下一个下落方块的提示

//游戏方块的表示
int blocks[7][4] = {  //7种方块，每个4格
	{1,3,5,7}, //I型
	{0,3,2,5}, //S型
	{1,3,2,4}, //Z型
	{1,3,2,5}, //T型
	{0,3,1,5}, //L型
	{1,3,5,4}, //J型
	{2,3,4,5}  //田型
};

int blockIndex; //表示当前方块的种类
int nextblockIndex; //表示下一方块的种类

//表示当前和下一方块坐标,并备份
struct point
{
	int x;
	int y;
};
struct point curBlock[4], nextBlock[4], backBlock[4];

//生成方块
void newBlocks()
{
	blockIndex = nextblockIndex;
	nextblockIndex = rand() % 7; //获取一个随机值【0，6】
	for (int i = 0;i < 4;i++)
	{
		//把俄罗斯方块转换成坐标表示
		//序号 %2 就是x坐标
		//序号 /2 就是y坐标
		curBlock[i].x = blocks[blockIndex][i] % 2 + 4;
		curBlock[i].y = blocks[blockIndex][i] / 2 ;
		nextBlock[i].x = blocks[nextblockIndex][i] % 2 + 4;
		nextBlock[i].y = blocks[nextblockIndex][i] / 2;
	}

	//随机旋转新方块
	for (int i = 0;i < nextblockIndex;i++)
	{
		doRotate();
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
				spriteBlock->setTextureRect(IntRect((table[i][j] - 1) * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE)); //设置绘制区域(截取区域)
				spriteBlock->setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE); //绘制位置
				spriteBlock->move(23, -128); //设置偏移量
				window->draw(*spriteBlock); //绘制
			}
		}
	}
	//2.正在降落的方块
	for (int i = 0;i < 4;i++)
	{
		spriteBlock->setTextureRect(IntRect(blockIndex * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE)); //设置绘制区域(截取区域)
		spriteBlock->setPosition(curBlock[i].x * BLOCK_SIZE, curBlock[i].y * BLOCK_SIZE); //绘制位置
		spriteBlock->move(23, -128); //设置偏移量
		window->draw(*spriteBlock); //绘制
	}

	//3.即将落下的方块
	for (int i = 0;i < 4;i++)
	{
		spriteBlock->setTextureRect(IntRect(nextblockIndex * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE)); //设置绘制区域(截取区域)
		if (nextblockIndex == 0)
		{
			spriteBlock->setPosition(nextBlock[i].x * BLOCK_SIZE + 300, nextBlock[i].y * BLOCK_SIZE + 60); //绘制位置
		}
		else if (nextblockIndex == 6)
		{
			spriteBlock->setPosition(nextBlock[i].x * BLOCK_SIZE + 320, nextBlock[i].y * BLOCK_SIZE + 65); //绘制位置
		}
		else
		{
			spriteBlock->setPosition(nextBlock[i].x * BLOCK_SIZE + 320, nextBlock[i].y * BLOCK_SIZE + 80); //绘制位置
		}
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

Sound sound1; //音效1

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

		if (!isclearLine())
		{
			sound1.play();
		}
		else
		{
			clearLine();
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
//平面中，一个点(x, y)绕任意点(dx, dy)逆时针旋转a度后的坐标
//x’ = (x - dx) * cos(a) - (y - dy) * sin(a) + dx;
//y’ = (x - dx) * sin(a) + (y - dy) * cos(a) + dy;
//在实际程序中，顺时针相当于数学中的逆时针
void doRotate()
{
	if (blockIndex == 6)
	{
		return; //防止田型方块因旋转而移动
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

bool paused = false; //暂停处理

//处理按键(左右移动，旋转，暂停，加速下落)
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

		//处理加速下降
		if (Keyboard::isKeyPressed(Keyboard::Down)&&!paused) //一直按下Down键
		{
			delay = SPEED_FAST;
		}
		else
			delay = SPEED_NORMAL;
	}
}

Sound sound2; //音效2

int score = 0;

//消除加分处理
bool isclearLine() //判断是否消除
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
			table[k][j] = table[i][j]; //重新书写行数
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

//游戏结束判定
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
	//记录游戏开始时刻
	// 获取当前时间（系统时间，假设为UTC时间）
	auto now = std::chrono::system_clock::now();

	// 转换为时间点（UTC时间）
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

	// 将系统时间转换为结构体形式
	std::tm now_tm;
	gmtime_s(&now_tm, &now_time_t); // 使用 gmtime_s 得到 UTC 时间

	// 将时间结构转换为北京时间（UTC + 8 小时）
	now_tm.tm_hour += 8; // 增加8小时

	// 调用 mktime 使结构体自动调整
	std::mktime(&now_tm); // 处理溢出问题（例如，日期自动调整）

	// 输出北京时间
	std::cout << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
	

	srand((unsigned)time(NULL));//生成随机种子

	//游戏界面绘制
	//1.创建游戏窗口
	RenderWindow window(
		VideoMode(576,746), //窗口模式:音频
		"TETRIS"); //窗口标题

	//2.添加游戏背景
	Texture t1; //把背景图片文件加载到内存
	if (!t1.loadFromFile("image/bg.jpg"))
	{
		return -1;
	}
	Sprite spriteBg(t1); //根据图片来创造精灵
	
	Texture t2; //把方块图片文件加载到内存
	if (!t2.loadFromFile("image/block.jpg"))
	{
		return -1;
	}
	Sprite spriteBlock(t2); //根据图片来创造精灵

	//3.分数显示
	Font font1;
	if (!font1.loadFromFile("font/ALGER.ttf"))
	{
		return -1;
	}
	Text scoreText;
	scoreText.setFont(font1);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(400,335); //文本位置
	scoreText.setRotation(-5.f); //文本角度

	//4.时间显示
	Text timeText;
	timeText.setFont(font1);
	timeText.setCharacterSize(25);
	timeText.setFillColor(Color::White);
	timeText.setPosition(390, 700);

	//5.背景音乐
	Music music;
	if (!music.openFromFile("music/bgm.wav")) //打开音乐文件,判断是否加载成功
	{
		return -1;
	}
	music.setLoop(true); //循环播放
	music.play();

	//6.音效
	SoundBuffer down;
	if (!down.loadFromFile("music/down.wav"))
	{
		return -1;
	}
	sound1.setBuffer(down); //缓存

	SoundBuffer xiaochu; 
	if (!xiaochu.loadFromFile("music/xiaochu.wav"))
	{
		return -1;
	}
	sound2.setBuffer(xiaochu); //缓存

	//生成第一个方块
	nextblockIndex = rand() % 7; //获取一个随机值【0，6】
	newBlocks();

	//计时器Clock(SFML)
	Clock clock;

	//总时间
	float timer1 = 0;
	float timer2 = 0;

	//进入游戏循环
	while (window.isOpen())
	{
		//获取从clock被启动（重启）到现在的时间间隔，并转换为秒数
		float time = clock.getElapsedTime().asSeconds();
		clock.restart(); //重启计时器
		if (!paused)
		{
			timer1 += time; // 只有在游戏没有暂停时，才让时间流逝
			timer2 += time;
		}

		//自然下落
		if (timer1 > delay&&!paused)
		{
			//降落一个位置
			drop();

			timer1 = 0;
		}

		//更新得分
		scoreText.setString("Score: " + std::to_string(score));
		
		//更新时间
		timeText.setString("Time: " + std::to_string((int)timer2)+"s");

		//游戏结束处理
		if (isGameOver())
		{
			paused = true;
			music.stop(); // 游戏结束时停止音乐
			printf("Final Score: %d\n", score);
			printf("Game Time: %ds\n", (int)timer2);

			SoundBuffer gameover;
			if (!gameover.loadFromFile("music/gameover.wav"))
			{
				return -1;
			}
			Sound sound3;
			sound3.setBuffer(gameover);
			sound3.play(); // 播放音效

			Font font2;
			if (!font2.loadFromFile("font/ALGER.ttf"))
			{
				return -1;
			}
			Text gameOverText;
			gameOverText.setFont(font2);
			gameOverText.setCharacterSize(70);
			gameOverText.setFillColor(Color::White);
			gameOverText.setPosition(100, 300); //文本位置
			gameOverText.setString("GAME OVER");

			Text scoreText2;
			scoreText2.setFont(font2);
			scoreText2.setCharacterSize(40);
			scoreText2.setFillColor(Color::White);
			scoreText2.setPosition(150, 370);
			scoreText2.setString("Final Score: " + std::to_string(score));

			// 游戏结束界面渲染
			while (sound3.getStatus() == Sound::Playing) // 音效播放时保持循环
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

			break; // 跳过本次循环，防止其他操作更新
		}

		//等待按下按键，并处理
		keyEvent(&window);

		//渲染精灵,绘制游戏背景
		window.draw(spriteBg); 

		//绘制方块
		drawBlocks(&spriteBlock,&window);

		//绘制分数文本及时间文本
		window.draw(scoreText);
		window.draw(timeText);

		//绘制游戏暂停提示
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
			pauseText.setPosition(170, 300); //文本位置
			pauseText.setString("PAUSED");
			window.draw(pauseText);
		}

		window.display(); //刷新并显示窗口

	}
	system("pause");
	return 0;
}
