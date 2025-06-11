using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace WinFormsGui
{
    internal class EnvironmentData
    {
        public double Lambda { get; set; } = 0.1;
        public int LossFunctionStyle { get; set; } = 0;
        public int NormalisationStyle { get; set; } = 0;
        public int NumarMaximAntrenari { get; set; } = 200;
        public int ProcentPrecizieAntrenament { get; set; } = 95;

        public EnvironmentData() { }
        public EnvironmentData(double lambda, int lossFunctionStyle, int normalisationStyle, int numarMaximAntrenari, int procentPrecizieAntrenament)
        {
            Lambda = lambda;
            LossFunctionStyle = lossFunctionStyle;
            NormalisationStyle = normalisationStyle;
            NumarMaximAntrenari = numarMaximAntrenari;
            ProcentPrecizieAntrenament = procentPrecizieAntrenament;
        }

        /// <summary>
        /// Serializeaza in format JSON parametrii obiectului
        /// </summary>
        /// <returns>un string in format JSON</returns>
        public string Serialize()
        {
            return JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
        }
    }
}
