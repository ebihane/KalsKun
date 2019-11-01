using System;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using DetailTool.Components;
using OpenCvSharp;
using OpenCvSharp.Extensions;

namespace DetailTool
{
    public partial class PictureReceiver : UserControl
    {
        #region Private Fields ------------------------------------------------------

        /// <summary>
        /// アドレス情報
        /// </summary>
        private SocketInfomation m_SocketAddress = null;

        /// <summary>
        /// UDP ソケット クライアント
        /// </summary>
        private UdpClient m_Client = null;

        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public　PictureReceiver()
        {
            InitializeComponent();
        }

        #endregion

        #region Public Constructor --------------------------------------------------

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="defultIp">デフォルトのIPアドレス</param>
        /// <param name="defaultPort">デフォルトのポート番号</param>
        public PictureReceiver(string defaultIp, int defaultPort)
        {
            InitializeComponent();

            char[] separatos = { ',' };
            string[] ipList = defaultIp.Split(separatos);

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
        /// Gets ソケット情報
        /// </summary>
        public SocketInfomation SocketInfo
        {
            get
            {
                return m_SocketAddress;
            }
        }

        #endregion

        public void Start()
        {
            m_Client.Connect(m_SocketAddress.EndPoint);

            int startCommand = 1;
            byte[] startBytes = BitConverter.GetBytes(startCommand);
            m_Client.Send(startBytes, startBytes.Length);
            m_Client.BeginReceive(evOnReceive, m_Client);
        }

        private void evOnReceive(IAsyncResult ar)
        {
            UdpClient sock = (UdpClient)ar.AsyncState;

            IPEndPoint fromEndPoint = null;
            byte[] receiveData = null;

            try
            {
                receiveData = sock.EndReceive(ar, ref fromEndPoint);
            }
            catch (SocketException)
            {
                goto FINISH;
            }
            catch (ObjectDisposedException)
            {
                goto FINISH;
            }

            picDisplay.BeginInvoke(new Action<byte[]>(showCameraData), receiveData);

        FINISH:
            return;
        }

        /// <summary>
        /// 受信データ表示
        /// </summary>
        /// <param name="receiveData">受信データ</param>
        /// <param name="target">描画対象 PictureBox</param>
        private void showCameraData(byte[] receiveData)
        {
            Mat disp = Cv2.ImDecode(receiveData, ImreadModes.Color);
            picDisplay.Image = disp.ToBitmap();
        }
    }
}
