using System;
using System.Collections.Generic;

namespace DetailTool.Components.Map
{
    public abstract class MapData
    {
        /// <summary>
        /// マップデータ
        /// </summary>
        List<List<byte>> m_Map = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MapData()
        {
        }

        /// <summary>
        /// Gets マップデータ
        /// </summary>
        public List<List<byte>> Map { get { return m_Map; } }

        /// <summary>
        /// 領域確保
        /// </summary>
        /// <param name="length">長さ (縦軸：Y)</param>
        /// <param name="width">幅 (横軸 : X)</param>
        public void Allocate(int length, int width)
        {
            m_Map = new List<List<byte>>();

            for (int y = 0; y < length; y++)
            {
                List<byte> temp = new List<byte>();
                for (int x = 0; x < width; x++)
                {
                    byte value = 0;
                    temp.Add(value);
                }

                m_Map.Add(temp);
            }
        }

        /// <summary>
        /// 値更新
        /// </summary>
        /// <param name="data">更新データ</param>
        public virtual void Update(byte[] data)
        {
            for (int y = 0; y < m_Map.Count; y++)
            {
                for (int x = 0; x < m_Map[y].Count; x++)
                {
                    int index = y * m_Map[y].Count + x;
                    m_Map[y][x] = data[index];
                }
            }
        }

        /// <summary>
        /// モニタ表示色取得
        /// </summary>
        /// <param name="x">対象位置 X 座標</param>
        /// <param name="y">対象位置 Y 座標</param>
        /// <returns>表示色</returns>
        public abstract System.Drawing.Color GetColor(int x, int y);
    }
}
