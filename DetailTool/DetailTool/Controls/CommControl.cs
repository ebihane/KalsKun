using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;
using DetailTool.Command;
using DetailTool.Components;

namespace DetailTool.Controls
{
    public partial class CommControl : UserControl
    {
        #region Private Fields ------------------------------------------------------

        /// <summary>
        /// アドレス情報
        /// </summary>
        private SocketInfomation m_Address = null;

        /// <summary>
        /// クライアントソケット
        /// </summary>
        private TcpClient m_TcpClient = null;

        /// <summary>
        /// 通信スレッド
        /// </summary>
        private Thread m_CommunicationThread = null;

        /// <summary>
        /// スレッド停止要求
        /// </summary>
        private ManualResetEvent m_StopRequest = null;

        /// <summary>
        /// 接続完了状態
        /// </summary>
        private ManualResetEvent m_Connected = null;

        /// <summary>
        /// コマンド要求キュー
        /// </summary>
        private Queue<CommandBase> m_RequestQueue = new Queue<CommandBase>();

        #endregion

        #region Public Constructor --------------------------------------------------

        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public CommControl()
        {
            InitializeComponent();
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="defaultIp">デフォルトのIPアドレス</param>
        /// <param name="defaultPort">デフォルトのポート番号</param>
        public CommControl(string defaultIp, int defaultPort)
        {
            InitializeComponent();

            char[] separators = { ',' };
            string[] ipList = defaultIp.Split(separators);

            if (ipList.Length == 4)
            {
                txtIp1.Text = ipList[0];
                txtIp2.Text = ipList[1];
                txtIp3.Text = ipList[2];
                txtIp4.Text = ipList[3];
            }

            txtPort.Text = defaultPort.ToString();
        }

        #endregion

        #region Public Propaties ----------------------------------------------------

        /// <summary>
        /// Gets 接続状態
        /// </summary>
        public bool IsConnected
        {
            get
            {
                bool retVal = false;

                if (m_TcpClient == null)
                {
                    retVal = false;
                }
                else
                {
                    retVal = m_TcpClient.Connected;
                }

                return retVal;
            }
        }

        /// <summary>
        /// Gets ソケット情報
        /// </summary>
        public SocketInfomation SocketInfo
        {
            get
            {
                return m_Address;
            }
        }

        #endregion

        #region Public Events -------------------------------------------------------

        /// <summary>
        /// 接続完了イベント
        /// </summary>
        public event EventHandler<EventArgs> OnConnected;

        /// <summary>
        /// 切断イベント
        /// </summary>
        public event EventHandler<EventArgs> OnDisconnection;

        #endregion

        #region Public Methods ------------------------------------------------------
        
        /// <summary>
        /// 終了処理
        /// </summary>
        public void FinishProcess()
        {
            if (m_StopRequest != null)
            {
                m_StopRequest.Set();
                m_CommunicationThread.Join(10000);
                m_CommunicationThread = null;
                m_StopRequest.Dispose();
            }

            m_StopRequest = null;
        }

        /// <summary>
        /// 送信
        /// </summary>
        /// <param name="command">送信対象コマンド</param>
        public void Send(CommandBase command)
        {
            if (m_CommunicationThread != null)
            {
                m_RequestQueue.Enqueue(command);
            }
        }

        #endregion

        #region Private Methods for Event Handler -----------------------------------

        /// <summary>
        /// 接続ボタンクリック
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void btnConnection_Click(object sender, EventArgs e)
        {
            if (m_TcpClient == null)
            {
                string ip = txtIp1.Text + "." + txtIp2.Text + "." + txtIp3.Text + "." + txtIp4.Text;
                int port = int.Parse(txtPort.Text);
                m_Address = new SocketInfomation(ip, port);
                m_TcpClient = new TcpClient();
                changeConnectButtonText("中止");
                changeStateLabel("接続中...");
                changeInputFieldEnable(false);
                m_StopRequest = new ManualResetEvent(false);
                m_CommunicationThread = new Thread(new ThreadStart(communicationThread));
                m_CommunicationThread.Start();
            }
            else if (m_TcpClient.Connected == false)
            {
                m_StopRequest.Set();
                m_CommunicationThread.Join(10000);
                m_StopRequest.Dispose();
                m_CommunicationThread = null;
                m_StopRequest = null;

                m_TcpClient = null;
                m_Address = null;
                changeConnectButtonText("接続");
                changeStateLabel("接続していません。");
                changeInputFieldEnable(true);
            }
            else
            {
                changeConnectButtonEnable(false);
                changeStateLabel("切断中");
                DisconnectCommand command = new DisconnectCommand();
                command.OnAnalyzed += evDisconnected;
                m_RequestQueue.Enqueue(command);
            }
        }

        /// <summary>
        /// 切断コマンド送信後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evDisconnected(object sender, EventArgs e)
        {
            m_StopRequest.Set();
            m_CommunicationThread.Join(10000);
            m_StopRequest.Dispose();
            m_CommunicationThread = null;
            m_StopRequest = null;

            m_TcpClient = null;
            m_Address = null;
            changeConnectButtonText("接続");
            changeStateLabel("接続していません。");
            changeInputFieldEnable(true);
            changeConnectButtonEnable(true);
        }

        #endregion

        #region Private Methods -----------------------------------------------------

        /// <summary>
        /// 接続完了イベント発行
        /// </summary>
        private void onConnected()
        {
            if (this.OnConnected != null)
            {
                EventArgs e = new EventArgs();
                this.OnConnected(this, e);
            }
        }

        /// <summary>
        /// 切断イベント発行
        /// </summary>
        private void onDisconnection()
        {
            if (this.OnDisconnection != null)
            {
                EventArgs e = new EventArgs();
                this.OnDisconnection(this, e);
            }
        }

        /// <summary>
        /// 状態表示更新
        /// </summary>
        /// <param name="text">表示文字列</param>
        private void changeStateLabel(string text)
        {
            if (lblState.InvokeRequired == true)
            {
                lblState.BeginInvoke(new Action<string>((message) => { lblState.Text = message; }), text);
            }
            else
            {
                lblState.Text = text;
            }
        }

        /// <summary>
        /// 接続ボタン表示更新
        /// </summary>
        /// <param name="text">表示文字列</param>
        private void changeConnectButtonText(string text)
        {
            if (btnConnection.InvokeRequired == true)
            {
                btnConnection.BeginInvoke(new Action<string>((message) => { btnConnection.Text = message; }), text);
            }
            else
            {
                btnConnection.Text = text;
            }
        }

        /// <summary>
        /// 接続ボタン有効・無効切り替え
        /// </summary>
        /// <param name="enable">状態</param>
        private void changeConnectButtonEnable(bool enable)
        {
            if (btnConnection.InvokeRequired == true)
            {
                btnConnection.BeginInvoke(new Action<bool>((state) => { btnConnection.Enabled = state; }), enable);
            }
            else
            {
                btnConnection.Enabled = enable;
            }
        }

        /// <summary>
        /// アドレス情報入力フィールド 有効・無効 切り替え
        /// </summary>
        /// <param name="enable">状態</param>
        private void changeInputFieldEnable(bool enable)
        {
            if (pnlInputField.InvokeRequired == true)
            {
                pnlInputField.BeginInvoke(new Action<bool>((state) => { pnlInputField.Enabled = state; }), enable);
            }
            else
            {
                pnlInputField.Enabled = enable;
            }
        }

        /// <summary>
        /// 通信スレッド本体
        /// </summary>
        private void communicationThread()
        {
            while (true)
            {
                if (m_StopRequest == null)
                {
                    break;
                }

                if (m_StopRequest.WaitOne(10) == true)
                {
                    m_TcpClient.Close();
                    m_TcpClient.Dispose();
                    break;
                }

                try
                {
                    m_Connected = new ManualResetEvent(false);
                    IAsyncResult result = m_TcpClient.BeginConnect(m_Address.IP, m_Address.PortNo, connectCallback, m_TcpClient);
                    while (true)
                    {
                        if(m_Connected.WaitOne(10) == true)
                        {
                            if (m_TcpClient.Connected == true)
                            {
                                break;
                            }
                        }

                        if (m_StopRequest.WaitOne(10) == true)
                        {
                            m_TcpClient.Close();
                            m_TcpClient.Dispose();
                            m_Connected.Dispose();
                            m_Connected = null;
                            goto FINISH;
                        }
                    }

                    m_Connected.Dispose();
                    m_Connected = null;
                    m_RequestQueue.Clear();
                    onConnected();
                    changeStateLabel("接続しました。");
                    changeConnectButtonText("切断");
                }
                catch (Exception e)
                {
                    changeStateLabel(e.Message);
                    Thread.Sleep(10);
                    continue;
                }

                NetworkStream stream = m_TcpClient.GetStream();
                communicationMain(stream);

                onDisconnection();
            }

            m_RequestQueue.Clear();

        FINISH:
            return;
        }

        /// <summary>
        /// 接続時コールバック
        /// </summary>
        /// <param name="ar">非同期結果</param>
        private void connectCallback(IAsyncResult ar)
        {
            try
            {
                TcpClient client = (TcpClient)ar.AsyncState;
                client.EndConnect(ar);
                m_Connected.Set();
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 通信処理本体
        /// </summary>
        /// <param name="stream">送受信ストリーム</param>
        private void communicationMain(NetworkStream stream)
        {
            while (true)
            {
                Thread.Sleep(10);

                if (m_StopRequest == null)
                {
                    break;
                }

                if (m_StopRequest.WaitOne(10) == true)
                {
                    m_TcpClient.Close();
                    m_TcpClient.Dispose();
                    break;
                }

                if (m_RequestQueue.Count <= 0)
                {
                    continue;
                }

                CommandBase command = m_RequestQueue.Dequeue();

                int expectedSize = 0;
                byte[] sendData = command.CreateCommand(out expectedSize);
                byte[] response = new byte[expectedSize];

                try
                {
                    stream.Write(sendData, 0, sendData.Length);
                }
                catch (Exception)
                {
                    goto FINISH;
                }

                try
                {
                    int index = 0;
                    int remain = expectedSize;
                    while (0 < remain)
                    {
                        int once = stream.Read(response, index, remain);
                        index += once;
                        remain -= once;
                    }
                }
                catch (Exception)
                {
                    goto FINISH;
                }

                command.Analyze(response);
            }

        FINISH:
            return;
        }

        #endregion
    }
}
