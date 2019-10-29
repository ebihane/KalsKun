using DetailTool.Command.Monitor;
using DetailTool.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DetailTool.Components.Monitor
{
    /// <summary>
    /// モニタ送信クラス
    /// </summary>
    public class MonitorSender
    {
        private CommControl m_CommControl = null;
        private Thread m_Thread = null;
        private int m_SendCycle = 0;
        private bool m_StopRequest = false;
        private bool m_IsExecute = false;
        private bool m_IsReceived = false;
        private bool m_IsConnected = false;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="comm">通信クラスインスタンス</param>
        public MonitorSender(CommControl comm)
        {
            m_CommControl = comm;
        }

        /// <summary>
        /// Gets 実施状態
        /// </summary>
        public bool IsExecute { get { return m_IsExecute; } }

        /// <summary>
        /// モニタ受信完了イベント
        /// </summary>
        public event EventHandler<EventArgs> OnMonitorReceived;

        /// <summary>
        /// 動作マップ受信完了イベント
        /// </summary>
        public event EventHandler<EventArgs> OnMoveMapReceived;

        /// <summary>
        /// スレッド起動
        /// </summary>
        /// <param name="cycle">送信周期</param>
        public void Start(int cycle)
        {
            if (m_Thread != null)
            {
                Stop();
            }

            m_SendCycle = cycle;
            m_StopRequest = false;
            m_Thread = new Thread(monitorThread);
            m_IsConnected = true;
            m_CommControl.OnDisconnection += evDisconnected;
            m_Thread.Start();
        }

        /// <summary>
        /// スレッド停止
        /// </summary>
        public void Stop()
        {
            if (m_Thread == null)
            {
                goto FINISH;
            }

            if (m_Thread.IsAlive == true)
            {
                m_StopRequest = true;

                bool result = m_Thread.Join(10000);
                if (result == false)
                {
                    m_Thread.Abort(true);
                }
            }

            m_Thread = null;
            m_IsExecute = false;
            m_CommControl.OnDisconnection -= evDisconnected;

        FINISH:
            return;
        }

        /// <summary>
        /// モニタコマンド受信完了イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evStateMonitorReceived(object sender, EventArgs e)
        {
            m_IsReceived = true;
        }

        /// <summary>
        /// 切断イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evDisconnected(object sender, EventArgs e)
        {
            m_IsConnected = false;
        }

        /// <summary>
        /// モニタスレッド
        /// </summary>
        private void monitorThread()
        {
            m_IsExecute = true;

            try
            {
                while (true)
                {
                    bool result = threadMain();
                    if (result != true)
                    {
                        break;
                    }

                    if (m_StopRequest == true)
                    {
                        break;
                    }

                    if (m_IsConnected == false)
                    {
                        break;
                    }

                    System.Threading.Thread.Sleep(m_SendCycle);
                }
            }
            catch (Exception)
            {
                // nop.
            }

            m_IsExecute = false;
        }

        /// <summary>
        /// スレッドメイン処理
        /// </summary>
        /// <returns>true:成功、false:失敗</returns>
        private bool threadMain()
        {
            bool retVal = false;

            // モニタコマンド生成・送信
            StateMonitorCommand monitor = new StateMonitorCommand();
            monitor.OnAnalyzed += evStateMonitorReceived;

            m_IsReceived = false;
            m_CommControl.Send(monitor);

            // 受信完了待ち
            while (true)
            {
                if (m_IsReceived == true)
                {
                    break;
                }

                if (m_IsConnected == false)
                {
                    goto FINISH;
                }
            }

            // 受信完了イベント発行
            if (this.OnMonitorReceived != null)
            {
                EventArgs e = new EventArgs();
                this.OnMonitorReceived(this, e);
            }

            // 動作マップ取得コマンド生成・送信
            MoveMapMonitorCommand moveMap = new MoveMapMonitorCommand();
            moveMap.OnAnalyzed += evStateMonitorReceived;

            m_IsReceived = false;
            m_CommControl.Send(moveMap);

            // 受信完了待ち
            while (true)
            {
                if (m_IsReceived == true)
                {
                    break;
                }

                if (m_IsConnected == false)
                {
                    goto FINISH;
                }
            }

            // マップ受信完了イベント発行
            if (this.OnMoveMapReceived != null)
            {
                EventArgs e = new EventArgs();
                this.OnMoveMapReceived(this, e);
            }

            retVal = true;

        FINISH:
            return retVal;
        }
    }
}
