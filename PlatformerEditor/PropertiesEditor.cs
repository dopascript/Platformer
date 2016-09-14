using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PlatformerEditor
{
    public partial class PropertiesEditor : Form
    {
        public PropertiesEditor()
        {
            InitializeComponent();
        }

        public void setInfo(Dictionary<string, string> info)
        {
            foreach(var key in info.Keys)
            {
                textBoxInfo.Text += key + ":" + info[key] + "\r\n";
            }
        }

        public Dictionary<string, string> getInfo()
        {
            Dictionary<string, string> result = new Dictionary<string, string>();
            foreach (var line in textBoxInfo.Lines)
            {
                if(line.Trim() == "")
                {
                    continue;
                }
                var splittedLine = line.Split(':');
                result.Add(splittedLine[0], splittedLine[1]);
            }
            return result;
        }
    }
}
