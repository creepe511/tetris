using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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

        private void Form4_Load(object sender, EventArgs e)
        {

        }

        private void Form4_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // 设定 SFML 游戏可执行文件的路径
            string gameExecutablePath = @"D:\GitHub\game\x64\Debug\俄罗斯方块2.exe"; // 请根据你的实际路径修改

            // 启动 SFML 游戏作为外部进程
            gameProcess = new Process();
            gameProcess.StartInfo.FileName = gameExecutablePath;

            // 确保 SFML 游戏窗口可以被嵌入到 Panel 中
            gameProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;  // 确保 SFML 窗口不弹出一个独立窗口
            gameProcess.EnableRaisingEvents = true;

            // 将 SFML 游戏的窗口句柄嵌入到 WinForms Panel 控件
            gameProcess.Start();
            EmbedGameWindow(gameProcess.MainWindowHandle);
        }

        private void EmbedGameWindow(IntPtr gameWindowHandle)
        {
            // 获取 Panel 控件的窗口句柄
            IntPtr panelHandle = this.panel1.Handle;

            // 使用 P/Invoke 将 SFML 游戏窗口嵌入到 Panel 控件中
            SetParent(gameWindowHandle, panelHandle);

            // 设置 SFML 游戏窗口的大小和位置，以匹配 Panel 的大小
            MoveWindow(gameWindowHandle, 0, 0, panel1.Width, panel1.Height, true);
        }

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndParent);

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern bool MoveWindow(IntPtr hWnd, int x, int y, int width, int height, bool repaint);
    }
}

