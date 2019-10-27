using DetailTool.Components.Setting;

namespace DetailTool.Command.Setting
{
    public class SetRobotSizeSettingCommand : CommandBase
    {
        /// <summary>
        /// ロボットサイズ
        /// </summary>
        private SizeData m_RobotSize = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="size">ロボットサイズ</param>
        public SetRobotSizeSettingCommand(SizeData size)
        {
            m_RobotSize = size;
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
            retVal.LParam[0] = 0;
            retVal.FParam[0] = (float)m_RobotSize.Length;
            retVal.FParam[1] = (float)m_RobotSize.Width;

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
