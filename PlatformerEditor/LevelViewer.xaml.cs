using PlatformerEditor.Data;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PlatformerEditor
{
    /// <summary>
    /// Interaction logic for LevelViewer.xaml
    /// </summary>
    public partial class LevelViewer : UserControl
    {
        Level _level;
        Dictionary<string, Canvas> _mapViews;
        Dictionary<string, BitmapImage> _textures;

        private string _currentSelectedMap;
        public string CurrentSelectedMap 
        {
            get { return _currentSelectedMap; }
            set { _currentSelectedMap = value; updateTransparence(); }
        }
        public int SelectedTileNumber { get; set; }
        public LevelViewer()
        {
            InitializeComponent();
            CurrentSelectedMap = "";
            SelectedTileNumber = 0;
            _textures = new Dictionary<string, BitmapImage>();
        }

        public void setLevel(Level level)
        {
            _level = level;
            updateLevel();
        }

        BitmapImage getTexture(string path)
        {
            if (!_textures.ContainsKey(path))
            {
                DirectoryInfo directory = (new FileInfo(_level.Path)).Directory;
                string filePath = System.IO.Path.Combine(directory.Parent.FullName, path);
                _textures.Add(path, new BitmapImage(new Uri(filePath)));
            }
            return _textures[path];
        }

        public void updateLevel()
        {
            byte r = _level.BackgroundColor.R; 
            byte g = _level.BackgroundColor.G; 
            byte b = _level.BackgroundColor.B;
            LevelGrid.Background = new SolidColorBrush(Color.FromRgb(r, g, b));

            var collisionMap = _level.Maps.First(m => m.Name == "CollisionMap");

            LevelGrid.Width = collisionMap.Size.Width * collisionMap.TileSize;
            LevelGrid.Height = collisionMap.Size.Height * collisionMap.TileSize;

            LevelMap.Children.Clear();
            LevelMap.Width = collisionMap.Size.Width * collisionMap.TileSize;
            LevelMap.Height = collisionMap.Size.Height * collisionMap.TileSize;

            _mapViews = new Dictionary<string, Canvas>();
            foreach(var map in _level.Maps)
            {
                Canvas newMap = new Canvas();
                newMap.Width = map.Size.Width * map.TileSize;
                newMap.Height = map.Size.Height * map.TileSize;
                newMap.Tag = map;
                _mapViews.Add(map.Name, newMap);
                LevelMap.Children.Add(newMap);

                for (int y = 0; y < map.Size.Height; y++)
                {
                    for (int x = 0; x < map.Size.Width; x++)
                    {
                        int i = y * map.Size.Width + x;
                        int tileNumber = map.Data[i];
                        if (tileNumber != 0)
                        {
                            var tile = CreateTile(map, tileNumber);
                            Canvas.SetLeft(tile, map.TileSize * x);
                            Canvas.SetTop(tile, map.TileSize * y);
                            newMap.Children.Add(tile);
                        }
                    }
                }
            }

            if (!_mapViews.ContainsKey(CurrentSelectedMap))
            {
                CurrentSelectedMap = _mapViews.Keys.First();
            }
        }

        private UIElement CreateTile(Map map, int tileNumber)
        {
            var crop = new Int32Rect()
                {
                    X = (tileNumber % map.TilesYCountInTexture) * map.TileSizeInTexture,
                    Y = (tileNumber / map.TilesYCountInTexture) * map.TileSizeInTexture,
                    Width = map.TileSizeInTexture,
                    Height = map.TileSizeInTexture
                };
            CroppedBitmap imageSource = new CroppedBitmap(getTexture(map.ImagePath), crop);
            
            Image result = new Image();
            result.Source = imageSource;
            result.Width = map.TileSize;
            result.Height = map.TileSize;

            return result;
        }

        private void updateTransparence()
        {
            foreach(var mapCanvas in LevelMap.Children.OfType<Canvas>())
            {
                Map map = (Map)mapCanvas.Tag;
                if (map.Name == CurrentSelectedMap)
                {
                    mapCanvas.Opacity = 1.0f;
                }
                else
                {
                    mapCanvas.Opacity = 0.3f;
                }
            }
        }

        bool _buttonPressed;
        private void LevelGrid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            _buttonPressed = true;
            var position = e.GetPosition(LevelGrid);
            addTile(SelectedTileNumber, position);
        }

        private void LevelGrid_MouseMove(object sender, MouseEventArgs e)
        {
            if (_buttonPressed)
            {
                var position = e.GetPosition(LevelGrid);
                removeTile(position);
                addTile(SelectedTileNumber, position);
            }
        }

        private void LevelGrid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            _buttonPressed = false;
        }

        private void removeTile(Point position)
        {
            var map = _level.Maps.First(m => m.Name == CurrentSelectedMap);
            var imageTiles = _mapViews[CurrentSelectedMap].Children.OfType<Image>();

            foreach (var imageTile in imageTiles)
            {
                double lPositionX = Canvas.GetLeft(imageTile);
                double lPositionY = Canvas.GetTop(imageTile);
                if (position.X >= lPositionX && position.X < lPositionX + imageTile.Width &&
                    position.Y >= lPositionY && position.Y < lPositionY + imageTile.Height)
                {
                    _mapViews[CurrentSelectedMap].Children.Remove(imageTile);
                    break;
                }
            }
        }

        private void addTile(int tileNumber, Point position)
        {
            var map = _level.Maps.First(m => m.Name == CurrentSelectedMap);

            int tileX = (int)position.X / map.TileSize;
            int tileY = (int)position.Y / map.TileSize;
            int tilePositionX = tileX * map.TileSize;
            int tilePositionY = tileY * map.TileSize;

            if (tileNumber != 0)
            {
                var tile = CreateTile(map, tileNumber);
                Canvas.SetLeft(tile, tilePositionX);
                Canvas.SetTop(tile, tilePositionY);
                _mapViews[CurrentSelectedMap].Children.Add(tile);
            }

            int i = tileY * map.Size.Width + tileX;
            if(i < map.Data.Length)
            {
                map.Data[i] = tileNumber;
            }
        }
    }
}
