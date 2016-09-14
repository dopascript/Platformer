using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PlatformerEditor.Data
{
    public class Level
    {
        public string Path { get; set; }

        public Color BackgroundColor { get; set; }
        public List<Map> Maps { get; set; }

        public Dictionary<Point, Dictionary<string, string>> ObjectsInfo { get; set; }
    }
}
