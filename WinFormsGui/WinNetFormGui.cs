using ScottPlot.WinForms;
using System.Diagnostics;
using System.Text.Json;

namespace WinFormsGui
{
    public enum LSTMState
    {
        Train,
        Forward
    }
    public partial class WinNetFormGui : Form
    {
        private IGetStockPrice fetcher;
        private PreferencesPage preferencesPage;
        private List<double> priceList = new List<double>();
        private int numarZilePrezicere = 5;
        string exePath = Path.GetFullPath(Path.Combine(Application.StartupPath, @"..\..\..\PrezicerePretActiuni.exe"));
        string exeDir;

        public WinNetFormGui()
        {
            exeDir = Path.GetDirectoryName(exePath);
            fetcher = new AlphaVantageStockGetter("MYNXNXD7L6JM1TQN");
            preferencesPage = new PreferencesPage(this);
            InitializeComponent();
            this.Controls.Add(preferencesPage);

            plot = new FormsPlot { Dock = DockStyle.Fill };
            PlotPanel.Controls.Add(plot);
            double[] x = { 1, 2, 3, 4, 5 };  // Valori pe axa X

            plot.Plot.Add.Scatter(x, valori);
            plot.Refresh();

        }

        public void ShowMainPage()
        {
            mainPanel.Visible = true;
            mainPanel.BringToFront();
        }

        private void ShowPreferencesPage()
        {
            mainPanel.Visible = false;
            preferencesPage.Visible = true;
            preferencesPage.BringToFront();
        }

        private void preferinteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowPreferencesPage();
        }

        private async void LoadStockPriceBt_Click(object sender, EventArgs e)
        {
            try
            {
                DateTime startDate = new DateTime(2024, 1, 1);
                priceList = await fetcher.GetClosePrices(tickerTB.Text, startDate);

                DateTime[] dateArray = new DateTime[priceList.Count];
                for (int i = 0; i < priceList.Count; i++)
                {
                    dateArray[i] = startDate.AddDays(i); // presupunem o zi pentru fiecare valoare
                }
                double[] xValues = dateArray.Select(d => d.ToOADate()).ToArray();
                double[] yValues = priceList.ToArray();
                plot.Plot.Clear();
                plot.Plot.Add.Scatter(xValues, yValues);
                plot.Plot.Axes.Bottom.TickGenerator = new ScottPlot.TickGenerators.DateTimeAutomatic();
                plot.Plot.Axes.AutoScale();
                plot.Refresh();

            }
            catch (Exception ex)
            {
                MessageBox.Show("Eroare: " + ex.Message);
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void ajutorToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Prezicere Pret Actiuni folosind LSTM. Lucrare licenta 2025, Universitatea Bacau", "Informatii", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void EstimeazaPretBt_Click(object sender, EventArgs e)
        {

            if (int.TryParse(numarZileTB.Text, out numarZilePrezicere))
            {
                //nothing all good
            }
            else
            {
                numarZilePrezicere = 5;
                MessageBox.Show("Numarul de zile nu e intreg. Implicit vom folosi 5 zile precedente.", "Info", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            //TODO creaza un input.json cu numarZilePrezicere valori.

            CallExe(LSTMState.Forward);
           
        }

        private void CallExe(LSTMState state)
        {
            string args;

            switch(state)
            {
                case LSTMState.Train:
                    args = $"-c=\"envData.json\" -i={numarZilePrezicere} -t=\"training.json\"";
                    break;
                case LSTMState.Forward:
                    args = $"-c=\"envData.json\" -i={numarZilePrezicere} -forward=\"inputs.json\"";
                    break;
                default:
                    args = $"-c=\"envData.json\" -i={numarZilePrezicere} -forward=\"inputs.json\"";
                    break;
            }

            ProcessStartInfo psi = new ProcessStartInfo
            {
                FileName = exePath,
                Arguments = args,
                UseShellExecute = false,
                CreateNoWindow = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                WorkingDirectory = exeDir
            };

            using Process process = new Process();
            process.StartInfo = psi;

            if (!File.Exists(exePath.Trim('"')))
            {
                MessageBox.Show($"Nu s-a gasit fisierul:\n{exePath}");
                return;
            }

            try
            {
                process.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Eroare la pornire: " + ex.Message);
            }


            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();

            process.WaitForExit();

            if (!string.IsNullOrEmpty(output))
            {
                MessageBox.Show($"Output: {output}");
            }
            if (!string.IsNullOrEmpty(error))
            {
                MessageBox.Show($"Erori: {error}");
            }
        }
        private void AntreneazaReteaBt_Click(object sender, EventArgs e)
        {
            if (tickerTB.Text.Length == 0)
            {
                MessageBox.Show("Nu ati ales o actiune", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if(plot.Plot.GetPlottables().Count()==0)
            {
                LoadStockPriceBt_Click(sender, e);
            }

            //cream un json numit training.json care sa aiba datele de antrenament
            
            var output = new
            {
                features = new
                {
                    feature1 = priceList
                }
            };

            // Serializare în string JSON
            string jsonString = JsonSerializer.Serialize(output, new JsonSerializerOptions { WriteIndented = true });
            //TODO. muta numele la fisiere in variabile de mediu ceva sa nu mai fie hardcoded.
            File.WriteAllText("training.json", jsonString);

            if (int.TryParse(numarZileTB.Text, out numarZilePrezicere))
            {
                //nothing all good
            }
            else
            {
                numarZilePrezicere = 5;
                MessageBox.Show("Numarul de zile nu e intreg. Implicit vom folosi 5 zile precedente.", "Info", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            CallExe(LSTMState.Train);
        }
    }

}