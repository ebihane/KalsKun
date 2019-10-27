using System;
using System.Windows.Forms;
using DetailTool.Command.Monitor;
using DetailTool.Command.Setting;
using DetailTool.Components;
using DetailTool.Components.Setting;
using DetailTool.Controls;

namespace DetailTool
{
    public partial class frmMain : Form
    {
        #region Public Constructor --------------------------------------------------

        public frmMain()
        {
            InitializeComponent();
        }

        #endregion

        #region Private Methods for Event Handler -----------------------------------

        /// <summary>
        /// フォームロードイベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void FrmMain_Load(object sender, EventArgs e)
        {
            initSettingTab();
            initMonitorTab();
        }

        /// <summary>
        /// フォームクローズ前イベント
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            usrCommControl.FinishProcess();
        }

        /// <summary>
        /// 接続完了イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onConnected(object sender, EventArgs e)
        {
            changePanelEnable(true);
        }

        /// <summary>
        /// 切断イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onDisconnection(object sender, EventArgs e)
        {
            changePanelEnable(false);
        }

        #endregion

        #region Setting Tab ---------------------------------------------------------

        #region Private Methods for Event Handler -----------------------------------

        /// <summary>
        /// 設定取得ボタンクリックイベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void btnSettingGet_Click(object sender, EventArgs e)
        {
            // 設定取得
            GetSettingCommand command = new GetSettingCommand();
            command.OnAnalyzed += onSettingReceived;
            usrCommControl.Send(command);
        }

        /// <summary>
        /// 設定変更ボタンクリックイベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void btnSettingChange_Click(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            DoubleText lengthText = null;
            DoubleText widthText = null;

            lengthText = (DoubleText)pnlSetting.Controls[0];
            widthText = (DoubleText)pnlSetting.Controls[1];
            if ((lengthText.Value != setting.RobotSize.Length) || (widthText.Value != setting.RobotSize.Width))
            {
                SizeData size = new SizeData();
                size.Length = lengthText.Value;
                size.Width = widthText.Value;
                SetRobotSizeSettingCommand command = new SetRobotSizeSettingCommand(size);
                command.OnAnalyzed += onRobotSizeSettingChanged;
                usrCommControl.Send(command);
            }

            lengthText = (DoubleText)pnlSetting.Controls[2];
            widthText = (DoubleText)pnlSetting.Controls[3];
            if ((lengthText.Value != setting.FarmSize.Length) || (widthText.Value != setting.FarmSize.Width))
            {
                SizeData size = new SizeData();
                size.Length = lengthText.Value;
                size.Width = widthText.Value;
                SetFarmSizeSettingCommand command = new SetFarmSizeSettingCommand(size);
                command.OnAnalyzed += onFarmSizeSettingChanged;
                usrCommControl.Send(command);
            }

            if ((setting.KusakariStart.DayOfWeek != (DateTimeSetting.DayOfWeekEnum)usrKusakariStart.DayOfWeek) ||
                (setting.KusakariStart.Hour != usrKusakariStart.Hour) ||
                (setting.KusakariStart.Minulte != usrKusakariStart.Minute))
            {
                DateTimeSetting dt = new DateTimeSetting();
                dt.DayOfWeek = (DateTimeSetting.DayOfWeekEnum)usrKusakariStart.DayOfWeek;
                dt.Hour = usrKusakariStart.Hour;
                dt.Minulte = usrKusakariStart.Minute;
                SetStartTimeSettingCommand command = new SetStartTimeSettingCommand(SetStartTimeSettingCommand.ModeEnum.Kusakari, dt);
                command.OnAnalyzed += onKusakariStartSettingChanged;
                usrCommControl.Send(command);
            }

            if ((setting.YakeiStart.DayOfWeek != (DateTimeSetting.DayOfWeekEnum)usrYakeiStart.DayOfWeek) ||
                (setting.YakeiStart.Hour != usrYakeiStart.Hour) ||
                (setting.YakeiStart.Minulte != usrYakeiStart.Minute))
            {
                DateTimeSetting dt = new DateTimeSetting();
                dt.DayOfWeek = (DateTimeSetting.DayOfWeekEnum)usrYakeiStart.DayOfWeek;
                dt.Hour = usrYakeiStart.Hour;
                dt.Minulte = usrYakeiStart.Minute;
                SetStartTimeSettingCommand command = new SetStartTimeSettingCommand(SetStartTimeSettingCommand.ModeEnum.Yakei, dt);
                command.OnAnalyzed += onYakeiStartSettingChanged;
                usrCommControl.Send(command);
            }
        }

        /// <summary>
        /// 設定受信完了時処理
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onSettingReceived(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            DoubleText doubleText = null;

            // ロボットの長さ
            doubleText = (DoubleText)pnlSetting.Controls[0];
            doubleText.Value = setting.RobotSize.Length;

            // ロボットの幅
            doubleText = (DoubleText)pnlSetting.Controls[1];
            doubleText.Value = setting.RobotSize.Width;

            // 畑の長さ
            doubleText = (DoubleText)pnlSetting.Controls[2];
            doubleText.Value = setting.FarmSize.Length;

            // 畑の幅
            doubleText = (DoubleText)pnlSetting.Controls[3];
            doubleText.Value = setting.FarmSize.Width;

            // 草刈り開始時刻
            usrKusakariStart.DayOfWeek = (int)setting.KusakariStart.DayOfWeek;
            usrKusakariStart.Hour = setting.KusakariStart.Hour;
            usrKusakariStart.Minute = setting.KusakariStart.Minulte;

            // 夜警開始時刻
            usrYakeiStart.DayOfWeek = (int)setting.YakeiStart.DayOfWeek;
            usrYakeiStart.Hour = setting.YakeiStart.Hour;
            usrYakeiStart.Minute = setting.YakeiStart.Minulte;
        }

        /// <summary>
        /// ロボットサイズ設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onRobotSizeSettingChanged(object sender, EventArgs e)
        {
            DoubleText doubleText = null;
            SettingData setting = SettingData.GetInstance();

            // ロボットの長さ
            doubleText = (DoubleText)pnlSetting.Controls[0];
            setting.RobotSize.Length = doubleText.Value;

            // ロボットの幅
            doubleText = (DoubleText)pnlSetting.Controls[1];
            setting.RobotSize.Width = doubleText.Value;
        }

        /// <summary>
        /// 畑のサイズ設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onFarmSizeSettingChanged(object sender, EventArgs e)
        {
            DoubleText doubleText = null;
            SettingData setting = SettingData.GetInstance();

            // 畑の長さ
            doubleText = (DoubleText)pnlSetting.Controls[2];
            setting.FarmSize.Length = doubleText.Value;

            // 畑の幅
            doubleText = (DoubleText)pnlSetting.Controls[3];
            setting.FarmSize.Width = doubleText.Value;
        }

        /// <summary>
        /// 草刈り開始時刻設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onKusakariStartSettingChanged(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            setting.KusakariStart.DayOfWeek = (DateTimeSetting.DayOfWeekEnum)usrKusakariStart.DayOfWeek;
            setting.KusakariStart.Hour = usrKusakariStart.Hour;
            setting.KusakariStart.Minulte = usrKusakariStart.Minute;
        }

        /// <summary>
        /// 夜警開始時刻 設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onYakeiStartSettingChanged(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            setting.YakeiStart.DayOfWeek = (DateTimeSetting.DayOfWeekEnum)usrYakeiStart.DayOfWeek;
            setting.YakeiStart.Hour = usrYakeiStart.Hour;
            setting.YakeiStart.Minulte = usrYakeiStart.Minute;
        }

        #endregion

        #region Private Methods -----------------------------------------------------

        /// <summary>
        /// 設定タブ初期化
        /// </summary>
        private void initSettingTab()
        {
            DoubleText doubleText = null;

            usrCommControl.OnConnected += onConnected;
            usrCommControl.OnDisconnection += onDisconnection;

            // ロボットの長さ
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "ロボットの長さ";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 0);

            // ロボットの幅
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "ロボットの幅";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 1);

            // 畑の長さ
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "畑の長さ";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 2);

            // 畑の幅
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "ロボットの幅";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 1);
        }

        /// <summary>
        /// 設定パネル有効状態変更
        /// </summary>
        /// <param name="state">true:有効、false:無効</param>
        private void changePanelEnable(bool state)
        {
            if (pnlSettingMain.InvokeRequired == true)
            {
                pnlSettingMain.BeginInvoke(new Action<bool>((enable) => { pnlSettingMain.Enabled = enable; }), state);
            }
            else
            {
                pnlSettingMain.Enabled = state;
            }
        }

        #endregion

        #endregion

        #region Monitor Tab ---------------------------------------------------------

        #region Private Methods for Event Handler -----------------------------------

        /// <summary>
        /// モニタボタン状態変化イベント
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chkMonitor_CheckedChanged(object sender, EventArgs e)
        {
            if (chkMonitor.Checked == true)
            {
                StateMonitorCommand command = new StateMonitorCommand();
                command.OnAnalyzed += evStateMonitorReceived;
                usrCommControl.Send(command);
            }
        }

        private void evStateMonitorReceived(object sender, EventArgs e)
        {
            MonitorData monitor = MonitorData.GetInstance();
        }

        #endregion

        #region Private Methods -----------------------------------------------------

        /// <summary>
        /// モニタタブ初期化
        /// </summary>
        private void initMonitorTab()
        {
            
        }
        
        #endregion

        #endregion
    }
}
