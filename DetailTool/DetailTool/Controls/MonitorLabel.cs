using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DetailTool.Components.Monitor.Items;

namespace DetailTool.Controls
{
    /// <summary>
    /// モニタデータ表示専用ラベル
    /// </summary>
    public partial class MonitorLabel : UserControl
    {
        /// <summary>
        /// ステータスデータ
        /// </summary>
        private StatusItemBase m_StatusItem = null;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="text">タイトル</param>
        /// <param name="item">表示対象のステータスデータクラスインスタンス</param>
        public MonitorLabel(string title, StatusItemBase item)
        {
            InitializeComponent();
            lblTitle.Text = title;
            m_StatusItem = item;
        }

        /// <summary>
        /// 表示更新
        /// </summary>
        public void UpdateValue()
        {
            string text = m_StatusItem.ToString();
            System.Drawing.Color backColor = m_StatusItem.GetBackColor();

            if (lblValue.InvokeRequired == true)
            {
                lblTitle.BeginInvoke(new Action<string, System.Drawing.Color>(
                (str, color) =>
                {
                    lblValue.Text = str;
                    lblValue.BackColor = color;
                }),
                text, backColor);
            }
            else
            {
                lblTitle.Text = text;
                lblTitle.BackColor = backColor;
            }
        }
    }
}
