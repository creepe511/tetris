using System;
using System.Data;
using System.Data.SQLite;
using System.Windows.Forms;

namespace UI
{
   
    public partial class HistoryPage : Form
    {
        private string selectedStartTime = string.Empty;
        private string dbPath = "";  

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

        private void Form2_Load(object sender, EventArgs e)
        {
            dgv.AllowUserToAddRows = false;
            dgv.Rows.Clear();
            LoadDataFromDatabase();
        }

        // 加载SQLite数据库数据到DataGridView
        private void LoadDataFromDatabase()
        {
            // 如果dbPath为空，直接返回
            if (string.IsNullOrEmpty(dbPath))
            {
                return;
            }

            // 连接到SQLite数据库
            string connectionString = $"Data Source={dbPath};Version=3;";
            using (SQLiteConnection connection = new SQLiteConnection(connectionString))
            {
                try
                {
                    connection.Open();

                    // 检查指定的表是否存在
                    string checkTableQuery = "SELECT name FROM sqlite_master WHERE type='table' AND name='YourTableName';";
                    SQLiteCommand checkTableCmd = new SQLiteCommand(checkTableQuery, connection);
                    var result = checkTableCmd.ExecuteScalar();

                    // 如果表不存在，直接返回
                    if (result == null)
                    {
                        return;
                    }

                    // 如果表存在，从表中获取数据
                    string query = "SELECT * FROM YourTableName;";
                    SQLiteDataAdapter dataAdapter = new SQLiteDataAdapter(query, connection);
                    DataTable dataTable = new DataTable();
                    dataAdapter.Fill(dataTable);

                    // 将数据绑定到DataGridView
                    dgv.DataSource = dataTable;
                }
                catch (Exception)
                {
                    return;
                }
            }
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

        private void dgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                // 获取当前选中行的开始时间
                selectedStartTime = dgv.Rows[e.RowIndex].Cells["StartTime"].Value.ToString();
            }
        }
    }
}




