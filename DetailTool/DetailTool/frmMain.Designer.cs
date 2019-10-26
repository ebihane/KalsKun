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
            this.usrYakeiStart = new DetailTool.Controls.DateTimeInputControl();
            this.usrKusakariStart = new DetailTool.Controls.DateTimeInputControl();
            this.pnlSetting = new System.Windows.Forms.FlowLayoutPanel();
            this.usrCommControl = new DetailTool.Controls.CommControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.pnlSettingMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.tabControl1.Location = new System.Drawing.Point(10, 14);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(778, 464);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.pnlSettingMain);
            this.tabPage1.Controls.Add(this.usrCommControl);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(770, 438);
            this.tabPage1.TabIndex = 1;
            this.tabPage1.Text = "設定";
            // 
            // pnlSettingMain
            // 
            this.pnlSettingMain.Controls.Add(this.usrYakeiStart);
            this.pnlSettingMain.Controls.Add(this.usrKusakariStart);
            this.pnlSettingMain.Controls.Add(this.pnlSetting);
            this.pnlSettingMain.Enabled = false;
            this.pnlSettingMain.Location = new System.Drawing.Point(6, 105);
            this.pnlSettingMain.Name = "pnlSettingMain";
            this.pnlSettingMain.Size = new System.Drawing.Size(758, 327);
            this.pnlSettingMain.TabIndex = 4;
            // 
            // usrYakeiStart
            // 
            this.usrYakeiStart.DayOfWeek = -1;
            this.usrYakeiStart.Hour = -1;
            this.usrYakeiStart.Location = new System.Drawing.Point(393, 120);
            this.usrYakeiStart.Minute = -1;
            this.usrYakeiStart.Name = "usrYakeiStart";
            this.usrYakeiStart.Size = new System.Drawing.Size(299, 111);
            this.usrYakeiStart.TabIndex = 6;
            this.usrYakeiStart.Title = "夜警";
            // 
            // usrKusakariStart
            // 
            this.usrKusakariStart.DayOfWeek = -1;
            this.usrKusakariStart.Hour = -1;
            this.usrKusakariStart.Location = new System.Drawing.Point(393, 3);
            this.usrKusakariStart.Minute = -1;
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
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(770, 438);
            this.tabPage2.TabIndex = 3;
            this.tabPage2.Text = "モニター";
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(770, 438);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "ログ";
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
    }
}

