

using ScottPlot.WinForms;


namespace WinFormsGui
{
    public enum DataNormalisationStyle
    {
        Logaritm,
        ZScore
    };
    public enum LossFunctionStyle
    {
        MSE
    }
    partial class WinNetFormGui
    {
        int[] valori = { 1, 2, 3, 4, 5 }; // Vector cu valori presetate

        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private FormsPlot plot;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            menuStrip1 = new MenuStrip();
            fileToolStripMenuItem = new ToolStripMenuItem();
            exitToolStripMenuItem = new ToolStripMenuItem();
            setariToolStripMenuItem = new ToolStripMenuItem();
            preferinteToolStripMenuItem = new ToolStripMenuItem();
            ajutorToolStripMenuItem1 = new ToolStripMenuItem();
            mainPanel = new Panel();
            tableLayoutPanel1 = new TableLayoutPanel();
            panel1 = new Panel();
            label2 = new Label();
            numarZileTB = new TextBox();
            EstimeazaPretBt = new Button();
            AntreneazaReteaBt = new Button();
            LoadStockPriceBt = new Button();
            label1 = new Label();
            tickerTB = new TextBox();
            PlotPanel = new Panel();
            checkBox1 = new CheckBox();
            menuStrip1.SuspendLayout();
            mainPanel.SuspendLayout();
            tableLayoutPanel1.SuspendLayout();
            panel1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.ImageScalingSize = new Size(24, 24);
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, setariToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(1571, 33);
            menuStrip1.TabIndex = 4;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { exitToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new Size(68, 29);
            fileToolStripMenuItem.Text = "Fisier";
            // 
            // exitToolStripMenuItem
            // 
            exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            exitToolStripMenuItem.Size = new Size(141, 34);
            exitToolStripMenuItem.Text = "Exit";
            exitToolStripMenuItem.Click += exitToolStripMenuItem_Click;
            // 
            // setariToolStripMenuItem
            // 
            setariToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { preferinteToolStripMenuItem, ajutorToolStripMenuItem1 });
            setariToolStripMenuItem.Name = "setariToolStripMenuItem";
            setariToolStripMenuItem.Size = new Size(72, 29);
            setariToolStripMenuItem.Text = "Setari";
            // 
            // preferinteToolStripMenuItem
            // 
            preferinteToolStripMenuItem.Name = "preferinteToolStripMenuItem";
            preferinteToolStripMenuItem.Size = new Size(189, 34);
            preferinteToolStripMenuItem.Text = "Preferinte";
            preferinteToolStripMenuItem.Click += preferinteToolStripMenuItem_Click;
            // 
            // ajutorToolStripMenuItem1
            // 
            ajutorToolStripMenuItem1.Name = "ajutorToolStripMenuItem1";
            ajutorToolStripMenuItem1.Size = new Size(189, 34);
            ajutorToolStripMenuItem1.Text = "Ajutor";
            ajutorToolStripMenuItem1.Click += ajutorToolStripMenuItem1_Click;
            // 
            // mainPanel
            // 
            mainPanel.Controls.Add(tableLayoutPanel1);
            mainPanel.Dock = DockStyle.Fill;
            mainPanel.Location = new Point(0, 33);
            mainPanel.Name = "mainPanel";
            mainPanel.Size = new Size(1571, 792);
            mainPanel.TabIndex = 5;
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 2;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.Controls.Add(panel1, 0, 0);
            tableLayoutPanel1.Controls.Add(PlotPanel, 1, 0);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 1;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.Size = new Size(1571, 792);
            tableLayoutPanel1.TabIndex = 0;
            // 
            // panel1
            // 
            panel1.Controls.Add(checkBox1);
            panel1.Controls.Add(label2);
            panel1.Controls.Add(numarZileTB);
            panel1.Controls.Add(EstimeazaPretBt);
            panel1.Controls.Add(AntreneazaReteaBt);
            panel1.Controls.Add(LoadStockPriceBt);
            panel1.Controls.Add(label1);
            panel1.Controls.Add(tickerTB);
            panel1.Dock = DockStyle.Fill;
            panel1.Location = new Point(3, 3);
            panel1.Name = "panel1";
            panel1.Size = new Size(779, 786);
            panel1.TabIndex = 1;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(23, 18);
            label2.Name = "label2";
            label2.Size = new Size(253, 25);
            label2.TabIndex = 6;
            label2.Text = "Numar de zile pentru prezicere";
            // 
            // numarZileTB
            // 
            numarZileTB.Location = new Point(305, 18);
            numarZileTB.Name = "numarZileTB";
            numarZileTB.Size = new Size(150, 31);
            numarZileTB.TabIndex = 5;
            // 
            // EstimeazaPretBt
            // 
            EstimeazaPretBt.Location = new Point(147, 353);
            EstimeazaPretBt.Name = "EstimeazaPretBt";
            EstimeazaPretBt.Size = new Size(246, 64);
            EstimeazaPretBt.TabIndex = 4;
            EstimeazaPretBt.Text = "Estimeaza urmatoarele valori";
            EstimeazaPretBt.UseVisualStyleBackColor = true;
            EstimeazaPretBt.Click += EstimeazaPretBt_Click;
            // 
            // AntreneazaReteaBt
            // 
            AntreneazaReteaBt.Location = new Point(147, 249);
            AntreneazaReteaBt.Name = "AntreneazaReteaBt";
            AntreneazaReteaBt.Size = new Size(246, 64);
            AntreneazaReteaBt.TabIndex = 3;
            AntreneazaReteaBt.Text = "Antreneaza retea";
            AntreneazaReteaBt.UseVisualStyleBackColor = true;
            AntreneazaReteaBt.Click += AntreneazaReteaBt_Click;
            // 
            // LoadStockPriceBt
            // 
            LoadStockPriceBt.Location = new Point(147, 141);
            LoadStockPriceBt.Name = "LoadStockPriceBt";
            LoadStockPriceBt.Size = new Size(246, 64);
            LoadStockPriceBt.TabIndex = 2;
            LoadStockPriceBt.Text = "Incarca valori istorice";
            LoadStockPriceBt.UseVisualStyleBackColor = true;
            LoadStockPriceBt.Click += LoadStockPriceBt_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(23, 61);
            label1.Name = "label1";
            label1.Size = new Size(168, 25);
            label1.TabIndex = 1;
            label1.Text = "Nume ticker actiune";
            // 
            // tickerTB
            // 
            tickerTB.Location = new Point(305, 61);
            tickerTB.Name = "tickerTB";
            tickerTB.Size = new Size(150, 31);
            tickerTB.TabIndex = 0;
            // 
            // PlotPanel
            // 
            PlotPanel.Dock = DockStyle.Fill;
            PlotPanel.Location = new Point(788, 3);
            PlotPanel.Name = "PlotPanel";
            PlotPanel.Size = new Size(780, 786);
            PlotPanel.TabIndex = 0;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(556, 60);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(124, 29);
            checkBox1.TabIndex = 7;
            checkBox1.Text = "multi input";
            checkBox1.UseVisualStyleBackColor = true;
            // 
            // WinNetFormGui
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1571, 825);
            Controls.Add(mainPanel);
            Controls.Add(menuStrip1);
            MainMenuStrip = menuStrip1;
            Margin = new Padding(4);
            Name = "WinNetFormGui";
            Text = "PrezicerePretActiune";
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            mainPanel.ResumeLayout(false);
            tableLayoutPanel1.ResumeLayout(false);
            panel1.ResumeLayout(false);
            panel1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem setariToolStripMenuItem;
        private ToolStripMenuItem preferinteToolStripMenuItem;
        private ToolStripMenuItem ajutorToolStripMenuItem1;
        private Panel mainPanel;
        private TableLayoutPanel tableLayoutPanel1;
        private Panel PlotPanel;
        private Panel panel1;
        private Label label1;
        private TextBox tickerTB;
        private Button AntreneazaReteaBt;
        private Button LoadStockPriceBt;
        private Button EstimeazaPretBt;
        private ToolStripMenuItem exitToolStripMenuItem;
        private Label label2;
        private TextBox numarZileTB;
        private CheckBox checkBox1;
    }
}