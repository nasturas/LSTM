using ScottPlot.WinForms;
using System.Diagnostics;

namespace WinFormsGui
{

    public partial class WinNetFormGui : Form
    {
        private IGetStockPrice fetcher;
        private PreferencesPage preferencesPage;
        private List<double> priceList = new List<double>();
        public WinNetFormGui()
        {
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
            string exePath = @"\..\..\..\miau.exe"; 
            int nrvar = 123; 

            
            string args = $"-c=\"envData.json\" -i={nrvar} -forward=\"input.json\"";

            ProcessStartInfo psi = new ProcessStartInfo
            {
                FileName = exePath,
                Arguments = args,
                UseShellExecute = false,       
                CreateNoWindow = true,         
                RedirectStandardOutput = true, 
                RedirectStandardError = true
            };

            using Process process = new Process();
            process.StartInfo = psi;

            process.Start();

           
            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();

            process.WaitForExit();

            
            MessageBox.Show($"Output: {output}\nErori: {error}");
        }
    }
    
}