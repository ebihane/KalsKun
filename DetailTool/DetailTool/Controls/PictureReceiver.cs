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
        /// <summary>
        /// アドレス情報
        /// </summary>
        private SocketInfomation m_SocketAddress = null;

        /// <summary>
        /// 
        /// </summary>
        private UdpClient m_Client = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public PictureReceiver(string ip, int port)
        {
            InitializeComponent();

            m_SocketAddress = new SocketInfomation(ip, port);
            m_Client = new UdpClient();
        }

        public void Start()
        {
            m_Client.Connect(m_SocketAddress.EndPoint);
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
