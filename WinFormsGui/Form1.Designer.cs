

using ScottPlot.WinForms;

namespace WinFormsGui
{
    partial class WinNetFormGui
    {
        int[] valori = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; // Vector cu valori presetate

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
            textBox1 = new TextBox();
            NumeTicker = new Label();
            PlotPanel = new Panel();
            tableLayoutPanel1 = new TableLayoutPanel();
            panel1 = new Panel();
            tableLayoutPanel1.SuspendLayout();
            panel1.SuspendLayout();
            SuspendLayout();
            // 
            // textBox1
            // 
            textBox1.Location = new Point(121, 16);
            textBox1.Name = "textBox1";
            textBox1.Size = new Size(134, 27);
            textBox1.TabIndex = 0;
            // 
            // NumeTicker
            // 
            NumeTicker.AutoSize = true;
            NumeTicker.Location = new Point(13, 16);
            NumeTicker.Name = "NumeTicker";
            NumeTicker.Size = new Size(102, 20);
            NumeTicker.TabIndex = 1;
            NumeTicker.Text = "Ticker Actiune";
            // 
            // PlotPanel
            // 
            PlotPanel.Dock = DockStyle.Fill;
            PlotPanel.Location = new Point(631, 3);
            PlotPanel.Name = "PlotPanel";
            PlotPanel.Size = new Size(623, 324);
            PlotPanel.TabIndex = 2;
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 2;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.Controls.Add(PlotPanel, 1, 0);
            tableLayoutPanel1.Controls.Add(panel1, 0, 0);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 2;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
            tableLayoutPanel1.Size = new Size(1257, 660);
            tableLayoutPanel1.TabIndex = 3;
            // 
            // panel1
            // 
            panel1.Controls.Add(NumeTicker);
            panel1.Controls.Add(textBox1);
            panel1.Dock = DockStyle.Fill;
            panel1.Location = new Point(3, 3);
            panel1.Name = "panel1";
            panel1.Size = new Size(622, 324);
            panel1.TabIndex = 3;
            // 
            // WinNetFormGui
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1257, 660);
            Controls.Add(tableLayoutPanel1);
            Name = "WinNetFormGui";
            Text = "PrezicerePretActiune";
            tableLayoutPanel1.ResumeLayout(false);
            panel1.ResumeLayout(false);
            panel1.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private TextBox textBox1;
        private Label NumeTicker;
        private Panel PlotPanel;
        private TableLayoutPanel tableLayoutPanel1;
        private Panel panel1;
    }
}