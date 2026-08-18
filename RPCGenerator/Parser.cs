using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    class Parser
    {
        public string Name { get; set; }
        public List<(string type, string name)> Parameters { get; set; } = new();
        public int PacketType { get; set; }

        public static Parser ParseLine(string line)
        {
            string[] words = line.Split(
                new char[] { ' ', ',', '(', ')'},
                StringSplitOptions.RemoveEmptyEntries);

            var result = new Parser();

            result.Name = words[0];
            result.PacketType = int.Parse(words[^1]);

            for (int i = 1; i < words.Length - 1; i += 2)
            {
                string type = words[i];
                string name = words[i + 1];
                result.Parameters.Add((type, name));
            }

            return result;
        }
    }
}
