using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Components.Setting
{
    public class SizeData
    {
        public SizeData()
        {
        }

        public float Width { get; set; }
        public float Length { get; set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            return sizeof(float) * 2;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public int Analyze(byte[] data, int startIndex)
        {
            int retVal = 0;
            float floatValue = 0.0F;
            int dataIndex = startIndex;

            // 幅 (= 横 = X)
            floatValue = BitConverter.ToSingle(data, dataIndex);
            this.Width = floatValue;
            dataIndex += sizeof(float);

            // 長さ (= 縦 = Y)
            floatValue = BitConverter.ToSingle(data, dataIndex);
            this.Length = floatValue;
            dataIndex += sizeof(float);

            retVal = dataIndex;

            return retVal;
        }

    }
}
