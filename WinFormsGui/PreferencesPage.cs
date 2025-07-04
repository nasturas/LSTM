﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinFormsGui
{
    public partial class PreferencesPage : UserControl
    {
        WinNetFormGui mainePage;
        public PreferencesPage(WinNetFormGui mainePage)
        {
            InitializeComponent();
            normalisationStyleCB.Items.AddRange(Enum.GetNames(typeof(DataNormalisationStyle)));
            lossFunctionStyleCB.Items.AddRange(Enum.GetNames(typeof(LossFunctionStyle)));
            normalisationStyleCB.SelectedIndex = 0;
            lossFunctionStyleCB.SelectedIndex = 0;
            this.mainePage = mainePage;
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            this.Visible = false;
            mainePage.ShowMainPage();
        }

        private void salveazaButton_Click(object sender, EventArgs e)
        {
            EnvironmentData envData = new EnvironmentData();
            double valoare;
            int valint;
            if(double.TryParse(lambdaTB.Text, out valoare))
            {
                envData.lambda = valoare;
            }

            envData.normalisationStyle=normalisationStyleCB.SelectedIndex;
            envData.lossFunctionStyle=lossFunctionStyleCB.SelectedIndex;

            if(int.TryParse(nrMaxAntrTB.Text, out valint))
            {
                envData.numar_maxim_antrenari = valint;
            }
            if (int.TryParse(precizieAntrTB.Text, out valint))
            {
                envData.procent_precizie_antrenament = valint;
            }
            File.WriteAllText("envData.json", envData.Serialize());
        }
    }
}
