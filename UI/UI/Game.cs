using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UI
{
    public partial class Game : Form
    {
        private Process gameProcess;

        public Game()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
        }

        // 窗口加载时的处理
        private void Form4_Load(object sender, EventArgs e)
        {
        }

        // 窗口关闭时的处理
        private void Form4_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 确保游戏进程退出
            if (gameProcess != null && !gameProcess.HasExited)
            {
                gameProcess.Kill();
                gameProcess.WaitForExit(); // 等待进程完全退出
            }
            Application.Exit();
        }

        // 按钮点击时启动或重启游戏进程
        private async void button1_Click(object sender, EventArgs e)
        {
            // 禁用按钮，避免重复点击
            button1.Enabled = false;

            // 获取 CurrentUser 的值
            string currentUser = LoginPage.CurrentUser;

            // 获取当前程序的启动目录
            string currentDirectory = Application.StartupPath;

            // 构建游戏可执行文件的完整路径
            string gameExecutablePath = Path.Combine(currentDirectory, "setup", "俄罗斯方块2.exe");

            // 结束现有的游戏进程（如果存在）
            if (gameProcess != null && !gameProcess.HasExited)
            {
                gameProcess.Kill();
                gameProcess.WaitForExit(); // 等待进程完全退出
            }

            // 启动游戏并传递 CurrentUser 作为命令行参数
            string arguments = $"--user={currentUser}";  // 构造命令行参数

            gameProcess = new Process();
            gameProcess.StartInfo.FileName = gameExecutablePath;
            gameProcess.StartInfo.Arguments = arguments;  // 添加命令行参数
            gameProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName(gameExecutablePath); // 设置工作目录为游戏文件夹
            gameProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden; // 隐藏黑色控制台窗口
            gameProcess.Start();

            // 等待游戏窗口加载并嵌入到 Panel 控件
            await Task.Run(() =>
            {
                while (gameProcess.MainWindowHandle == IntPtr.Zero)
                {
                    Thread.Sleep(100); // 等待游戏窗口句柄
                }
            });

            // 嵌入游戏窗口到 Panel 中
            EmbedGameWindow(gameProcess.MainWindowHandle);

            // 游戏嵌入后，将 panel1 设置为不透明
            panel1.BackColor = Color.White;  // 设置为不透明颜色，或者其他操作
            button1.Enabled = true; // 启用按钮
        }



        private void button2_Click(object sender, EventArgs e)
        {
            // 结束游戏进程
            if (gameProcess != null && !gameProcess.HasExited)
            {
                gameProcess.Kill();
                gameProcess.WaitForExit(); // 等待进程完全退出
            }

            // 返回 Form1
            MainMenu form1 = new MainMenu();  // 假设 Form1 是你的第一个窗体
            form1.Show();
            this.Hide();  // 隐藏当前窗体
        }

        // 游戏进程退出后的事件处理
        private void GameProcess_Exited(object sender, EventArgs e)
        {
            // 确保在主线程上执行 UI 更新
            this.Invoke((Action)(() =>
            {
                // 游戏结束时启用按钮
                button1.Enabled = true;

                // 允许窗口重新响应
                this.Enabled = true; 



                // 这里可以做一些清理工作，比如恢复窗体UI等
                panel1.BackColor = Color.Transparent;  // 如果你想在游戏结束后恢复透明效果
            }));
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
    }
}



















