using System;
using System.Data;
using System.Data.SQLite;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace UI
{
   
    public partial class HistoryPage : Form
    {
        private string selectedStartTime = string.Empty;
        private string updatedRemark = string.Empty;
        private string currentDirectory = Application.StartupPath;
        private Process historyProcess;
        private string dbPath ;  

        public HistoryPage()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            dbPath = Path.Combine(currentDirectory, "data", "database.db");
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
            string username = textBox1.Text;
            if (textBox1.Text == "")
            {
                historyProcess = new Process();
                string historyresetPath = Path.Combine(currentDirectory, "setup", "reset.exe");
                historyProcess.StartInfo.FileName = historyresetPath;  // 设置可执行文件的路径
                historyProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                historyProcess.Start();
                LoadDataFromDatabase();
                return;
            }
            historyProcess = new Process();
            string historysearchPath = Path.Combine(currentDirectory, "setup", "search.exe");
            string arguments = $"--username={username}";
            historyProcess.StartInfo.FileName = historysearchPath;
            historyProcess.StartInfo.Arguments = arguments;
            historyProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            historyProcess.Start();

            textBox1.Clear();
            LoadDataFromDatabase();
        }

        private void dgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                // 获取当前选中行的开始时间
                selectedStartTime = dgv.Rows[e.RowIndex].Cells["开始时间"].Value.ToString();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            historyProcess = new Process();
            string historydeletePath = Path.Combine(currentDirectory, "setup", "delete.exe");
            string arguments = $"--start_time={selectedStartTime}";
            historyProcess.StartInfo.FileName = historydeletePath;
            historyProcess.StartInfo.Arguments = arguments;
            historyProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            historyProcess.Start();
            LoadDataFromDatabase();
        }

        private void dgv_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (e.ColumnIndex == dgv.Columns["备注"].Index)
            {
                historyProcess = new Process();
                // 获取当前行的备注列的修改后值
                var updatedRemark = dgv.Rows[e.RowIndex].Cells["备注"].Value.ToString();

                // 指定外部程序路径
                string historymodifyPath = Path.Combine(currentDirectory, "setup", "modify.exe"); 

                // 构建命令行参数，传递开始时间和备注
                string arguments = $"--start_time={selectedStartTime} --remark={updatedRemark}";

                // 配置启动外部进程的相关信息
                historyProcess.StartInfo.FileName = historymodifyPath;
                historyProcess.StartInfo.Arguments = arguments;
                historyProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                // 启动外部程序
                historyProcess.Start();
                LoadDataFromDatabase();
            }
        }
    }
}




