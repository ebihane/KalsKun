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
            this.btnTimeAdjust = new System.Windows.Forms.Button();
            this.chkMonitor = new System.Windows.Forms.CheckBox();
            this.pnlSettingMain = new System.Windows.Forms.Panel();
            this.btnSettingChange = new System.Windows.Forms.Button();
            this.btnSettingGet = new System.Windows.Forms.Button();
            this.usrYakeiStart = new DetailTool.Controls.DateTimeInputControl();
            this.usrKusakariStart = new DetailTool.Controls.DateTimeInputControl();
            this.pnlSetting = new System.Windows.Forms.FlowLayoutPanel();
            this.usrCommControl = new DetailTool.Controls.CommControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.pnlSystemError = new System.Windows.Forms.FlowLayoutPanel();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.pnlSettingMain.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.tabControl1.Location = new System.Drawing.Point(10, 14);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(778, 464);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.btnTimeAdjust);
            this.tabPage1.Controls.Add(this.chkMonitor);
            this.tabPage1.Controls.Add(this.pnlSettingMain);
            this.tabPage1.Controls.Add(this.usrCommControl);
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(770, 434);
            this.tabPage1.TabIndex = 1;
            this.tabPage1.Text = "設定";
            // 
            // btnTimeAdjust
            // 
            this.btnTimeAdjust.Enabled = false;
            this.btnTimeAdjust.Location = new System.Drawing.Point(524, 6);
            this.btnTimeAdjust.Name = "btnTimeAdjust";
            this.btnTimeAdjust.Size = new System.Drawing.Size(119, 48);
            this.btnTimeAdjust.TabIndex = 6;
            this.btnTimeAdjust.Text = "時刻合わせ";
            this.btnTimeAdjust.UseVisualStyleBackColor = true;
            this.btnTimeAdjust.Click += new System.EventHandler(this.btnTimeAdjust_Click);
            // 
            // chkMonitor
            // 
            this.chkMonitor.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkMonitor.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.chkMonitor.Location = new System.Drawing.Point(648, 6);
            this.chkMonitor.Name = "chkMonitor";
            this.chkMonitor.Size = new System.Drawing.Size(119, 48);
            this.chkMonitor.TabIndex = 5;
            this.chkMonitor.Text = "モニター開始";
            this.chkMonitor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkMonitor.UseVisualStyleBackColor = true;
            this.chkMonitor.CheckedChanged += new System.EventHandler(this.chkMonitor_CheckedChanged);
            // 
            // pnlSettingMain
            // 
            this.pnlSettingMain.Controls.Add(this.btnSettingChange);
            this.pnlSettingMain.Controls.Add(this.btnSettingGet);
            this.pnlSettingMain.Controls.Add(this.usrYakeiStart);
            this.pnlSettingMain.Controls.Add(this.usrKusakariStart);
            this.pnlSettingMain.Controls.Add(this.pnlSetting);
            this.pnlSettingMain.Enabled = false;
            this.pnlSettingMain.Location = new System.Drawing.Point(6, 105);
            this.pnlSettingMain.Name = "pnlSettingMain";
            this.pnlSettingMain.Size = new System.Drawing.Size(758, 327);
            this.pnlSettingMain.TabIndex = 4;
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
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Location = new System.Drawing.Point(4, 26);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(770, 434);
            this.tabPage2.TabIndex = 3;
            this.tabPage2.Text = "モニター";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.pnlSystemError);
            this.groupBox1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.groupBox1.Location = new System.Drawing.Point(501, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(266, 202);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "エラー状態";
            // 
            // pnlSystemError
            // 
            this.pnlSystemError.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.pnlSystemError.Location = new System.Drawing.Point(2, 22);
            this.pnlSystemError.Name = "pnlSystemError";
            this.pnlSystemError.Size = new System.Drawing.Size(262, 174);
            this.pnlSystemError.TabIndex = 0;
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage3.Controls.Add(this.dataGridView1);
            this.tabPage3.Location = new System.Drawing.Point(4, 26);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(770, 434);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "マップ";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(3, 3);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowTemplate.Height = 21;
            this.dataGridView1.Size = new System.Drawing.Size(663, 428);
            this.dataGridView1.TabIndex = 0;
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 489);
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
            this.groupBox1.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage3;
        private Controls.CommControl usrCommControl;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Panel pnlSettingMain;
        private Controls.DateTimeInputControl usrYakeiStart;
        private Controls.DateTimeInputControl usrKusakariStart;
        private System.Windows.Forms.FlowLayoutPanel pnlSetting;
        private System.Windows.Forms.Button btnSettingGet;
        private System.Windows.Forms.Button btnSettingChange;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.FlowLayoutPanel pnlSystemError;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.CheckBox chkMonitor;
        private System.Windows.Forms.Button btnTimeAdjust;
    }
}

