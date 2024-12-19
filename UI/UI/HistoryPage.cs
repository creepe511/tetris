using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Data.SQLite;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
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

        }

        private void Form2_Load(object sender, EventArgs e)
        {
            dgv.AllowUserToAddRows = false;
            dgv.Rows.Clear();
            LoadUserGameRecords();
        }

        // 加载当前用户的游戏记录到 DataGridView
        private void LoadUserGameRecords()
        {
            string username = LoginPage.CurrentUser;  // 获取当前登录用户
            if (string.IsNullOrEmpty(username))
            {
                MessageBox.Show("用户未登录！");
                return;
            }

            string connectionString = $"Data Source={GetDatabasePath()};Version=3;";

            using (var connection = new SQLiteConnection(connectionString))
            {
                connection.Open();

                // 查询包含用户名和其他字段
                string query = "SELECT username, start_time, score, duration, note FROM game_records WHERE username = @username";

                using (var cmd = new SQLiteCommand(query, connection))
                {
                    cmd.Parameters.AddWithValue("@username", username);

                    SQLiteDataAdapter dataAdapter = new SQLiteDataAdapter(cmd);
                    DataTable dataTable = new DataTable();
                    dataAdapter.Fill(dataTable);

                    dgv.Rows.Clear(); // 清空现有的行

                    foreach (DataRow row in dataTable.Rows)
                    {
                        // 添加行到 DataGridView
                        dgv.Rows.Add(row["username"], row["start_time"], row["score"], row["duration"], row["note"]);
                    }
                }
            }
        }


        // 获取数据库文件路径
        private string GetDatabasePath()
        {
            return Program.GetDatabasePath();  // 使用 Program 中的 GetDatabasePath 方法
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

        }
    }
}
