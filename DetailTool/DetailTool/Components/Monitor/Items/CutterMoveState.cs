using System;
using System.Drawing;

namespace DetailTool.Components.Monitor.Items
{
    public class CutterMoveState : StatusItemBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public CutterMoveState()
        {
        }

        /// <summary>
        /// Gets or sets 草刈り刃動作指示
        /// </summary>
        public CutterDriveEnum MoveState { get; set; }

        /// <summary>
        /// 草刈り刃動作指示
        /// </summary>
        public enum CutterDriveEnum
        {
            E_CUTTER_STOP,      // 停止
            E_CUTTER_DRIVE,     // 駆動
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public override int GetSize()
        {
            return sizeof(int);
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象バイト配列</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public override int Analyze(byte[] data, int startIndex)
        {
            int retVal = 0;
            int index = startIndex;

            int value = BitConverter.ToInt32(data, index);
            this.MoveState = (CutterDriveEnum)value;
            index += sizeof(int);

            retVal = index;

            return retVal;
        }

        /// <summary>
        /// 文字列取得
        /// </summary>
        /// <returns>文字列</returns>
        public override string ToString()
        {
            string retVal = string.Empty;

            switch (this.MoveState)
            {
                case CutterDriveEnum.E_CUTTER_STOP:
                    retVal = "停止中";
                    break;

                case CutterDriveEnum.E_CUTTER_DRIVE:
                    retVal = "回転中";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.MoveState);
                    break;
            }

            return retVal;
        }

        /// <summary>
        /// 背景色取得
        /// </summary>
        /// <returns>背景色</returns>
        public override Color GetBackColor()
        {
            Color retVal = base.GetBackColor();

            if (this.MoveState == CutterDriveEnum.E_CUTTER_DRIVE)
            {
                retVal = Color.Yellow;
            }

            return retVal;
        }
    }
}
