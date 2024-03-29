﻿using System;
using System.Windows.Forms;

namespace DetailTool.Controls
{
    public partial class DateTimeInputControl : UserControl
    {
        private int m_DayOfWeek = 0;
        private int m_Hour = 0;
        private int m_Minute = 0;

        public DateTimeInputControl()
        {
            InitializeComponent();

            for (int hour = 0; hour <= 23; hour++)
            {
                cboHour.Items.Add(hour);
            }

            for (int minute = 0; minute <= 59; minute++)
            {
                cboMinute.Items.Add(minute);
            }
        }

        public string Title
        {
            get
            {
                return grpFrame.Text;
            }

            set
            {
                grpFrame.Text = value;
            }
        }

        public int DayOfWeek
        {
            get
            {
                return m_DayOfWeek;
            }

            set
            {
                if ((value < 0) || (9 <= value))
                {
                    goto FINISH;
                }

                if (cboDayOfWeek.InvokeRequired == true)
                {
                    cboDayOfWeek.BeginInvoke(new Action<int>(
                    (index) =>
                    {
                        cboDayOfWeek.SelectedIndex = index;
                    }),
                    value);
                }
                else
                {
                    cboDayOfWeek.SelectedIndex = value;
                }

                m_DayOfWeek = value;

            FINISH:
                return;
            }
        }

        /// <summary>
        /// Gets or set 時
        /// </summary>
        public int Hour
        {
            get
            {
                return m_Hour;
            }

            set
            {
                if ((value < 0) || (24 <= value))
                {
                    goto FINISH;
                }

                if (cboHour.InvokeRequired == true)
                {
                    cboHour.BeginInvoke(new Action<int>(
                    (index) =>
                    {
                        cboHour.SelectedIndex = index;
                    }),
                    value);
                }
                else
                {
                    cboHour.SelectedIndex = value;
                }

                m_Hour = value;

            FINISH:
                return;
            }
        }

        /// <summary>
        /// Gets or sets 分
        /// </summary>
        public int Minute
        {
            get
            {
                return m_Minute;
            }

            set
            {
                if ((value < 0) || (60 <= value))
                {
                    goto FINISH;
                }

                if (cboMinute.InvokeRequired == true)
                {
                    cboMinute.BeginInvoke(new Action<int>(
                    (index) =>
                    {
                        cboMinute.SelectedIndex = index;
                    }),
                    value);
                }
                else
                {
                    cboMinute.SelectedIndex = value;
                }

                m_Minute = value;

            FINISH:
                return;
            }
        }

        /// <summary>
        /// 曜日リスト 変更イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void cboDayOfWeek_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_DayOfWeek = cboDayOfWeek.SelectedIndex;
        }

        /// <summary>
        /// 時リスト 変更イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void cboHour_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_Hour = cboHour.SelectedIndex;
        }

        /// <summary>
        /// 分リスト 変更イベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void cboMinute_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_Minute = cboMinute.SelectedIndex;
        }
    }
}
