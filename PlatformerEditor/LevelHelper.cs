using Newtonsoft.Json.Linq;
using PlatformerEditor.Data;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PlatformerEditor
{
    class LevelHelper
    {
        static public Level CreateNewLevel(int width, int height)
        {
            Level result = new Level();
            result.Path = @"C:\Dev\Perso\MarioBross\MarioBross\lele2.json";
            result.BackgroundColor = System.Drawing.Color.AliceBlue;

            result.Maps = new List<Map>();
            var backDecorsMap = new Map()
            {
                Name = "BackDecorsMap",
                Data = new int[width * height],
                ImagePath = "map_decor.png",
                Size = new System.Drawing.Size(width, height),
                TileSizeInTexture = 16,
                TileSize = 16,
                TilesYCountInTexture = 29
            };
            result.Maps.Add(backDecorsMap);

            var frontDecorsMap = new Map()
            {
                Name = "FrontDecorsMap",
                Data = new int[width * height],
                ImagePath = "map_decor.png",
                Size = new System.Drawing.Size(width, height),
                TileSizeInTexture = 16,
                TileSize = 16,
                TilesYCountInTexture = 29
            };
            result.Maps.Add(frontDecorsMap);

            var collisionMap = new Map()
            {
                Name = "CollisionMap",
                Data = new int[width * height],
                ImagePath = "map.png",
                Size = new System.Drawing.Size(width, height),
                TileSizeInTexture = 16,
                TileSize = 16,
                TilesYCountInTexture = 29
            };
            result.Maps.Add(collisionMap);

            var itemMap = new Map()
            {
                Name = "Items",
                Data = new int[width * height],
                ImagePath = "items.png",
                Size = new System.Drawing.Size(width, height),
                TileSizeInTexture = 16,
                TileSize = 16,
                TilesYCountInTexture = 29
            };
            result.Maps.Add(itemMap);

            return result;
        }

        static public void SaveJson(Level level, string path)
        {
            JObject jsonLevel = new JObject();
            jsonLevel.Add("BackColorR", level.BackgroundColor.R);
            jsonLevel.Add("BackColorG", level.BackgroundColor.G);
            jsonLevel.Add("BackColorB", level.BackgroundColor.B);

            JArray jsonMapArray = new JArray();
            foreach(var map in level.Maps)
            {
                if (map.Name == "Items")
                {
                    continue;
                }

                JObject jsonMap = new JObject();
                jsonMap.Add("Name", map.Name);
                jsonMap.Add("ImagePath", map.ImagePath);
                jsonMap.Add("Width", map.Size.Width);
                jsonMap.Add("Height", map.Size.Height);
                jsonMap.Add("TileSize", map.TileSize);
                jsonMap.Add("TileSizeInTexture", map.TileSizeInTexture);
                jsonMap.Add("TilesYCountInTexture", map.TilesYCountInTexture);

                string tiles = "";
                for (int y = 0; y < map.Size.Height; y++)
                {
                    for (int x = 0; x < map.Size.Width; x++)
                    {
                        int i = y * map.Size.Width + x;
                        if (map.Data[i] != 0)
                        {
                            tiles += map.Data[i].ToString();
                        }
                        tiles += ",";
                    }
                }
                tiles = tiles.Substring(0, tiles.Count() - 1);
                jsonMap.Add("Tiles", tiles);
                jsonMapArray.Add(jsonMap);  
            }
            jsonLevel.Add("Maps", jsonMapArray);

            JArray jsonItems = new JArray();
            var itemMap = level.Maps.First(m => m.Name == "Items");
            for (int y = 0; y < itemMap.Size.Height; y++)
            {
                for (int x = 0; x < itemMap.Size.Width; x++)
                {
                    int i = y * itemMap.Size.Width + x;
                    if (itemMap.Data[i] != 0)
                    {
                        JObject jsonItem = new JObject();
                        if (itemMap.Data[i] == 1)
                        {
                            jsonItem.Add("Name", "PlayerAvatar");
                        }
                        else if (itemMap.Data[i] == 2)
                        {
                            jsonItem.Add("Name", "EnemyMushroom");
                        }
                        else if (itemMap.Data[i] == 3)
                        {
                            jsonItem.Add("Name", "BonusMushroom");
                        }
                        else if (itemMap.Data[i] == 4)
                        {
                            jsonItem.Add("Name", "MobilePlatform");
                        }
                        else if (itemMap.Data[i] == 5)
                        {
                            jsonItem.Add("Name", "BlocBonus");
                        }
                        else if (itemMap.Data[i] == 6)
                        {
                            jsonItem.Add("Name", "CoinBonus");
                        }
                        else if (itemMap.Data[i] == 7)
                        {
                            jsonItem.Add("Name", "SpikeBloc");
                        }
                        else if (itemMap.Data[i] == 8)
                        {
                            jsonItem.Add("Name", "BumperBloc");
                        }
                        jsonItem.Add("X", x * itemMap.TileSize);
                        jsonItem.Add("Y", y * itemMap.TileSize);

                        if(level.ObjectsInfo.ContainsKey(new System.Drawing.Point(x, y)))
                        {
                            var infoDico = level.ObjectsInfo[new System.Drawing.Point(x, y)];
                            foreach (var key in infoDico.Keys)
                            {
                                jsonItem.Add(key, infoDico[key]);
                            }
                        }


                        jsonItems.Add(jsonItem);
                    }
                }
            }
            jsonLevel.Add("Items", jsonItems);

            string json = jsonLevel.ToString(Newtonsoft.Json.Formatting.Indented);
            File.WriteAllText(path, json);
        }

        static public Level LoadJson(string path)
        {
            Level result = new Level();
            result.ObjectsInfo = new Dictionary<System.Drawing.Point, Dictionary<string, string>>();

            result.Path = path;
            string json = File.ReadAllText(path);
            var jsonLevel = JObject.Parse(json);

            int backR = jsonLevel["BackColorR"].Value<int>();
            int backG = jsonLevel["BackColorG"].Value<int>();
            int backB = jsonLevel["BackColorB"].Value<int>();
            result.BackgroundColor = System.Drawing.Color.FromArgb(255, backR, backG, backB);

            result.Maps = new List<Map>();
            foreach (var jsonMap in jsonLevel["Maps"].ToArray())
            {
                int sizeX = jsonMap["Width"].Value<int>();
                int sizeY = jsonMap["Height"].Value<int>();
                var map = new Map()
                {
                    Name = jsonMap["Name"].Value<string>(),
                    Data = new int[sizeX * sizeY],
                    ImagePath = jsonMap["ImagePath"].Value<string>(),
                    TileSize = jsonMap["TileSize"].Value<int>(),
                    TileSizeInTexture = jsonMap["TileSizeInTexture"].Value<int>(),
                    TilesYCountInTexture = jsonMap["TilesYCountInTexture"].Value<int>(),
                    Size = new System.Drawing.Size(sizeX, sizeY)
                };

                string[] tiles = jsonMap["Tiles"].Value<string>().Split(',');

                for (int y = 0; y < sizeY; y++)
                {
                    for (int x = 0; x < sizeX; x++)
                    {
                        int i = y * sizeX + x;
                        if (tiles[i] != "")
                        {
                            map.Data[i] = int.Parse(tiles[i]);
                        }
                        else
                        {
                            map.Data[i] = 0;
                        }
                    }
                }

                result.Maps.Add(map);
            }

            var collisionMap = result.Maps.First(m => m.Name == "CollisionMap");

            var mapObjects = new Map()
                {
                    Name = "Items",
                    Data = new int[collisionMap.Size.Width * collisionMap.Size.Height],
                    ImagePath = "items.png",
                    TileSize = collisionMap.TileSize,
                    TileSizeInTexture = collisionMap.TileSizeInTexture,
                    TilesYCountInTexture = collisionMap.TilesYCountInTexture,
                    Size = collisionMap.Size
                };

            foreach (JObject jsonItem in jsonLevel["Items"].ToArray())
            {
                string name = jsonItem["Name"].ToString();
                int tileX = int.Parse(jsonItem["X"].ToString()) / collisionMap.TileSize;
                int tileY = int.Parse(jsonItem["Y"].ToString()) / collisionMap.TileSize;
                int index = tileY * mapObjects.Size.Width + tileX;

                Dictionary<string, string> info = new Dictionary<string, string>();
                foreach(var propriety in jsonItem.Properties())
                {
                    if(propriety.Name != "Name" && propriety.Name != "X" && propriety.Name != "Y")
                    {
                        info.Add(propriety.Name, propriety.Value.ToString());
                    }
                }
                result.ObjectsInfo.Add(new System.Drawing.Point(tileX, tileY), info);


                if (name == "PlayerAvatar")
                {
                    mapObjects.Data[index] = 1;
                }
                else if (name == "EnemyMushroom")
                {
                    mapObjects.Data[index] = 2;
                }
                else if (name == "BonusMushroom")
                {
                    mapObjects.Data[index] = 3;
                }
                else if (name == "MobilePlatform")
                {
                    mapObjects.Data[index] = 4;
                }
                else if (name == "BlocBonus")
                {
                    mapObjects.Data[index] = 5;
                }
                else if (name == "CoinBonus")
                {
                    mapObjects.Data[index] = 6;
                }
                else if (name == "SpikeBloc")
                {
                    mapObjects.Data[index] = 7;
                }
                else if (name == "BumperBloc")
                {
                    mapObjects.Data[index] = 8;
                }
            }

            result.Maps.Add(mapObjects);

            return result;
        }
    }
}
