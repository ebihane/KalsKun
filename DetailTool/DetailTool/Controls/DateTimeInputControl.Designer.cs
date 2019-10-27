namespace DetailTool.Controls
{
    partial class DateTimeInputControl
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

        #region コンポーネント デザイナーで生成されたコード

        /// <summary> 
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.grpFrame = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cboMinute = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cboHour = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.cboDayOfWeek = new System.Windows.Forms.ComboBox();
            this.grpFrame.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpFrame
            // 
            this.grpFrame.Controls.Add(this.label4);
            this.grpFrame.Controls.Add(this.cboMinute);
            this.grpFrame.Controls.Add(this.label3);
            this.grpFrame.Controls.Add(this.cboHour);
            this.grpFrame.Controls.Add(this.label2);
            this.grpFrame.Controls.Add(this.label1);
            this.grpFrame.Controls.Add(this.cboDayOfWeek);
            this.grpFrame.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.grpFrame.Location = new System.Drawing.Point(3, 3);
            this.grpFrame.Name = "grpFrame";
            this.grpFrame.Size = new System.Drawing.Size(288, 99);
            this.grpFrame.TabIndex = 1;
            this.grpFrame.TabStop = false;
            this.grpFrame.Text = "groupBox1";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(244, 62);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(24, 16);
            this.label4.TabIndex = 7;
            this.label4.Text = "分";
            // 
            // cboMinute
            // 
            this.cboMinute.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboMinute.FormattingEnabled = true;
            this.cboMinute.Location = new System.Drawing.Point(188, 59);
            this.cboMinute.Name = "cboMinute";
            this.cboMinute.Size = new System.Drawing.Size(50, 24);
            this.cboMinute.TabIndex = 6;
            this.cboMinute.SelectedIndexChanged += new System.EventHandler(this.cboMinute_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(158, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 16);
            this.label3.TabIndex = 5;
            this.label3.Text = "時";
            // 
            // cboHour
            // 
            this.cboHour.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboHour.FormattingEnabled = true;
            this.cboHour.Location = new System.Drawing.Point(102, 59);
            this.cboHour.Name = "cboHour";
            this.cboHour.Size = new System.Drawing.Size(50, 24);
            this.cboHour.TabIndex = 4;
            this.cboHour.SelectedIndexChanged += new System.EventHandler(this.cboHour_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 62);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "開始時刻";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 16);
            this.label1.TabIndex = 2;
            this.label1.Text = "曜日";
            // 
            // cboDayOfWeek
            // 
            this.cboDayOfWeek.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboDayOfWeek.FormattingEnabled = true;
            this.cboDayOfWeek.Items.AddRange(new object[] {
            "日曜日",
            "月曜日",
            "火曜日",
            "水曜日",
            "木曜日",
            "金曜日",
            "土曜日",
            "毎日",
            "無し"});
            this.cboDayOfWeek.Location = new System.Drawing.Point(102, 20);
            this.cboDayOfWeek.Name = "cboDayOfWeek";
            this.cboDayOfWeek.Size = new System.Drawing.Size(166, 24);
            this.cboDayOfWeek.TabIndex = 1;
            this.cboDayOfWeek.SelectedIndexChanged += new System.EventHandler(this.cboDayOfWeek_SelectedIndexChanged);
            // 
            // DateTimeInputControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.grpFrame);
            this.Name = "DateTimeInputControl";
            this.Size = new System.Drawing.Size(299, 111);
            this.grpFrame.ResumeLayout(false);
            this.grpFrame.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox grpFrame;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cboMinute;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cboHour;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cboDayOfWeek;
    }
}
