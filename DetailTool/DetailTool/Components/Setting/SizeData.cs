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

        public double Vertical { get; set; }
        public double Horizontal { get; set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            return sizeof(double) * 2;
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
            double doubleValue = 0.0F;
            int dataIndex = startIndex;

            // 幅 (= 横 = X)
            doubleValue = BitConverter.ToDouble(data, dataIndex);
            this.Horizontal = doubleValue;
            dataIndex += sizeof(double);

            // 長さ (= 縦 = Y)
            doubleValue = BitConverter.ToDouble(data, dataIndex);
            this.Vertical = doubleValue;
            dataIndex += sizeof(double);

            retVal = dataIndex;

            return retVal;
        }

    }
}
