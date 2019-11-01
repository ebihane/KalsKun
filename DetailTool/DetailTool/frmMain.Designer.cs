namespace DetailTool
{
    partial class frmMain
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.pnlSettingMain = new System.Windows.Forms.Panel();
            this.btnTimeAdjust = new System.Windows.Forms.Button();
            this.btnSettingChange = new System.Windows.Forms.Button();
            this.btnSettingGet = new System.Windows.Forms.Button();
            this.usrYakeiStart = new DetailTool.Controls.DateTimeInputControl();
            this.usrKusakariStart = new DetailTool.Controls.DateTimeInputControl();
            this.pnlSetting = new System.Windows.Forms.FlowLayoutPanel();
            this.usrCommControl = new DetailTool.Controls.CommControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.rdoErrorStatus = new System.Windows.Forms.RadioButton();
            this.pnlSystemError = new System.Windows.Forms.FlowLayoutPanel();
            this.pnlDetail = new System.Windows.Forms.FlowLayoutPanel();
            this.rdoDetail = new System.Windows.Forms.RadioButton();
            this.rdoMonitor = new System.Windows.Forms.RadioButton();
            this.lblStateDateTime = new System.Windows.Forms.Label();
            this.lblStartDateTitle = new System.Windows.Forms.Label();
            this.chkMonitor = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lblMovedRate = new System.Windows.Forms.Label();
            this.lblMovedRateTitle = new System.Windows.Forms.Label();
            this.pnlMonitor = new System.Windows.Forms.FlowLayoutPanel();
            this.usrMapMonitor = new DetailTool.Controls.MapGrid();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.pnlSettingMain.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.tabControl1.Location = new System.Drawing.Point(10, 14);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(778, 441);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.pnlSettingMain);
            this.tabPage1.Controls.Add(this.usrCommControl);
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(770, 411);
            this.tabPage1.TabIndex = 1;
            this.tabPage1.Text = "設定";
            // 
            // pnlSettingMain
            // 
            this.pnlSettingMain.Controls.Add(this.btnTimeAdjust);
            this.pnlSettingMain.Controls.Add(this.btnSettingChange);
            this.pnlSettingMain.Controls.Add(this.btnSettingGet);
            this.pnlSettingMain.Controls.Add(this.usrYakeiStart);
            this.pnlSettingMain.Controls.Add(this.usrKusakariStart);
            this.pnlSettingMain.Controls.Add(this.pnlSetting);
            this.pnlSettingMain.Enabled = false;
            this.pnlSettingMain.Location = new System.Drawing.Point(6, 105);
            this.pnlSettingMain.Name = "pnlSettingMain";
            this.pnlSettingMain.Size = new System.Drawing.Size(764, 310);
            this.pnlSettingMain.TabIndex = 4;
            // 
            // btnTimeAdjust
            // 
            this.btnTimeAdjust.Enabled = false;
            this.btnTimeAdjust.Location = new System.Drawing.Point(642, 3);
            this.btnTimeAdjust.Name = "btnTimeAdjust";
            this.btnTimeAdjust.Size = new System.Drawing.Size(119, 52);
            this.btnTimeAdjust.TabIndex = 8;
            this.btnTimeAdjust.Text = "時刻合わせ";
            this.btnTimeAdjust.UseVisualStyleBackColor = true;
            this.btnTimeAdjust.Click += new System.EventHandler(this.btnTimeAdjust_Click);
            // 
            // btnSettingChange
            // 
            this.btnSettingChange.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnSettingChange.Location = new System.Drawing.Point(518, 3);
            this.btnSettingChange.Name = "btnSettingChange";
            this.btnSettingChange.Size = new System.Drawing.Size(119, 52);
            this.btnSettingChange.TabIndex = 7;
            this.btnSettingChange.Text = "設定変更";
            this.btnSettingChange.UseVisualStyleBackColor = true;
            this.btnSettingChange.Click += new System.EventHandler(this.btnSettingChange_Click);
            // 
            // btnSettingGet
            // 
            this.btnSettingGet.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnSettingGet.Location = new System.Drawing.Point(393, 3);
            this.btnSettingGet.Name = "btnSettingGet";
            this.btnSettingGet.Size = new System.Drawing.Size(119, 52);
            this.btnSettingGet.TabIndex = 5;
            this.btnSettingGet.Text = "設定取得";
            this.btnSettingGet.UseVisualStyleBackColor = true;
            this.btnSettingGet.Click += new System.EventHandler(this.btnSettingGet_Click);
            // 
            // usrYakeiStart
            // 
            this.usrYakeiStart.DayOfWeek = 0;
            this.usrYakeiStart.Hour = 0;
            this.usrYakeiStart.Location = new System.Drawing.Point(393, 193);
            this.usrYakeiStart.Minute = 0;
            this.usrYakeiStart.Name = "usrYakeiStart";
            this.usrYakeiStart.Size = new System.Drawing.Size(299, 111);
            this.usrYakeiStart.TabIndex = 6;
            this.usrYakeiStart.Title = "夜警";
            // 
            // usrKusakariStart
            // 
            this.usrKusakariStart.DayOfWeek = 0;
            this.usrKusakariStart.Hour = 0;
            this.usrKusakariStart.Location = new System.Drawing.Point(393, 76);
            this.usrKusakariStart.Minute = 0;
            this.usrKusakariStart.Name = "usrKusakariStart";
            this.usrKusakariStart.Size = new System.Drawing.Size(299, 111);
            this.usrKusakariStart.TabIndex = 5;
            this.usrKusakariStart.Title = "草刈り";
            // 
            // pnlSetting
            // 
            this.pnlSetting.Location = new System.Drawing.Point(3, 3);
            this.pnlSetting.Name = "pnlSetting";
            this.pnlSetting.Size = new System.Drawing.Size(384, 301);
            this.pnlSetting.TabIndex = 4;
            // 
            // usrCommControl
            // 
            this.usrCommControl.Location = new System.Drawing.Point(6, 6);
            this.usrCommControl.Name = "usrCommControl";
            this.usrCommControl.Size = new System.Drawing.Size(497, 93);
            this.usrCommControl.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage2.Controls.Add(this.rdoErrorStatus);
            this.tabPage2.Controls.Add(this.pnlSystemError);
            this.tabPage2.Controls.Add(this.pnlDetail);
            this.tabPage2.Controls.Add(this.rdoDetail);
            this.tabPage2.Controls.Add(this.rdoMonitor);
            this.tabPage2.Controls.Add(this.lblStateDateTime);
            this.tabPage2.Controls.Add(this.lblStartDateTitle);
            this.tabPage2.Controls.Add(this.chkMonitor);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.lblMovedRate);
            this.tabPage2.Controls.Add(this.lblMovedRateTitle);
            this.tabPage2.Controls.Add(this.pnlMonitor);
            this.tabPage2.Controls.Add(this.usrMapMonitor);
            this.tabPage2.Location = new System.Drawing.Point(4, 26);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(770, 411);
            this.tabPage2.TabIndex = 3;
            this.tabPage2.Text = "モニター";
            // 
            // rdoErrorStatus
            // 
            this.rdoErrorStatus.Appearance = System.Windows.Forms.Appearance.Button;
            this.rdoErrorStatus.Location = new System.Drawing.Point(684, 354);
            this.rdoErrorStatus.Name = "rdoErrorStatus";
            this.rdoErrorStatus.Size = new System.Drawing.Size(86, 53);
            this.rdoErrorStatus.TabIndex = 17;
            this.rdoErrorStatus.Text = "エラー状態";
            this.rdoErrorStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.rdoErrorStatus.UseVisualStyleBackColor = true;
            this.rdoErrorStatus.CheckedChanged += new System.EventHandler(this.RdoErrorStatus_CheckedChanged);
            // 
            // pnlSystemError
            // 
            this.pnlSystemError.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.pnlSystemError.Location = new System.Drawing.Point(503, 75);
            this.pnlSystemError.Name = "pnlSystemError";
            this.pnlSystemError.Size = new System.Drawing.Size(264, 273);
            this.pnlSystemError.TabIndex = 0;
            this.pnlSystemError.Visible = false;
            // 
            // pnlDetail
            // 
            this.pnlDetail.Location = new System.Drawing.Point(503, 75);
            this.pnlDetail.Name = "pnlDetail";
            this.pnlDetail.Size = new System.Drawing.Size(264, 273);
            this.pnlDetail.TabIndex = 16;
            this.pnlDetail.Visible = false;
            // 
            // rdoDetail
            // 
            this.rdoDetail.Appearance = System.Windows.Forms.Appearance.Button;
            this.rdoDetail.Location = new System.Drawing.Point(595, 354);
            this.rdoDetail.Name = "rdoDetail";
            this.rdoDetail.Size = new System.Drawing.Size(83, 53);
            this.rdoDetail.TabIndex = 15;
            this.rdoDetail.Text = "詳細";
            this.rdoDetail.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.rdoDetail.UseVisualStyleBackColor = true;
            this.rdoDetail.CheckedChanged += new System.EventHandler(this.RdoDetail_CheckedChanged);
            // 
            // rdoMonitor
            // 
            this.rdoMonitor.Appearance = System.Windows.Forms.Appearance.Button;
            this.rdoMonitor.Checked = true;
            this.rdoMonitor.Location = new System.Drawing.Point(503, 354);
            this.rdoMonitor.Name = "rdoMonitor";
            this.rdoMonitor.Size = new System.Drawing.Size(86, 53);
            this.rdoMonitor.TabIndex = 14;
            this.rdoMonitor.TabStop = true;
            this.rdoMonitor.Text = "モニター";
            this.rdoMonitor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.rdoMonitor.UseVisualStyleBackColor = true;
            this.rdoMonitor.CheckedChanged += new System.EventHandler(this.RdoMonitor_CheckedChanged);
            // 
            // lblStateDateTime
            // 
            this.lblStateDateTime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblStateDateTime.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblStateDateTime.Location = new System.Drawing.Point(79, 3);
            this.lblStateDateTime.Name = "lblStateDateTime";
            this.lblStateDateTime.Size = new System.Drawing.Size(178, 23);
            this.lblStateDateTime.TabIndex = 13;
            this.lblStateDateTime.Text = "2019/11/02 11:22:33";
            this.lblStateDateTime.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lblStartDateTitle
            // 
            this.lblStartDateTitle.AutoSize = true;
            this.lblStartDateTitle.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblStartDateTitle.Location = new System.Drawing.Point(3, 6);
            this.lblStartDateTitle.Name = "lblStartDateTitle";
            this.lblStartDateTitle.Size = new System.Drawing.Size(72, 16);
            this.lblStartDateTitle.TabIndex = 12;
            this.lblStartDateTitle.Text = "開始日時";
            // 
            // chkMonitor
            // 
            this.chkMonitor.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkMonitor.Enabled = false;
            this.chkMonitor.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.chkMonitor.Location = new System.Drawing.Point(503, 3);
            this.chkMonitor.Name = "chkMonitor";
            this.chkMonitor.Size = new System.Drawing.Size(264, 66);
            this.chkMonitor.TabIndex = 11;
            this.chkMonitor.Text = "モニター開始";
            this.chkMonitor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkMonitor.UseVisualStyleBackColor = true;
            this.chkMonitor.CheckedChanged += new System.EventHandler(this.chkMonitor_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label1.Location = new System.Drawing.Point(475, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(24, 16);
            this.label1.TabIndex = 10;
            this.label1.Text = "％";
            // 
            // lblMovedRate
            // 
            this.lblMovedRate.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblMovedRate.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblMovedRate.Location = new System.Drawing.Point(387, 3);
            this.lblMovedRate.Name = "lblMovedRate";
            this.lblMovedRate.Size = new System.Drawing.Size(82, 23);
            this.lblMovedRate.TabIndex = 9;
            this.lblMovedRate.Text = "-";
            this.lblMovedRate.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lblMovedRateTitle
            // 
            this.lblMovedRateTitle.AutoSize = true;
            this.lblMovedRateTitle.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblMovedRateTitle.Location = new System.Drawing.Point(311, 6);
            this.lblMovedRateTitle.Name = "lblMovedRateTitle";
            this.lblMovedRateTitle.Size = new System.Drawing.Size(70, 16);
            this.lblMovedRateTitle.TabIndex = 8;
            this.lblMovedRateTitle.Text = "走行済み";
            // 
            // pnlMonitor
            // 
            this.pnlMonitor.Location = new System.Drawing.Point(503, 75);
            this.pnlMonitor.Name = "pnlMonitor";
            this.pnlMonitor.Size = new System.Drawing.Size(264, 273);
            this.pnlMonitor.TabIndex = 2;
            // 
            // usrMapMonitor
            // 
            this.usrMapMonitor.Location = new System.Drawing.Point(0, 27);
            this.usrMapMonitor.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.usrMapMonitor.Name = "usrMapMonitor";
            this.usrMapMonitor.Size = new System.Drawing.Size(500, 380);
            this.usrMapMonitor.TabIndex = 1;
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 463);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.Text = "刈ルス君ツール";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmMain_FormClosing);
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.pnlSettingMain.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private Controls.CommControl usrCommControl;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Panel pnlSettingMain;
        private Controls.DateTimeInputControl usrYakeiStart;
        private Controls.DateTimeInputControl usrKusakariStart;
        private System.Windows.Forms.FlowLayoutPanel pnlSetting;
        private System.Windows.Forms.Button btnSettingGet;
        private System.Windows.Forms.Button btnSettingChange;
        private Controls.MapGrid usrMapMonitor;
        private System.Windows.Forms.FlowLayoutPanel pnlMonitor;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblMovedRate;
        private System.Windows.Forms.Label lblMovedRateTitle;
        private System.Windows.Forms.Button btnTimeAdjust;
        private System.Windows.Forms.CheckBox chkMonitor;
        private System.Windows.Forms.Label lblStateDateTime;
        private System.Windows.Forms.Label lblStartDateTitle;
        private System.Windows.Forms.RadioButton rdoMonitor;
        private System.Windows.Forms.FlowLayoutPanel pnlSystemError;
        private System.Windows.Forms.RadioButton rdoDetail;
        private System.Windows.Forms.RadioButton rdoErrorStatus;
        private System.Windows.Forms.FlowLayoutPanel pnlDetail;
    }
}

