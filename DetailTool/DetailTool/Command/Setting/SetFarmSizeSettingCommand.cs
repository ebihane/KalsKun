using DetailTool.Components.Setting;

namespace DetailTool.Command.Setting
{
    public class SetFarmSizeSettingCommand : CommandBase
    {
        /// <summary>
        /// 畑のサイズ
        /// </summary>
        private SizeData m_FarmSize = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="size">畑のサイズ</param>
        public SetFarmSizeSettingCommand(SizeData size)
        {
            m_FarmSize = size;
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
            retVal.LParam[0] = 1;
            retVal.FParam[0] = (float)m_FarmSize.Length;
            retVal.FParam[1] = (float)m_FarmSize.Width;

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
