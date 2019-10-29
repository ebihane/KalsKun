namespace DetailTool.Controls
{
    partial class MapGrid
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
            this.dgvMap = new System.Windows.Forms.DataGridView();
            ((System.ComponentModel.ISupportInitialize)(this.dgvMap)).BeginInit();
            this.SuspendLayout();
            // 
            // dgvMap
            // 
            this.dgvMap.AllowUserToAddRows = false;
            this.dgvMap.AllowUserToDeleteRows = false;
            this.dgvMap.AllowUserToResizeColumns = false;
            this.dgvMap.AllowUserToResizeRows = false;
            this.dgvMap.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            this.dgvMap.ColumnHeadersVisible = false;
            this.dgvMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvMap.Location = new System.Drawing.Point(0, 0);
            this.dgvMap.Name = "dgvMap";
            this.dgvMap.ReadOnly = true;
            this.dgvMap.RowHeadersVisible = false;
            this.dgvMap.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            this.dgvMap.RowTemplate.Height = 21;
            this.dgvMap.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.dgvMap.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dgvMap.Size = new System.Drawing.Size(500, 434);
            this.dgvMap.TabIndex = 0;
            // 
            // MapGrid
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.dgvMap);
            this.Name = "MapGrid";
            this.Size = new System.Drawing.Size(500, 434);
            ((System.ComponentModel.ISupportInitialize)(this.dgvMap)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dgvMap;
    }
}
