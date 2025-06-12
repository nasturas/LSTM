using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsGui
{
    internal interface IGetStockPrice
    {
        Task<List<(DateTime Date, double Close)>> GetClosePrices(string ticker, DateTime fromDate);
    }
}
