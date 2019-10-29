using System;
using System.Drawing;

namespace DetailTool.Components.Map
{
    public class MoveMap : MapData
    {
        #region Private static Fields -----------------------------------------------

        private static MoveMap m_This = null;
        private static object m_Lock = new object();

        #endregion

        #region Private Fields ------------------------------------------------------

        /// <summary>
        /// 未走行値
        /// </summary>
        private byte m_NotMoveValue = 1;

        /// <summary>
        /// 走行済み
        /// </summary>
        private byte m_MovedValue = 2;

        #endregion

        #region Public Constructor --------------------------------------------------

        /// <summary>
        /// コンストラクタ
        /// </summary>
        private MoveMap()
        {
        }

        #endregion

        #region Public Methods ------------------------------------------------------

        /// <summary>
        /// インスタンス取得
        /// </summary>
        /// <returns>本クラスのインスタンス</returns>
        public static MoveMap GetInstance()
        {
            if (m_This == null)
            {
                lock (m_Lock)
                {
                    if (m_This == null)
                    {
                        m_This = new MoveMap();
                    }
                }
            }

            return m_This;
        }

        /// <summary>
        /// 更新
        /// </summary>
        /// <param name="data"></param>
        public override void Update(byte[] data)
        {
            base.Update(data);

            Array.Sort(data);
            byte min = data[0];
            byte max = data[data.Length - 1];

            m_MovedValue = max;
            m_NotMoveValue = (byte)(max - 1);
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

            if (value == m_NotMoveValue)
            {
                retVal = Color.LightGreen;
            }
            else if (value == m_MovedValue)
            {
                retVal = Color.Khaki;
            }
            else if (value == MapValues.MoveFailedNo)
            {
                retVal = Color.Gray;
            }
            else
            {
                retVal = Color.DarkGray;
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
            /// 走行禁止
            /// </summary>
            public const byte MoveFailedNo = 0;
        }

        #endregion
    }
}