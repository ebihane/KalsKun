using DetailTool.Command;
using DetailTool.Components.Setting;
using DetailTool.Controls;
using System;
using System.Windows.Forms;

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
            FloatText floatText = null;

            usrCommControl.OnConnected += onConnected;
            usrCommControl.OnDisconnection += onDisconnection;

            // ロボットの長さ
            floatText = new FloatText(0.1F, 10000F);
            floatText.Title = "ロボットの長さ";
            floatText.Unit = "mm";
            pnlSetting.Controls.Add(floatText);
            pnlSetting.Controls.SetChildIndex(floatText, 0);

            // ロボットの幅
            floatText = new FloatText(0.1F, 10000F);
            floatText.Title = "ロボットの幅";
            floatText.Unit = "mm";
            pnlSetting.Controls.Add(floatText);
            pnlSetting.Controls.SetChildIndex(floatText, 1);

            // 畑の長さ
            floatText = new FloatText(0.1F, 10000F);
            floatText.Title = "畑の長さ";
            floatText.Unit = "mm";
            pnlSetting.Controls.Add(floatText);
            pnlSetting.Controls.SetChildIndex(floatText, 2);

            // 畑の幅
            floatText = new FloatText(0.1F, 10000F);
            floatText.Title = "ロボットの幅";
            floatText.Unit = "mm";
            pnlSetting.Controls.Add(floatText);
            pnlSetting.Controls.SetChildIndex(floatText, 1);
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
            if (pnlSettingMain.InvokeRequired == true)
            {
                pnlSetting.BeginInvoke(new Action<object>((obj) => { pnlSettingMain.Enabled = true; }), null);
            }
            else
            {
                pnlSettingMain.Enabled = true;
            }

            // 設定取得
            GetSettingCommand command = new GetSettingCommand();
            command.OnAnalyzed += onSettingReceived;
            usrCommControl.Send(command);
        }

        /// <summary>
        /// 切断イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void onDisconnection(object sender, EventArgs e)
        {
            if (pnlSettingMain.InvokeRequired == true)
            {
                pnlSettingMain.BeginInvoke(new Action<object>((obj) => { pnlSettingMain.Enabled = false; }), null);
            }
            else
            {
                pnlSettingMain.Enabled = false;
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
            FloatText floatText = null;

            // ロボットの長さ
            floatText = (FloatText)pnlSetting.Controls[0];
            floatText.Value = setting.RobotSize.Length;

            // ロボットの幅
            floatText = (FloatText)pnlSetting.Controls[1];
            floatText.Value = setting.RobotSize.Width;

            // 畑の長さ
            floatText = (FloatText)pnlSetting.Controls[2];
            floatText.Value = setting.FarmSize.Length;

            // 畑の幅
            floatText = (FloatText)pnlSetting.Controls[3];
            floatText.Value = setting.FarmSize.Width;

            // 草刈り開始時刻
            usrKusakariStart.DayOfWeek = (int)setting.KusakariStart.DayOfWeek;
            usrKusakariStart.Hour = setting.KusakariStart.Hour;
            usrKusakariStart.Minute = setting.KusakariStart.Minulte;

            // 夜警開始時刻
            usrYakeiStart.DayOfWeek = (int)setting.YakeiStart.DayOfWeek;
            usrYakeiStart.Hour = setting.YakeiStart.Hour;
            usrYakeiStart.Minute = setting.YakeiStart.Minulte;
        }

        #endregion
    }
}
