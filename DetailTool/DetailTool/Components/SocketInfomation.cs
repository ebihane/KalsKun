using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Components
{
    public class SocketInfomation
    {
        private string m_BaseIp = string.Empty;

        private int m_PortNo = 0;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="ip">IPアドレス</param>
        /// <param name="port">ポート番号</param>
        public SocketInfomation(string ip, int port)
        {
            m_BaseIp = ip;
            m_PortNo = port;
        }

        /// <summary>
        /// Gets IP
        /// </summary>
        public IPAddress IP
        {
            get
            {
                IPAddress retVal = null;

                if (m_BaseIp == string.Empty)
                {
                    retVal = IPAddress.Any;
                }
                else
                {
                    IPAddress ip = null;
                    if (IPAddress.TryParse(m_BaseIp, out ip) == true)
                    {
                        retVal = ip;
                    }
                }

                return retVal;
            }
        }

        /// <summary>
        /// Gets IP アドレス
        /// </summary>
        public string IPString
        {
            get { return m_BaseIp; }
        }

        public IPEndPoint EndPoint
        {
            get
            {
                IPEndPoint retVal = new IPEndPoint(this.IP, m_PortNo);
                return retVal;
            }
        }

        public int PortNo
        {
            get { return m_PortNo; }
        }
    }
}
