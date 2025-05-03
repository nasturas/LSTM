using ScottPlot.WinForms;

namespace WinFormsGui
{

    public partial class WinNetFormGui : Form
    {

        public WinNetFormGui()
        {
            InitializeComponent();
            plot = new FormsPlot { Dock = DockStyle.Fill };
            PlotPanel.Controls.Add(plot);
            double[] x = { 1, 2, 3, 4, 5 };  // Valori pe axa X

            plot.Plot.Add.Scatter(x, valori);
            plot.Refresh();
        }

    }
}