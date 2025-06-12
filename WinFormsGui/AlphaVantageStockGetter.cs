using System;
using System.Collections.Generic;
using System.Globalization;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;


namespace WinFormsGui
{
    internal class AlphaVantageStockGetter : IGetStockPrice
    {
        private static readonly HttpClient client = new HttpClient();
        private readonly string apiKey;

        public AlphaVantageStockGetter(string apiKey)
        {
            this.apiKey = apiKey;
        }

        public async Task<List<(DateTime Date, double Close)>> GetClosePrices(string ticker, DateTime fromDate)
        {
            var closes = new List<(DateTime Date, double Close)>();
            string url = $"https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={ticker}&apikey={apiKey}&outputsize=full";

            var response = await client.GetAsync(url);
            if (!response.IsSuccessStatusCode)
                throw new Exception($"Eroare API: {response.StatusCode}");

            var json = await response.Content.ReadAsStringAsync();
            using JsonDocument doc = JsonDocument.Parse(json);

            if (!doc.RootElement.TryGetProperty("Time Series (Daily)", out var timeSeries))
                throw new Exception("Structura JSON invalida sau limita de API atinsa.");

            foreach (var day in timeSeries.EnumerateObject())
            {
                if (DateTime.TryParse(day.Name, out DateTime date) && date >= fromDate)
                {
                    if (day.Value.TryGetProperty("4. close", out var closeProp) &&
                        double.TryParse(closeProp.GetString(), NumberStyles.Any, CultureInfo.InvariantCulture, out double close))
                    {
                        closes.Add((date, close));
                    }
                }
            }

            // Opțional: sortează crescător după dată
            closes.Sort((a, b) => a.Date.CompareTo(b.Date));

            return closes;
        }

    }
}