﻿using DetailTool.Components.Setting;

namespace DetailTool.Command.Setting
{
    public class GetSettingCommand : CommandBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public GetSettingCommand()
        {
        }

        /// <summary>
        /// イベント生成
        /// </summary>
        /// <param name="expectedSize">期待する応答データサイズ</param>
        /// <returns>送信イベント</returns>
        protected override EventInfo createEvent(out int expectedSize)
        {
            EventInfo retVal = new EventInfo();
            expectedSize = SettingData.GetInstance().GetSize();

            retVal.Code = 1000;

            return retVal;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        protected override void analyzeCore(byte[] data)
        {
            SettingData setting = SettingData.GetInstance();

            setting.Analyze(data);
        }
    }
}
