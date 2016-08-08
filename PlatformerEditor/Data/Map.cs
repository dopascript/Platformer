using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PlatformerEditor.Data
{
    public class Map
    {
        public int TileSize { get; set; }
        public int TileSizeInTexture { get; set; }
        public int TilesYCountInTexture { get; set; }
        public Size Size { get; set; }
        public string Name { get; set; }
        public int[] Data { get; set; }
        public string ImagePath { get; set; }
    }
}
