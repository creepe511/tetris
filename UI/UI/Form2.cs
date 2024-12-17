using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UI
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
        }
        private void LoadHistory()
        {
            History history = new History();
            history.connect();
            string sql = "select*from test";
            SqlDataReader reader = history.read(sql);
            while (reader.Read())
            {
                dgv.Rows.Add(reader[0].ToString(), reader[1].ToString(), reader[2].ToString());
            }
            reader.Close();
            history.HistoryClose();
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
            LoadHistory();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Form1 form1 = new Form1();
            form1.Show();
            form1.Location = this.Location;
            this.Hide();
        }
    }
}
