﻿using System;
using System.Drawing;

namespace DetailTool.Components.Monitor.Items
{
    public class MotorMoveType : StatusItemBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MotorMoveType()
        {
        }

        /// <summary>
        /// Gets or sets 動作指示
        /// </summary>
        public MoveTypeEnum MoveType { get; set; }

        /// <summary>
        /// 動作指示定数
        /// </summary>
        public enum MoveTypeEnum
        {
            NOT_REQUEST = 0,    // 要求無し
            AVOIDANCE,          // 回避指示
            TURN,               // ターン指示
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
            this.MoveType = (MoveTypeEnum)value;
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

            switch (this.MoveType)
            {
                case MoveTypeEnum.NOT_REQUEST:
                    retVal = "－";
                    break;

                case MoveTypeEnum.AVOIDANCE:
                    retVal = "障害物";
                    break;

                case MoveTypeEnum.TURN:
                    retVal = "赤テープ";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.MoveType);
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

            if (this.MoveType == MoveTypeEnum.AVOIDANCE)
            {
                retVal = Color.Pink;
            }
            else if (this.MoveType == MoveTypeEnum.TURN)
            {
                retVal = Color.Pink;
            }
            else
            {
                // nop.
            }

            return retVal;
        }
    }
}
