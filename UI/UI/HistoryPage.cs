using System;
using System.Data;
using System.Data.SQLite;
using System.IO;
using System.Windows.Forms;

namespace UI
{
    public partial class HistoryPage : Form
    {
        public HistoryPage()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void dgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            // 可以实现点击事件的其他逻辑，如果需要
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            dgv.AllowUserToAddRows = false;
            dgv.Rows.Clear();
            CreateDatabase(); // 创建数据库并加载数据
        }

        // 创建数据库和游戏记录表
        private void CreateDatabase()
        {
            // 获取数据库路径，确保它位于当前目录下的 users 文件夹内
            string databasePath = Path.Combine(Directory.GetCurrentDirectory(), "users", "game_records.db");

            // 检查文件夹是否存在，如果不存在则创建
            string directoryPath = Path.GetDirectoryName(databasePath);
            if (!Directory.Exists(directoryPath))
            {
                Directory.CreateDirectory(directoryPath);
            }

            // 如果数据库文件不存在，则创建数据库和表
            if (!File.Exists(databasePath))
            {
                CreateGameRecordsTable(databasePath);
            }
        }

        // 创建游戏记录表
        private void CreateGameRecordsTable(string databasePath)
        {
            using (var connection = new SQLiteConnection($"Data Source={databasePath};Version=3;"))
            {
                connection.Open();

                string createTableQuery = @"
                    CREATE TABLE IF NOT EXISTS game_records (
                        username TEXT NOT NULL, 
                        start_time TEXT NOT NULL, 
                        score INTEGER, 
                        duration TEXT, 
                        note TEXT
                    );
                ";

                using (var command = new SQLiteCommand(createTableQuery, connection))
                {
                    command.ExecuteNonQuery();
                }
            }
        }

        // 获取数据库文件路径
        private string GetDatabasePath()
        {
            // 直接返回数据库文件的路径，确保它在当前目录下的 users 文件夹内
            return Path.Combine(Directory.GetCurrentDirectory(), "users", "game_records.db");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MainMenu form1 = new MainMenu();
            form1.Show();
            form1.Location = this.Location;
            this.Hide();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            // 这里可以实现其他功能，例如刷新数据等
        }
    }
}


