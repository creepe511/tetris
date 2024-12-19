using System;
using System.Data.SQLite;
using System.IO;
using System.Windows.Forms;

namespace UI
{
    public partial class LoginPage : Form
    {
        public static string CurrentUser { get; set; }  // 静态变量用于保存当前用户名

        public LoginPage()
        {
            InitializeComponent();
            // 维持窗口大小恒定
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.StartPosition = FormStartPosition.CenterScreen;
        }

        private void label4_Click(object sender, EventArgs e)
        {
            RegistrationPage form2 = new RegistrationPage();
            form2.Show();
            form2.Location = this.Location;
            this.Hide();
        }

        private void Form5_Load(object sender, EventArgs e)
        {
            InitializeDatabase();
        }

        private void InitializeDatabase()
        {
            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";

            using (var connection = new SQLiteConnection(connectionString))
            {
                try
                {
                    connection.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("数据库连接失败: " + ex.Message);
                }
            }
        }

        private string GetDatabasePath()
        {
            // 获取当前应用程序的目录
            string directoryPath = Application.StartupPath;

            // 设置 users 文件夹路径
            string usersFolderPath = Path.Combine(directoryPath, "users");

            // 如果目录不存在，则创建该目录
            if (!Directory.Exists(usersFolderPath))
            {
                Directory.CreateDirectory(usersFolderPath);
            }

            // 返回数据库文件的完整路径
            return Path.Combine(usersFolderPath, "mydatabase.db");
        }

        private void LoginPage_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string username = textBox1.Text;
            string password = textBox2.Text;

            if (IsUsernameExist(username))
            {
                if (IsValidUser(username, password))
                {
                    CurrentUser = username;  // 保存当前登录的用户名
                    MessageBox.Show("登录成功");

                    MainMenu form2 = new MainMenu();
                    form2.Show();
                    form2.Location = this.Location;
                    this.Hide();
                    return;
                }
                textBox2.Clear();
                MessageBox.Show("密码错误");
                return;
            }

            textBox1.Clear();
            textBox2.Clear();
            MessageBox.Show("用户名不存在");
            return;
        }


        // 检查用户名和密码是否匹配
        private bool IsValidUser(string username, string password)
        {
            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                // 查询用户名和密码是否匹配
                string selectQuery = "SELECT COUNT(*) FROM users WHERE username = @username AND password = @password";
                using (var cmd = new SQLiteCommand(selectQuery, connection))
                {
                    // 使用参数化查询
                    cmd.Parameters.AddWithValue("@username", username);
                    cmd.Parameters.AddWithValue("@password", password);

                    // 执行查询并获取结果
                    int userCount = Convert.ToInt32(cmd.ExecuteScalar());

                    // 如果 userCount > 0，则表示该用户名和密码匹配
                    return userCount > 0;
                }
            }
        }

        // 检查用户名是否存在
        private bool IsUsernameExist(string username)
        {
            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                // 查询数据库，检查是否已存在该用户名
                string selectQuery = "SELECT COUNT(*) FROM users WHERE username = @username";
                using (var cmd = new SQLiteCommand(selectQuery, connection))
                {
                    // 使用参数化查询
                    cmd.Parameters.AddWithValue("@username", username);

                    // 执行查询并获取结果
                    int userCount = Convert.ToInt32(cmd.ExecuteScalar());

                    return userCount > 0;
                }
            }
        }
    }
}

