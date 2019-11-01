namespace DetailTool.Controls
{
    partial class CommControl
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
            this.btnConnection = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lblState = new System.Windows.Forms.Label();
            this.pnlInputField = new System.Windows.Forms.Panel();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.txtIp4 = new System.Windows.Forms.TextBox();
            this.txtIp3 = new System.Windows.Forms.TextBox();
            this.txtIp2 = new System.Windows.Forms.TextBox();
            this.txtIp1 = new System.Windows.Forms.TextBox();
            this.pnlInputField.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnConnection
            // 
            this.btnConnection.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnConnection.Location = new System.Drawing.Point(308, 7);
            this.btnConnection.Name = "btnConnection";
            this.btnConnection.Size = new System.Drawing.Size(107, 46);
            this.btnConnection.TabIndex = 0;
            this.btnConnection.Text = "接続";
            this.btnConnection.UseVisualStyleBackColor = true;
            this.btnConnection.Click += new System.EventHandler(this.btnConnection_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label1.Location = new System.Drawing.Point(4, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 16);
            this.label1.TabIndex = 4;
            this.label1.Text = "IP Address";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label2.Location = new System.Drawing.Point(4, 33);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(61, 16);
            this.label2.TabIndex = 5;
            this.label2.Text = "Port No";
            // 
            // lblState
            // 
            this.lblState.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblState.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblState.Location = new System.Drawing.Point(6, 61);
            this.lblState.Name = "lblState";
            this.lblState.Size = new System.Drawing.Size(485, 25);
            this.lblState.TabIndex = 7;
            this.lblState.Text = "接続していません";
            this.lblState.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pnlInputField
            // 
            this.pnlInputField.Controls.Add(this.txtPort);
            this.pnlInputField.Controls.Add(this.txtIp4);
            this.pnlInputField.Controls.Add(this.txtIp3);
            this.pnlInputField.Controls.Add(this.txtIp2);
            this.pnlInputField.Controls.Add(this.txtIp1);
            this.pnlInputField.Location = new System.Drawing.Point(91, 3);
            this.pnlInputField.Name = "pnlInputField";
            this.pnlInputField.Size = new System.Drawing.Size(211, 55);
            this.pnlInputField.TabIndex = 8;
            // 
            // txtPort
            // 
            this.txtPort.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtPort.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.txtPort.Location = new System.Drawing.Point(4, 29);
            this.txtPort.MaxLength = 5;
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(98, 23);
            this.txtPort.TabIndex = 11;
            this.txtPort.Text = "20001";
            this.txtPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtIp4
            // 
            this.txtIp4.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtIp4.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.txtIp4.Location = new System.Drawing.Point(160, 2);
            this.txtIp4.MaxLength = 3;
            this.txtIp4.Name = "txtIp4";
            this.txtIp4.Size = new System.Drawing.Size(46, 23);
            this.txtIp4.TabIndex = 8;
            this.txtIp4.Text = "1";
            this.txtIp4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtIp3
            // 
            this.txtIp3.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtIp3.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.txtIp3.Location = new System.Drawing.Point(108, 2);
            this.txtIp3.MaxLength = 3;
            this.txtIp3.Name = "txtIp3";
            this.txtIp3.Size = new System.Drawing.Size(46, 23);
            this.txtIp3.TabIndex = 10;
            this.txtIp3.Text = "3";
            this.txtIp3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtIp2
            // 
            this.txtIp2.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtIp2.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.txtIp2.Location = new System.Drawing.Point(56, 2);
            this.txtIp2.MaxLength = 3;
            this.txtIp2.Name = "txtIp2";
            this.txtIp2.Size = new System.Drawing.Size(46, 23);
            this.txtIp2.TabIndex = 9;
            this.txtIp2.Text = "168";
            this.txtIp2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtIp1
            // 
            this.txtIp1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtIp1.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.txtIp1.Location = new System.Drawing.Point(4, 2);
            this.txtIp1.MaxLength = 3;
            this.txtIp1.Name = "txtIp1";
            this.txtIp1.Size = new System.Drawing.Size(46, 23);
            this.txtIp1.TabIndex = 7;
            this.txtIp1.Text = "192";
            this.txtIp1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // CommControl
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.pnlInputField);
            this.Controls.Add(this.lblState);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnConnection);
            this.Name = "CommControl";
            this.Size = new System.Drawing.Size(500, 93);
            this.pnlInputField.ResumeLayout(false);
            this.pnlInputField.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnection;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblState;
        private System.Windows.Forms.Panel pnlInputField;
        private System.Windows.Forms.TextBox txtPort;
        private System.Windows.Forms.TextBox txtIp4;
        private System.Windows.Forms.TextBox txtIp3;
        private System.Windows.Forms.TextBox txtIp2;
        private System.Windows.Forms.TextBox txtIp1;
    }
}
