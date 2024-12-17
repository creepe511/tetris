using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;

namespace UI
{
    public partial class Form4 : Form
    {
        private Process gameProcess;

        public Form4()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            
        }

        // 处理窗口加载
        private void Form4_Load(object sender, EventArgs e)
        {
            // 初始化
        }

        // 处理窗口关闭
        private void Form4_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 确保游戏进程退出
            if (gameProcess != null && !gameProcess.HasExited)
            {
                gameProcess.Kill();
            }
            Application.Exit();
        }

        // 按钮点击时启动或重启游戏进程
        private void button1_Click(object sender, EventArgs e)
        {
            // 如果游戏进程已存在并且已经退出，重新启动
            if (gameProcess == null || gameProcess.HasExited)
            {
                StartGame();
            }
            else
            {
                RestartGame();
            }
        }
        
        // 按钮点击时返回 Form1，并结束游戏进程
        private void button2_Click_1(object sender, EventArgs e)
        {
            // 结束游戏进程
            if (gameProcess != null && !gameProcess.HasExited)
            {
                gameProcess.Kill();
            }

            // 返回 Form1
            Form1 form1 = new Form1();  // 假设 Form1 是你的第一个窗体
            form1.Show();
            this.Hide();  // 隐藏当前窗体
        }

        // 启动游戏方法
        private void StartGame()
        {
            // SFML 游戏可执行文件路径
            string gameExecutablePath = @"C:\Users\41920\Downloads\Setup1\Setup1\俄罗斯方块2.exe";  // 修改为你的实际路径

            // 确保游戏路径存在
            if (!File.Exists(gameExecutablePath))
            {
                MessageBox.Show("游戏文件未找到，请检查路径！");
                return;
            }

            // 启动 SFML 游戏进程
            gameProcess = new Process();
            gameProcess.StartInfo.FileName = gameExecutablePath;

            // 设置工作目录为游戏文件所在的目录
            gameProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName(gameExecutablePath);

            // 启动时隐藏游戏窗口（避免跳出独立窗口）
            gameProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            gameProcess.EnableRaisingEvents = true;

            // 事件捕捉
            gameProcess.Exited += GameProcess_Exited;

            // 启动游戏进程
            gameProcess.Start();

            // 等待 SFML 游戏窗口完全加载，使用线程等待或其他方法
            while (gameProcess.MainWindowHandle == IntPtr.Zero)
            {
                Thread.Sleep(100); // 每次循环稍等，确保获取到游戏窗口句柄
            }

            // 获取 SFML 游戏的窗口句柄并嵌入到 Panel 中
            EmbedGameWindow(gameProcess.MainWindowHandle);
        }

        // 重启游戏方法
        private void RestartGame()
        {
            // 结束当前游戏进程
            if (gameProcess != null && !gameProcess.HasExited)
            {
                gameProcess.Kill();
            }

            // 重新启动游戏
            StartGame();
        }

        // 游戏进程退出后的事件处理
        private void GameProcess_Exited(object sender, EventArgs e)
        {
            // 游戏进程退出后，处理退出逻辑
            Console.WriteLine("游戏进程已退出");
        }

        // 嵌入 SFML 游戏窗口
        private void EmbedGameWindow(IntPtr gameWindowHandle)
        {
            if (gameWindowHandle == IntPtr.Zero)
            {
                MessageBox.Show("无法获取游戏窗口句柄！");
                return;
            }

            // 获取 WinForms Panel 控件的窗口句柄
            IntPtr panelHandle = this.panel1.Handle;

            // 使用 P/Invoke 将 SFML 游戏窗口嵌入到 Panel 控件
            SetParent(gameWindowHandle, panelHandle);

            // 确保游戏窗口与 Panel 大小匹配
            MoveWindow(gameWindowHandle, 0, 0, panel1.Width, panel1.Height, true);

            // 强制显示游戏窗口
            ShowWindow(gameWindowHandle, SW_SHOWNORMAL);

            // 移除窗口边框和标题栏
            RemoveWindowBorder(gameWindowHandle);

            // 禁用右上角操作按钮（最小化、最大化、关闭按钮）
            DisableWindowButtons(gameWindowHandle);
        }

        // P/Invoke 声明
        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndParent);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool MoveWindow(IntPtr hWnd, int x, int y, int width, int height, bool repaint);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        [DllImport("user32.dll")]
        private static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll")]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr GetParent(IntPtr hwnd);

        // 确保游戏窗口显示
        private const int SW_SHOWNORMAL = 1;
        private const int GWL_STYLE = -16;
        private const int WS_BORDER = 0x00800000;
        private const int WS_CAPTION = 0x00C00000;
        private const int WS_MINIMIZEBOX = 0x20000;
        private const int WS_MAXIMIZEBOX = 0x10000;
        private const int WS_SYSMENU = 0x80000;
        private const int WS_THICKFRAME = 0x40000;

        // 移除窗口的边框和标题栏
        private void RemoveWindowBorder(IntPtr gameWindowHandle)
        {
            int currentStyle = GetWindowLong(gameWindowHandle, GWL_STYLE);
            int newStyle = currentStyle & ~WS_BORDER & ~WS_CAPTION;
            SetWindowLong(gameWindowHandle, GWL_STYLE, newStyle);
        }

        // 禁用最小化、最大化、关闭按钮
        private void DisableWindowButtons(IntPtr gameWindowHandle)
        {
            int currentStyle = GetWindowLong(gameWindowHandle, GWL_STYLE);
            int newStyle = currentStyle & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SYSMENU;
            SetWindowLong(gameWindowHandle, GWL_STYLE, newStyle);
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}










