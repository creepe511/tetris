using System;
using System.Data.SQLite;
using System.IO;
using System.Windows.Forms;

namespace UI
{
    internal static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            // 初始化数据库（如果没有则创建）
            InitializeDatabase();

            // 启动应用程序的界面
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new LoginPage());
        }

        // 获取数据库文件路径（在当前应用程序目录的 users 文件夹中创建数据库）
        public static string GetDatabasePath()  // 修改为 public
        {
            // 获取应用程序的当前目录
            string directoryPath = Application.StartupPath;

            // 定义 users 文件夹的路径
            string usersFolderPath = Path.Combine(directoryPath, "users");

            // 如果 users 目录不存在，则创建该目录
            if (!Directory.Exists(usersFolderPath))
            {
                Directory.CreateDirectory(usersFolderPath);
            }

            // 返回数据库文件的完整路径（数据库存放在 users 文件夹中）
            return Path.Combine(usersFolderPath, "mydatabase.db");
        }

        // 初始化数据库，创建表（如果没有），并确保不删除已有数据
        static void InitializeDatabase()
        {
            string dbPath = GetDatabasePath();

            // 获取数据库路径
            string connectionString = $"Data Source={dbPath};Version=3;";

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                // 创建表的 SQL 查询语句（如果表不存在则创建）
                string createTableQuery = @"
                CREATE TABLE IF NOT EXISTS users (
                    username TEXT NOT NULL UNIQUE,
                    password TEXT NOT NULL
                );
                ";

                using (var cmd = new SQLiteCommand(createTableQuery, connection))
                {
                    cmd.ExecuteNonQuery();
                }

                // 插入默认用户（如果用户表为空且没有 alice 用户，则插入默认用户）
                string insertQuery = @"
                INSERT OR IGNORE INTO users (username, password) 
                VALUES ('alice', 'password123');
                ";

                using (var cmd = new SQLiteCommand(insertQuery, connection))
                {
                    cmd.ExecuteNonQuery();
                }

                connection.Close();
            }
        }
    }
}














