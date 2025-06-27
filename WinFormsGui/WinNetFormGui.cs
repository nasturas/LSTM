using ScottPlot.WinForms;
using System.Diagnostics;
using System.Text.Json;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ToolTip;

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
        private List<(DateTime Date, double Close)> priceList = new List<(DateTime Date, double Close)>();
        private int numarZilePrezicere = 5;
        string exePath = Path.GetFullPath(Path.Combine(Application.StartupPath, @"PrezicerePretActiuni.exe"));
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
            int rtn = await PlotPrices();
        }

        private async Task<int> PlotPrices()
        {
            try
            {
                if (tickerTB.Text.Length == 0)
                {
                    MessageBox.Show("Nu ati ales o actiune", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return 1;
                }
                DateTime startDate = new DateTime(2024, 1, 1);
                priceList = await fetcher.GetClosePrices(tickerTB.Text, startDate);

                double[] xValues = priceList.Select(c => c.Date.ToOADate()).ToList().ToArray();
                double[] yValues = priceList.Select(c => c.Close).ToList().ToArray();
                plot.Plot.Clear();
                plot.Plot.Add.Scatter(xValues, yValues);
                plot.Plot.Axes.Bottom.TickGenerator = new ScottPlot.TickGenerators.DateTimeAutomatic();
                plot.Plot.Axes.AutoScale();
                plot.Refresh();
                return 0;

            }
            catch (Exception ex)
            {
                MessageBox.Show("Eroare: " + ex.Message);
                return 1;
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

        private async void EstimeazaPretBt_Click(object sender, EventArgs e)
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
            if (plot.Plot.GetPlottables().Count() == 0)
            {
                int rtnplot = await PlotPrices();
            }
            if (numarZilePrezicere>priceList.Count)
            {
                MessageBox.Show("Numarul de zile nu poate fi mai mare decat numarul de zile de cand actiunea e pe bursa.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            int indexStart = priceList.Count-numarZilePrezicere;
            List<double> inputList = priceList.GetRange(indexStart, numarZilePrezicere).Select(c => c.Close).ToList();


            var output = new
            {
                features = new
                {
                    feature1 = inputList
                }
            };

            // Serializare în string JSON
            string jsonString = JsonSerializer.Serialize(output, new JsonSerializerOptions { WriteIndented = true });
            //TODO. muta numele la fisiere in variabile de mediu ceva sa nu mai fie hardcoded.
            File.WriteAllText("inputs.json", jsonString);

            CallExe(LSTMState.Forward);
            //citeste din output.json si scrie cu rosu noul punct.
            string json = File.ReadAllText("output.json");
            using JsonDocument doc = JsonDocument.Parse(json);
            if (!doc.RootElement.TryGetProperty("rezultat", out var rezultat))
            {
                MessageBox.Show("Nu pot citi rezulatul estimarii!");
                return;
            }
            double doubleResult=0.0;
            try
            {
                doubleResult = rezultat.GetDouble();
                MessageBox.Show($"Estimare: {doubleResult}", "INfo", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex) { }
            DateTime dateTime = priceList[priceList.Count-1].Date.AddDays(1);
            var redPoint = plot.Plot.Add.Scatter(
                xs: new double[] { priceList[priceList.Count - 1].Date.ToOADate(), dateTime.ToOADate() },
                ys: new double[] { priceList[priceList.Count - 1].Close, doubleResult }
                );
            redPoint.Color = ScottPlot.Color.FromColor(System.Drawing.Color.Red);
            plot.Plot.Axes.AutoScale();
            plot.Refresh();
        }

        private void CallExe(LSTMState state)
        {
            string args;

            switch (state)
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
        private async void AntreneazaReteaBt_Click(object sender, EventArgs e)
        {


            if(plot.Plot.GetPlottables().Count()==0)
            {
                int plotrtn = await PlotPrices();
            }

            //cream un json numit training.json care sa aiba datele de antrenament

            var output = new
            {
                features = new
                {
                    feature1 = priceList.Select(c => c.Close).ToList()
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