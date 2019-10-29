using System;
using System.Drawing;

namespace DetailTool.Components.Monitor.Items
{
    public class MotorMoveState : StatusItemBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public MotorMoveState()
        {
        }

        /// <summary>
        /// Gets or sets モータ動作指示
        /// </summary>
        public MotorMoveStateEnum MoveState { get; set; }

        /// <summary>
        /// 動作指示
        /// </summary>
        public enum MotorMoveStateEnum
        {
            E_COMMAND_STOP,     // 停止
            E_COMMAND_FRONT,    // 前進
            E_COMMAND_R_TURN,   // 右ターン
            E_COMMAND_AVOID,    // 回避
            E_COMMAND_REMOTE,   // 遠隔動作
            E_COMMAND_MONITOR,  // モニタ
            E_COMMAND_L_TURN,   // 左ターン
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
            this.MoveState = (MotorMoveStateEnum)value;
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
                case MotorMoveStateEnum.E_COMMAND_STOP:
                    retVal = "停止中";
                    break;

                case MotorMoveStateEnum.E_COMMAND_FRONT:
                    retVal = "前進中";
                    break;

                case MotorMoveStateEnum.E_COMMAND_R_TURN:
                    retVal = "右旋回";
                    break;

                case MotorMoveStateEnum.E_COMMAND_AVOID:
                    retVal = "回避";
                    break;

                case MotorMoveStateEnum.E_COMMAND_REMOTE:
                    retVal = "手動操作";
                    break;

                case MotorMoveStateEnum.E_COMMAND_MONITOR:
                    retVal = "モニタ";
                    break;

                case MotorMoveStateEnum.E_COMMAND_L_TURN:
                    retVal = "左旋回";
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

            switch (this.MoveState)
            {
                case MotorMoveStateEnum.E_COMMAND_FRONT:
                    retVal = Color.LightGreen;
                    break;

                case MotorMoveStateEnum.E_COMMAND_R_TURN:
                    retVal = Color.LightGreen;
                    break;

                case MotorMoveStateEnum.E_COMMAND_L_TURN:
                    retVal = Color.LightGreen;
                    break;

                case MotorMoveStateEnum.E_COMMAND_REMOTE:
                    retVal = Color.Yellow;
                    break;

                case MotorMoveStateEnum.E_COMMAND_AVOID:
                    retVal = Color.Pink;
                    break;

                default:
                    break;
            }

            return retVal;
        }

    }
}
