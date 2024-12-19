using System;
using System.Data.SQLite;
using System.IO;
using System.Windows.Forms;

namespace UI
{
    public partial class RegistrationPage : Form
    {

        public RegistrationPage()
        {
            InitializeComponent();
            // 维持窗口大小恒定
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.StartPosition = FormStartPosition.CenterScreen;
        }

        private void RegistrationPage_Load(object sender, EventArgs e)
        {
            // 这里不需要做初始化操作，用户表已经创建好
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // 获取用户名和密码
            string username = textBox1.Text;
            string password = textBox2.Text;
            string confirm = textBox3.Text;

            // 检查两次输入的密码是否一致
            if (password != confirm)
            {
                MessageBox.Show("两次密码输入不一致");
                textBox2.Clear();
                textBox3.Clear();
                return;
            }

            // 检查是否输入了用户名和密码
            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password) || string.IsNullOrWhiteSpace(confirm))
            {
                MessageBox.Show("确保正确输入所有字段！", "输入错误", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                textBox2.Clear();
                textBox3.Clear();
                return;
            }

            // 检查用户名是否已存在
            if (IsUsernameExist(username))
            {
                MessageBox.Show("用户名已存在，请选择其他用户名");
                textBox1.Clear();
                textBox2.Clear();
                textBox3.Clear();
                return;
            }

            // 如果用户名不存在，则插入到数据库
            try
            {
                InsertUserIntoDatabase(username, password); // 插入用户信息
                CreateGameRecordsTable(username);           // 创建游戏记录表
                MessageBox.Show("注册成功！");
                LoginPage form3 = new LoginPage();
                form3.Show();
                form3.Location = this.Location;
                this.Hide();
            }
            catch (Exception ex)
            {
                MessageBox.Show("注册失败: " + ex.Message);
            }
        }

        // 获取数据库文件路径（统一使用 Program.GetDatabasePath 方法）
        private string GetDatabasePath()
        {
            return Program.GetDatabasePath();  // 直接调用 Program 中的 GetDatabasePath 方法
        }

        // 检查用户名是否存在（检查 users 表）
        private bool IsUsernameExist(string username)
        {
            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";  // 使用 Program.GetDatabasePath 获取路径

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                // 查询数据库，检查是否已存在该用户名
                string selectQuery = "SELECT COUNT(*) FROM users WHERE username = @username";
                using (var cmd = new SQLiteCommand(selectQuery, connection))
                {
                    cmd.Parameters.AddWithValue("@username", username);

                    int userCount = Convert.ToInt32(cmd.ExecuteScalar());
                    return userCount > 0;
                }
            }
        }

        // 将用户数据插入到数据库（插入到 users 表）
        private void InsertUserIntoDatabase(string username, string password)
        {
            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";  // 使用 Program.GetDatabasePath 获取路径

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                // 插入数据的 SQL 查询语句
                string insertQuery = @"
                    INSERT INTO users (username, password) 
                    VALUES (@username, @password);
                ";

                using (var cmd = new SQLiteCommand(insertQuery, connection))
                {
                    cmd.Parameters.AddWithValue("@username", username);
                    cmd.Parameters.AddWithValue("@password", password);

                    cmd.ExecuteNonQuery();
                }

                connection.Close();
            }
        }

        // 创建游戏记录表（每个用户都有自己的游戏记录表）
        private void CreateGameRecordsTable(string username)
        {
            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";  // 使用 Program.GetDatabasePath 获取路径

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                string createTableQuery = @"
                    CREATE TABLE IF NOT EXISTS game_records (
                        username TEXT NOT NULL, 
                        start_time TEXT, 
                        score INTEGER, 
                        duration TEXT, 
                        note TEXT,
                        PRIMARY KEY (username)
                    );
                ";

                using (var cmd = new SQLiteCommand(createTableQuery, connection))
                {
                    cmd.ExecuteNonQuery();
                }

                string insertQuery = "INSERT INTO game_records (username) VALUES (@username);";
                using (var cmd = new SQLiteCommand(insertQuery, connection))
                {
                    cmd.Parameters.AddWithValue("@username", username);
                    cmd.ExecuteNonQuery();
                }

                connection.Close();
            }
        }

        // 跳转到登录页面
        private void label4_Click(object sender, EventArgs e)
        {
            LoginPage form2 = new LoginPage();
            form2.Show();
            form2.Location = this.Location;
            this.Hide();
        }

        private void RegistrationPage_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }
    }
}









