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
        public double lambda { get; set; } = 0.1;
        public int lossFunctionStyle { get; set; } = 0;
        public int normalisationStyle { get; set; } = 0;
        public int numar_maxim_antrenari { get; set; } = 200;
        public int procent_precizie_antrenament { get; set; } = 95;

        public EnvironmentData() { }
        public EnvironmentData(double nlambda, int nlossFunctionStyle, int nnormalisationStyle, int numarMaximAntrenari, int procentPrecizieAntrenament)
        {
            lambda = nlambda;
            lossFunctionStyle = nlossFunctionStyle;
            normalisationStyle = nnormalisationStyle;
            numar_maxim_antrenari = numarMaximAntrenari;
            procent_precizie_antrenament = procentPrecizieAntrenament;
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
