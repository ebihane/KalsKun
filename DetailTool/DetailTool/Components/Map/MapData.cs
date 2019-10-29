using System;
using System.Collections.Generic;

namespace DetailTool.Components.Map
{
    public class MapData
    {
        /// <summary>
        /// マップデータ
        /// </summary>
        List<List<int>> m_Map = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MapData()
        {
        }

        /// <summary>
        /// Gets マップデータ
        /// </summary>
        public List<List<int>> Map { get { return m_Map; } }

        /// <summary>
        /// 領域確保
        /// </summary>
        /// <param name="length">長さ (縦軸：Y)</param>
        /// <param name="width">幅 (横軸 : X)</param>
        public void Allocate(int length, int width)
        {
            m_Map = new List<List<int>>();

            for (int y = 0; y < length; y++)
            {
                List<int> temp = new List<int>();
                for (int x = 0; x < width; x++)
                {
                    int value = 0;
                    temp.Add(value);
                }

                m_Map.Add(temp);
            }
        }

    }
}
