using DetailTool.Components.Monitor;

namespace DetailTool.Components
{
    /// <summary>
    /// モニタデータを管理するシングルトンクラス
    /// </summary>
    public class MonitorData
    {
        #region Private static Fields -----------------------------------------------

        private static MonitorData m_This = null;
        private static object m_Lock = new object();

        #endregion

        #region Private Fields for Propaty ------------------------------------------

        private CommanderMonitor m_CommanderMonitor = null;
        private MotorMonitor m_MotorMonitor = null;
        private FrontCameraMonitor m_FrontCameraMonitor = null;
        private AnimalCameraMonitor m_AnimalCameraMonitor = null;
        private AroundCameraMonitor m_AroundCameraMonitor = null;

        #endregion

        #region Public Constructor --------------------------------------------------

        private MonitorData()
        {
            m_CommanderMonitor = new CommanderMonitor();
            m_MotorMonitor = new MotorMonitor();
            m_FrontCameraMonitor = new FrontCameraMonitor();
            m_AnimalCameraMonitor = new AnimalCameraMonitor();
            m_AroundCameraMonitor = new AroundCameraMonitor();
        }

        #endregion

        #region Public Propaties ----------------------------------------------------

        public CommanderMonitor Commander { get { return m_CommanderMonitor; } }
        public MotorMonitor Monitor { get { return m_MotorMonitor; } }
        public FrontCameraMonitor FrontCamera { get { return m_FrontCameraMonitor; } }
        public AnimalCameraMonitor AnimalCamera { get { return m_AnimalCameraMonitor; } }
        public AroundCameraMonitor AroundCamera { get { return m_AroundCameraMonitor; } }

        #endregion

        #region Public Methods ------------------------------------------------------

        /// <summary>
        /// インスタンス取得
        /// </summary>
        /// <returns>本クラスのインスタンス</returns>
        public static MonitorData GetInstance()
        {
            if (m_This == null)
            {
                lock (m_Lock)
                {
                    if (m_This == null)
                    {
                        m_This = new MonitorData();
                    }
                }
            }

            return m_This;
        }

        /// <summary>
        /// 受信データ解析
        /// </summary>
        /// <param name="data">受信データ配列</param>
        public void Analyze(byte[] data)
        {
            int index = 0;

            index = m_CommanderMonitor.Analyze(data, index);
            index = m_MotorMonitor.Analyze(data, index);
            index = m_AroundCameraMonitor.Analyze(data, index);
            index = m_FrontCameraMonitor.Analyze(data, index);
            index = m_AnimalCameraMonitor.Analyze(data, index);
        }

        #endregion
    }
}
