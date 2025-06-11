using ScottPlot.WinForms;

namespace WinFormsGui
{

    public partial class WinNetFormGui : Form
    {
        private IGetStockPrice fetcher;
        public WinNetFormGui()
        {
            IGetStockPrice fetcher = new AlphaVantageStockGetter("MYNXNXD7L6JM1TQN");
            InitializeComponent();
            
            plot = new FormsPlot { Dock = DockStyle.Fill };
            PlotPanel.Controls.Add(plot);
            double[] x = { 1, 2, 3, 4, 5 };  // Valori pe axa X

            plot.Plot.Add.Scatter(x, valori);
            plot.Refresh();
            ShowMainPage();
        }

        public void ShowMainPage()
        {
            mainPanel.Controls.Clear();
        }

        private void ShowPreferencesPage()
        {
            mainPanel.Controls.Clear();
            PreferencesPage preferencesPage = new PreferencesPage(this);
            mainPanel.Controls.Add(preferencesPage);
        }

        private void preferinteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowPreferencesPage();
        }

        private async void LoadStockPriceBt_Click(object sender, EventArgs e)
        {
            try
            {
                var closes = await fetcher.GetClosePrices("AAPL", new DateTime(2024, 1, 1));
                
            }
            catch (Exception ex)
            {
                MessageBox.Show("Eroare: " + ex.Message);
            }
        }
    }
}