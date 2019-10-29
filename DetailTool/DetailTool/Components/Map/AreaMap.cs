using System.Drawing;

namespace DetailTool.Components.Map
{
    public class AreaMap : MapData
    {
        #region Private static Fields -----------------------------------------------

        private static AreaMap m_This = null;
        private static object m_Lock = new object();

        #endregion

        #region Public Constructor --------------------------------------------------

        private AreaMap()
        {
        }

        #endregion

        #region Public Methods ------------------------------------------------------

        /// <summary>
        /// インスタンス取得
        /// </summary>
        /// <returns>本クラスのインスタンス</returns>
        public static AreaMap GetInstance()
        {
            if (m_This == null)
            {
                lock (m_Lock)
                {
                    if (m_This == null)
                    {
                        m_This = new AreaMap();
                    }
                }
            }

            return m_This;
        }

        /// <summary>
        /// モニタ表示色取得
        /// </summary>
        /// <param name="x">対象位置 X 座標</param>
        /// <param name="y">対象位置 Y 座標</param>
        /// <returns>表示色</returns>
        public override Color GetColor(int x, int y)
        {
            Color retVal = Color.DarkGray;
            byte value = this.Map[y][x];

            switch (value)
            {
                case MapValues.Unknown:
                    retVal = Color.White;
                    break;

                case MapValues.Movable:
                    retVal = Color.LightGreen;
                    break;

                case MapValues.NotMovable:
                    retVal = Color.Gray;
                    break;

                case MapValues.Obstacle:
                    retVal = Color.Khaki;
                    break;

                default:
                    retVal = Color.White;
                    break;
            }

            return retVal;
        }

        #endregion

        #region Private Constants ---------------------------------------------------

        /// <summary>
        /// マップ格納値
        /// </summary>
        private static class MapValues
        {
            /// <summary>
            /// 未確定
            /// </summary>
            public const byte Unknown = 0;

            /// <summary>
            /// 移動可能
            /// </summary>
            public const byte Movable = 1;

            /// <summary>
            /// 移動不可
            /// </summary>
            public const byte NotMovable = 2;

            /// <summary>
            /// 障害物
            /// </summary>
            public const byte Obstacle = 3;
        }

        #endregion
    }
}
