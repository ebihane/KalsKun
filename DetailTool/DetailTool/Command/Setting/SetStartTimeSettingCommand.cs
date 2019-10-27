using DetailTool.Components.Setting;

namespace DetailTool.Command.Setting
{
    public class SetStartTimeSettingCommand : CommandBase
    {
        /// <summary>
        /// 送信する lParam[1] の値
        /// </summary>
        private int m_SendLParam = -1;

        /// <summary>
        /// 開始時刻
        /// </summary>
        private DateTimeSetting m_DateTime = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="mode">モード</param>
        /// <param name="dt">開始時刻</param>
        public SetStartTimeSettingCommand(ModeEnum mode, DateTimeSetting dt)
        {
            m_DateTime = dt;

            if (mode == ModeEnum.Kusakari)
            {
                m_SendLParam = 2;
            }
            else if (mode == ModeEnum.Yakei)
            {
                m_SendLParam = 3;
            }
            else
            {
                // nop.
            }
        }

        /// <summary>
        /// 設定対象指定
        /// </summary>
        public enum ModeEnum
        {
            /// <summary>
            /// 草刈り開始時刻
            /// </summary>
            Kusakari,

            /// <summary>
            /// 夜警開始時刻
            /// </summary>
            Yakei,
        }

        /// <summary>
        /// イベント生成
        /// </summary>
        /// <param name="expectedSize">期待する受信サイズ</param>
        /// <returns>送信イベント</returns>
        protected override EventInfo createEvent(out int expectedSize)
        {
            EventInfo retVal = new EventInfo();
            expectedSize = sizeof(int);

            retVal.Code = 1001;
            retVal.LParam[0] = m_SendLParam;
            retVal.LParam[1] = (int)m_DateTime.DayOfWeek;
            retVal.LParam[2] = m_DateTime.Hour;
            retVal.LParam[3] = m_DateTime.Minulte;

            return retVal;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        protected override void analyzeCore(byte[] data)
        {
        }
    }
}
