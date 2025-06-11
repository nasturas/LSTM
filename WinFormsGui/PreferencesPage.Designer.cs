namespace WinFormsGui
{
    partial class PreferencesPage
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            lambdaTB = new TextBox();
            nrMaxAntrTB = new TextBox();
            precizieAntrTB = new TextBox();
            normalisationStyleCB = new ComboBox();
            lossFunctionStyleCB = new ComboBox();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            label4 = new Label();
            label5 = new Label();
            okButton = new Button();
            salveazaButton = new Button();
            SuspendLayout();
            // 
            // lambdaTB
            // 
            lambdaTB.Location = new Point(337, 28);
            lambdaTB.Name = "lambdaTB";
            lambdaTB.Size = new Size(182, 31);
            lambdaTB.TabIndex = 0;
            // 
            // nrMaxAntrTB
            // 
            nrMaxAntrTB.Location = new Point(337, 173);
            nrMaxAntrTB.Name = "nrMaxAntrTB";
            nrMaxAntrTB.Size = new Size(182, 31);
            nrMaxAntrTB.TabIndex = 3;
            // 
            // precizieAntrTB
            // 
            precizieAntrTB.Location = new Point(337, 220);
            precizieAntrTB.Name = "precizieAntrTB";
            precizieAntrTB.Size = new Size(182, 31);
            precizieAntrTB.TabIndex = 4;
            // 
            // normalisationStyleCB
            // 
            normalisationStyleCB.FormattingEnabled = true;
            normalisationStyleCB.Location = new Point(337, 75);
            normalisationStyleCB.Name = "normalisationStyleCB";
            normalisationStyleCB.Size = new Size(182, 33);
            normalisationStyleCB.TabIndex = 5;
            // 
            // lossFunctionStyleCB
            // 
            lossFunctionStyleCB.FormattingEnabled = true;
            lossFunctionStyleCB.Location = new Point(337, 124);
            lossFunctionStyleCB.Name = "lossFunctionStyleCB";
            lossFunctionStyleCB.Size = new Size(182, 33);
            lossFunctionStyleCB.TabIndex = 6;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(47, 28);
            label1.Name = "label1";
            label1.Size = new Size(151, 25);
            label1.TabIndex = 7;
            label1.Text = "Viteza de invatare";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(47, 76);
            label2.Name = "label2";
            label2.Size = new Size(158, 25);
            label2.TabIndex = 8;
            label2.Text = "Tip de normalizare";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(47, 124);
            label3.Name = "label3";
            label3.Size = new Size(163, 25);
            label3.TabIndex = 9;
            label3.Text = "Functie de pierdere";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(47, 172);
            label4.Name = "label4";
            label4.Size = new Size(277, 25);
            label4.TabIndex = 10;
            label4.Text = "Numarul maxim de antrenamente";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(47, 220);
            label5.Name = "label5";
            label5.Size = new Size(260, 25);
            label5.TabIndex = 11;
            label5.Text = "Precizie antrenament in procent";
            // 
            // okButton
            // 
            okButton.Location = new Point(388, 285);
            okButton.Name = "okButton";
            okButton.Size = new Size(112, 34);
            okButton.TabIndex = 12;
            okButton.Text = "OK";
            okButton.UseVisualStyleBackColor = true;
            okButton.Click += okButton_Click;
            // 
            // salveazaButton
            // 
            salveazaButton.Location = new Point(520, 285);
            salveazaButton.Name = "salveazaButton";
            salveazaButton.Size = new Size(112, 34);
            salveazaButton.TabIndex = 13;
            salveazaButton.Text = "Salveaza";
            salveazaButton.UseVisualStyleBackColor = true;
            salveazaButton.Click += salveazaButton_Click;
            // 
            // PreferencesPage
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            Controls.Add(salveazaButton);
            Controls.Add(okButton);
            Controls.Add(label5);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(lossFunctionStyleCB);
            Controls.Add(normalisationStyleCB);
            Controls.Add(precizieAntrTB);
            Controls.Add(nrMaxAntrTB);
            Controls.Add(lambdaTB);
            Name = "PreferencesPage";
            Size = new Size(667, 346);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox lambdaTB;
        private TextBox nrMaxAntrTB;
        private TextBox precizieAntrTB;
        private ComboBox normalisationStyleCB;
        private ComboBox lossFunctionStyleCB;
        private Label label1;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label5;
        private Button okButton;
        private Button salveazaButton;
    }
}
