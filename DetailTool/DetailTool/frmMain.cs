using System;
using System.Windows.Forms;
using DetailTool.Command.Monitor;
using DetailTool.Command.Setting;
using DetailTool.Command.Utility;
using DetailTool.Components;
using DetailTool.Components.Map;
using DetailTool.Components.Monitor;
using DetailTool.Components.Monitor.Items;
using DetailTool.Components.Setting;
using DetailTool.Controls;

namespace DetailTool
{
    public partial class frmMain : Form
    {
        #region Public Constructor --------------------------------------------------

        /// <summary>
        /// モニタ送信スレッド
        /// </summary>
        private MonitorSender m_MonitorSender = null;

        #endregion

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
            // モニタコマンド送信スレッドインスタンス生成
            m_MonitorSender = new MonitorSender(this.usrCommControl);

            initSettingTab();
            initMonitorTab();
            initDetailTab();
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
            this.BeginInvoke(new Action<bool>(
            (state) =>
            {
                pnlSettingMain.Enabled = state;
                chkMonitor.Enabled = state;
                btnTimeAdjust.Enabled = state;
            }),
            true);

            // 設定取得
            if ((usrCommControl.SocketInfo.IPString != "192.168.3.2")
            &&  (usrCommControl.SocketInfo.IPString != "192.168.3.3")
            &&  (usrCommControl.SocketInfo.IPString != "192.168.3.4"))
            {
                GetSettingCommand command = new GetSettingCommand();
                command.OnAnalyzed += evSettingReceived;
                usrCommControl.Send(command);
            }
        }

        /// <summary>
        /// 切断イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onDisconnection(object sender, EventArgs e)
        {
            this.BeginInvoke(new Action<bool>(
            (state) =>
            {
                pnlSettingMain.Enabled = state;
                chkMonitor.Enabled = state;
                btnTimeAdjust.Enabled = state;
            }),
            false);
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
            command.OnAnalyzed += evSettingReceived;
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
            DoubleText verticalText = null;
            DoubleText horizontalText = null;

            verticalText = (DoubleText)pnlSetting.Controls[0];
            horizontalText = (DoubleText)pnlSetting.Controls[1];
            if ((verticalText.Value != setting.RobotSize.Vertical) || (horizontalText.Value != setting.RobotSize.Horizontal))
            {
                SizeData size = new SizeData();
                size.Vertical = verticalText.Value;
                size.Horizontal = horizontalText.Value;
                SetRobotSizeSettingCommand command = new SetRobotSizeSettingCommand(size);
                command.OnAnalyzed += evRobotSizeSettingChanged;
                usrCommControl.Send(command);
            }

            verticalText = (DoubleText)pnlSetting.Controls[2];
            horizontalText = (DoubleText)pnlSetting.Controls[3];
            if ((verticalText.Value != setting.FarmSize.Vertical) || (horizontalText.Value != setting.FarmSize.Horizontal))
            {
                SizeData size = new SizeData();
                size.Vertical = verticalText.Value;
                size.Horizontal = horizontalText.Value;
                SetFarmSizeSettingCommand command = new SetFarmSizeSettingCommand(size);
                command.OnAnalyzed += evFarmSizeSettingChanged;
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
                command.OnAnalyzed += evKusakariStartSettingChanged;
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
                command.OnAnalyzed += evYakeiStartSettingChanged;
                usrCommControl.Send(command);
            }

            FloatText moveEndRateText = (FloatText)pnlSetting.Controls[4];
            float rate = moveEndRateText.Value / 100.0F;
            if (setting.MoveEndRate != rate)
            {
                SetMoveEndRateCommand command = new SetMoveEndRateCommand(rate);
                command.OnAnalyzed += evMoveEndRateSettingChanged;
                usrCommControl.Send(command);
            }
        }

        /// <summary>
        /// 時刻合わせボタンクリックイベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void btnTimeAdjust_Click(object sender, EventArgs e)
        {
            TimeAdjustCommand command = new TimeAdjustCommand();
            usrCommControl.Send(command);
        }

        /// <summary>
        /// 設定受信完了時処理
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evSettingReceived(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            DoubleText doubleText = null;

            // ロボットの縦
            doubleText = (DoubleText)pnlSetting.Controls[0];
            doubleText.Value = setting.RobotSize.Vertical;

            // ロボットの横
            doubleText = (DoubleText)pnlSetting.Controls[1];
            doubleText.Value = setting.RobotSize.Horizontal;

            // 畑の縦
            doubleText = (DoubleText)pnlSetting.Controls[2];
            doubleText.Value = setting.FarmSize.Vertical;

            // 畑の横
            doubleText = (DoubleText)pnlSetting.Controls[3];
            doubleText.Value = setting.FarmSize.Horizontal;

            // 草刈り開始時刻
            usrKusakariStart.DayOfWeek = (int)setting.KusakariStart.DayOfWeek;
            usrKusakariStart.Hour = setting.KusakariStart.Hour;
            usrKusakariStart.Minute = setting.KusakariStart.Minulte;

            // 夜警開始時刻
            usrYakeiStart.DayOfWeek = (int)setting.YakeiStart.DayOfWeek;
            usrYakeiStart.Hour = setting.YakeiStart.Hour;
            usrYakeiStart.Minute = setting.YakeiStart.Minulte;

            // 動作完了判定閾値
            FloatText floatText = (FloatText)pnlSetting.Controls[4];
            floatText.Value = setting.MoveEndRate * 100;

            // エリアマップ生成
            AreaMap areaMap = AreaMap.GetInstance();
            areaMap.Allocate(setting.MapLength, setting.MapWidth);

            // 動作マップ生成
            MoveMap moveMap = MoveMap.GetInstance();
            moveMap.Allocate(setting.MapLength, setting.MapWidth);
        }

        /// <summary>
        /// ロボットサイズ設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evRobotSizeSettingChanged(object sender, EventArgs e)
        {
            DoubleText doubleText = null;
            SettingData setting = SettingData.GetInstance();

            // ロボットの縦
            doubleText = (DoubleText)pnlSetting.Controls[0];
            setting.RobotSize.Vertical = doubleText.Value;

            // ロボットの横
            doubleText = (DoubleText)pnlSetting.Controls[1];
            setting.RobotSize.Horizontal = doubleText.Value;
        }

        /// <summary>
        /// 畑のサイズ設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evFarmSizeSettingChanged(object sender, EventArgs e)
        {
            DoubleText doubleText = null;
            SettingData setting = SettingData.GetInstance();

            // 畑の縦
            doubleText = (DoubleText)pnlSetting.Controls[2];
            setting.FarmSize.Vertical = doubleText.Value;

            // 畑の横
            doubleText = (DoubleText)pnlSetting.Controls[3];
            setting.FarmSize.Horizontal = doubleText.Value;
        }

        /// <summary>
        /// 草刈り開始時刻設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evKusakariStartSettingChanged(object sender, EventArgs e)
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
        private void evYakeiStartSettingChanged(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            setting.YakeiStart.DayOfWeek = (DateTimeSetting.DayOfWeekEnum)usrYakeiStart.DayOfWeek;
            setting.YakeiStart.Hour = usrYakeiStart.Hour;
            setting.YakeiStart.Minulte = usrYakeiStart.Minute;
        }

        /// <summary>
        /// 動作完了判定閾値 設定変更後イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evMoveEndRateSettingChanged(object sender, EventArgs e)
        {
            SettingData setting = SettingData.GetInstance();
            FloatText moveEndRateText = (FloatText)pnlSetting.Controls[4];
            setting.MoveEndRate = moveEndRateText.Value;
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
            doubleText.Title = "ロボットサイズ(縦)";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 0);

            // ロボットの幅
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "ロボットサイズ(横)";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 1);

            // 畑の長さ
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "畑サイズ(縦)";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 2);

            // 畑の幅
            doubleText = new DoubleText(0.1, 10000);
            doubleText.Title = "畑サイズ(横)";
            doubleText.Unit = "mm";
            pnlSetting.Controls.Add(doubleText);
            pnlSetting.Controls.SetChildIndex(doubleText, 3);

            // 動作完了判定閾値
            FloatText floatText = new FloatText(0.0F, 100.0F);
            floatText.Title = "動作完了判定割合";
            floatText.Unit = "％";
            pnlSetting.Controls.Add(floatText);
            pnlSetting.Controls.SetChildIndex(floatText, 4);
        }

#endregion

#endregion

#region Monitor Tab ---------------------------------------------------------

#region Private Methods for Event Handler -----------------------------------

        /// <summary>
        /// モニタボタン状態変化イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void chkMonitor_CheckedChanged(object sender, EventArgs e)
        {
            if (chkMonitor.Checked == true)
            {
                // マップ更新
                usrMapMonitor.CreateMap();

                chkMonitor.Text = "モニター停止";
                m_MonitorSender.Start(200);
            }
            else
            {
                m_MonitorSender.Stop();
                chkMonitor.Text = "モニター開始";
            }
        }

        /// <summary>
        /// 動作マップモニタコマンド 受信完了イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evMoveMapMonitorReceived(object sender, EventArgs e)
        {
            if (this.InvokeRequired == true)
            {
                this.BeginInvoke(new Action(delegate
                {
                    usrMapMonitor.UpdateMapColor();
                }));
            }
            else
            {
                usrMapMonitor.UpdateMapColor();
            }
        }

        /// <summary>
        /// モニタ受信イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evMonitorReceived(object sender, EventArgs e)
        {
            if (this.InvokeRequired == true)
            {
                this.BeginInvoke(new Action(delegate
                {
                    foreach (Control obj in pnlStatus.Controls)
                    {
                        MonitorLabel label = (MonitorLabel)obj;
                        label.UpdateValue();
                    }

                    lblMovedRate.Text = string.Format("{0:F1}", MoveMap.GetInstance().MovedRate);
                    lblStateDateTime.Text = MonitorData.GetInstance().Commander.LastStartDate.ToString();
                }));
            }
            else
            {
                foreach (Control obj in pnlStatus.Controls)
                {
                    MonitorLabel label = (MonitorLabel)obj;
                    label.UpdateValue();
                }

                lblMovedRate.Text = string.Format("{0:F1}", MoveMap.GetInstance().MovedRate);
                lblStateDateTime.Text = MonitorData.GetInstance().Commander.LastStartDate.ToString();
            }
        }

#endregion

#region Private Methods -----------------------------------------------------

        /// <summary>
        /// モニタータブ初期化
        /// </summary>
        private void initMonitorTab()
        {
            MonitorLabel label = null;
            MonitorData monitor = MonitorData.GetInstance();

            // モニタ更新イベント登録
            m_MonitorSender.OnMonitorReceived += evMonitorReceived;

            // マップ更新イベント登録
            m_MonitorSender.OnMoveMapReceived += evMoveMapMonitorReceived;

            // ステータス
            label = new MonitorLabel("動作", monitor.Commander.CurrentSequence);
            pnlStatus.Controls.Add(label);
            label = new MonitorLabel("草刈り刃", monitor.Motor.Cutter);
            pnlStatus.Controls.Add(label);
            label = new MonitorLabel("操作モード", monitor.Motor.RemoteMode);
            pnlStatus.Controls.Add(label);
            label = new MonitorLabel("走行禁止エリア", monitor.FrontCamera.RedTape);
            pnlStatus.Controls.Add(label);
            label = new MonitorLabel("障害物", monitor.FrontCamera.BlueObject);
            pnlStatus.Controls.Add(label);
            label = new MonitorLabel("動物検知", monitor.AnimalCamera.Animal);
            pnlStatus.Controls.Add(label);
            label = new MonitorLabel("夜間検知", monitor.AroundCamera.Detect);
            pnlStatus.Controls.Add(label);
        }

#endregion

#endregion

#region Detail Tab ----------------------------------------------------------

#region Private Methods for Event Handler -----------------------------------

        /// <summary>
        /// モニタコマンド 受信完了イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void evDetailReceived(object sender, EventArgs e)
        {
            if (this.InvokeRequired == true)
            {
                this.BeginInvoke(new Action(delegate
                {
                    foreach (Control control in pnlSystemError.Controls)
                    {
                        MonitorLabel label = (MonitorLabel)control;
                        label.UpdateValue();
                    }
                }));
            }
            else
            {
                foreach (Control control in pnlSystemError.Controls)
                {
                    MonitorLabel label = (MonitorLabel)control;
                    label.UpdateValue();
                }
            }
        }

#endregion

#region Private Methods -----------------------------------------------------

        /// <summary>
        /// 詳細タブ初期化
        /// </summary>
        private void initDetailTab()
        {
            MonitorLabel label = null;
            MonitorData monitor = MonitorData.GetInstance();

            // 詳細更新イベント登録
            m_MonitorSender.OnMonitorReceived += evDetailReceived;

            // システムエラー状態
            label = new MonitorLabel("司令塔", monitor.Commander.SystemError);
            pnlSystemError.Controls.Add(label);
            label = new MonitorLabel("モータ", monitor.Motor.ErrorStatus);
            pnlSystemError.Controls.Add(label);
            label = new MonitorLabel("前方", monitor.FrontCamera.SystemError);
            pnlSystemError.Controls.Add(label);
            label = new MonitorLabel("動物", monitor.AnimalCamera.SystemError);
            pnlSystemError.Controls.Add(label);
            label = new MonitorLabel("周辺", monitor.AroundCamera.SystemError);
            pnlSystemError.Controls.Add(label);

            // ジャイロデータ
        }

#endregion

#endregion
    }
}
