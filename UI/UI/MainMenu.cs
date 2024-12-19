using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UI
{
    public partial class MainMenu : Form
    {
        public MainMenu()
        {
            InitializeComponent();
            //维持窗口大小恒定
            this.FormBorderStyle = FormBorderStyle.FixedDialog;  
            this.MaximizeBox = false; 
            this.MinimizeBox = false;  
            this.StartPosition = FormStartPosition.CenterScreen;
            label1.BackColor = Color.Transparent;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            HistoryPage form2 = new HistoryPage();
            form2.Show();
            form2.Location = this.Location;
            this.Hide();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {
            
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Indication form3 = new Indication();
            form3.Show();
            form3.Location = this.Location;
            this.Hide();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Game form4 = new Game();
            form4.Show();
            form4.Location = this.Location;
            this.Hide();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            LoginPage form5 = new LoginPage();
            form5.Show();
            form5.Location = this.Location;
            this.Hide();
        }
    }
}
