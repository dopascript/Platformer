using PlatformerEditor.Data;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PlatformerEditor
{
    public partial class Form1 : Form
    {
        Level _level;
        LevelViewer _levelViewer;
        public Form1()
        {
            InitializeComponent();

            _levelViewer = new LevelViewer();
            elementHost1.Child = _levelViewer;
            _levelViewer.onPointerChangePosition += levelViewer_onPointerChangePosition;
        }

        private void levelViewer_onPointerChangePosition(Point position)
        {
            toolStripStatusLabel1.Text = "Position : " + position.X.ToString() + "," + position.Y.ToString();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            _level = LevelHelper.CreateNewLevel(200,40);

            _levelViewer.setLevel(_level);
            updateMapSelector();
        }

        private void toolStripButtonSave_Click(object sender, EventArgs e)
        {
            DirectoryInfo directory = (new FileInfo(_level.Path)).Directory;
            string filePath = System.IO.Path.Combine(directory.FullName, _level.Path);
            LevelHelper.SaveJson(_level, filePath);
        }

        private void toolStripButtonLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog lOpenFileDialog = new OpenFileDialog();
            if (lOpenFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                _level = LevelHelper.LoadJson(lOpenFileDialog.FileName);
                _levelViewer.setLevel(_level);
                updateMapSelector();
            }
        }

        private void updateMapSelector()
        {
            cmbMapSelector.Items.Clear();
            foreach(var map in _level.Maps)
            {
                cmbMapSelector.Items.Add(map.Name);
            }
        }

        private void cmbMapSelector_SelectedIndexChanged(object sender, EventArgs e)
        {
            var map = _level.Maps.First(m => m.Name == cmbMapSelector.SelectedItem.ToString());
            _levelViewer.CurrentSelectedMap = map.Name;

            DirectoryInfo directory = (new FileInfo(_level.Path)).Directory;
            string filePath = System.IO.Path.Combine(directory.Parent.FullName, map.ImagePath);
            pictureBoxTileSelector.Image = Bitmap.FromFile(filePath);
        }

        private void pictureBoxTileSelector_MouseDown(object sender, MouseEventArgs e)
        {
            var map = _level.Maps.First(m => m.Name == cmbMapSelector.SelectedItem.ToString());
            int x = e.X / map.TileSize;
            int y = e.Y / map.TileSize;
            _levelViewer.SelectedTileNumber = y * map.TilesYCountInTexture + x;
        }

    }
}
