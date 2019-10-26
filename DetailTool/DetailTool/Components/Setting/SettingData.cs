using System;

namespace DetailTool.Components.Setting
{
    public class SettingData
    {
        #region Private static Fields -----------------------------------------------

        private static SettingData m_This = null;
        private static object m_Lock = new object();

        #endregion

        #region Private Fields for Propaty ------------------------------------------

        #endregion

        #region Public Constructor --------------------------------------------------

        public SettingData()
        {
        }

        #endregion

        #region Public Propaties ----------------------------------------------------

        public SizeData RobotSize { get; set; }
        public SizeData FarmSize { get; set; }
        public int MapWidth { get; private set; }
        public int MapLength { get; private set; }
        public DateTimeSetting KusakariStart { get; set; }
        public DateTimeSetting YakeiStart { get; set; }

        #endregion

        #region Public Methods ------------------------------------------------------

        /// <summary>
        /// インスタンス取得
        /// </summary>
        /// <returns>本クラスのインスタンス</returns>
        public static SettingData GetInstance()
        {
            if (m_This == null)
            {
                lock (m_Lock)
                {
                    if (m_This == null)
                    {
                        m_This = new SettingData();
                    }
                }
            }

            return m_This;
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += this.RobotSize.GetSize();
            retVal += this.FarmSize.GetSize();
            retVal += sizeof(int);
            retVal += sizeof(int);
            retVal += this.KusakariStart.GetSize();
            retVal += this.YakeiStart.GetSize();

            return retVal;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public void Analyze(byte[] data)
        {
            int intValue = 0;
            int dataIndex = 0;

            dataIndex = this.RobotSize.Analyze(data, dataIndex);
            dataIndex = this.FarmSize.Analyze(data, dataIndex);

            // マップの個数 横
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.MapWidth = intValue;
            dataIndex += sizeof(int);

            // マップの個数 縦
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.MapLength = intValue;
            dataIndex += sizeof(int);

            dataIndex = this.KusakariStart.Analyze(data, dataIndex);
            dataIndex = this.YakeiStart.Analyze(data, dataIndex);
        }

        #endregion
    }
}
