﻿using System;
using System.Windows.Forms;

namespace DetailTool.Controls
{
    public partial class DoubleText : UserControl
    {
        private double m_Value = 0;
        private double m_Min = 0;
        private double m_Max = 0;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="min">入力範囲 最小値</param>
        /// <param name="max">入力範囲 最大値</param>
        public DoubleText(double min, double max)
        {
            InitializeComponent();

            m_Min = min;
            m_Max = max;
        }

        /// <summary>
        /// Gets or sets タイトル
        /// </summary>
        public string Title
        {
            get
            {
                return lblTitle.Text;
            }

            set
            {
                if (lblTitle.InvokeRequired == true)
                {
                    lblTitle.BeginInvoke(new Action<string>((text) => { lblTitle.Text = text; }), value);
                }
                else
                {
                    lblTitle.Text = value;
                }
            }
        }

        /// <summary>
        /// Gets or sets 単位
        /// </summary>
        public string Unit
        {
            get
            {
                return lblUnit.Text;
            }

            set
            {
                if (lblUnit.InvokeRequired == true)
                {
                    lblUnit.BeginInvoke(new Action<string>((text) => { lblUnit.Text = text; }), value);
                }
                else
                {
                    lblUnit.Text = value;
                }
            }
        }

        /// <summary>
        /// Gets or sets 値
        /// </summary>
        public double Value
        {
            get
            {
                return m_Value;
            }
            
            set
            {
                if (txtValue.InvokeRequired == true)
                {
                    txtValue.BeginInvoke(new Action<double>((val) => { txtValue.Text = val.ToString(); }), value);
                }
                else
                {
                    txtValue.Text = value.ToString();
                }

                m_Value = value;
            }
        }

        /// <summary>
        /// テキストボックス キー入力イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void txtValue_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.Enter)
            {
                string errorMessage = validateString(txtValue.Text);
                if (errorMessage != string.Empty)
                {
                    MessageBox.Show(errorMessage, "Error");
                    txtValue.Text = m_Value.ToString();
                    goto FINISH;
                }

                m_Value = double.Parse(txtValue.Text);
            }

        FINISH:
            return;
        }

        /// <summary>
        /// テキストボックス フォーカス移動イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void txtValue_Leave(object sender, EventArgs e)
        {
            string errorMessage = validateString(txtValue.Text);
            if (errorMessage != string.Empty)
            {
                MessageBox.Show(errorMessage, "Error");
                txtValue.Text = m_Value.ToString();
                goto FINISH;
            }

            m_Value = double.Parse(txtValue.Text);

        FINISH:
            return;
        }

        /// <summary>
        /// 入力値判定
        /// </summary>
        /// <param name="valueStr">入力文字列</param>
        /// <returns>エラーメッセージ (string.Empty:OK)</returns>
        private string validateString(string valueStr)
        {
            string retVal = "予期しない異常が発生しました。";

            double value = 0;
            bool result = double.TryParse(valueStr, out value);
            if (result != true)
            {
                retVal = "数字が入力されていません。";
                goto FINISH;
            }

            if ((value < m_Min) || (m_Max < value))
            {
                retVal = string.Format("入力された数値が範囲外です。({0} ～ {1})", m_Min, m_Max);
                goto FINISH;
            }

            retVal = string.Empty;

        FINISH:
            return retVal;
        }
    }
}
