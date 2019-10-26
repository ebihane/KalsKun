using System;
using System.Windows.Forms;

namespace DetailTool.Controls
{
    public partial class DateTimeInputControl : UserControl
    {
        public DateTimeInputControl()
        {
            InitializeComponent();
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
                return cboDayOfWeek.SelectedIndex;
            }

            set
            {
                if ((value < 0) || (9 <= value))
                {
                    goto FINISH;
                }

                if (cboDayOfWeek.InvokeRequired == true)
                {
                    cboDayOfWeek.BeginInvoke(new Action<int>((index) => { cboDayOfWeek.SelectedIndex = index; }), value);
                }
                else
                {
                    cboDayOfWeek.SelectedIndex = value;
                }

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
                return cboHour.SelectedIndex;
            }

            set
            {
                if ((value < 0) || (24 <= value))
                {
                    goto FINISH;
                }

                if (cboHour.InvokeRequired == true)
                {
                    cboHour.BeginInvoke(new Action<int>((index) => { cboHour.SelectedIndex = index; }), value);
                }
                else
                {
                    cboHour.SelectedIndex = value;
                }

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
                return cboMinute.SelectedIndex;
            }

            set
            {
                if ((value < 0) || (60 <= value))
                {
                    goto FINISH;
                }

                if (cboMinute.InvokeRequired == true)
                {
                    cboMinute.BeginInvoke(new Action<int>((index) => { cboMinute.SelectedIndex = index; }), value);
                }
                else
                {
                    cboMinute.SelectedIndex = value;
                }

            FINISH:
                return;
            }
        }

        /// <summary>
        /// ロードイベント
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">e</param>
        private void dateTimeInputControl_Load(object sender, EventArgs e)
        {
            for (int hour = 0; hour <= 23; hour++)
            {
                cboHour.Items.Add(hour);
            }

            for (int minute = 0; minute <= 59; minute++)
            {
                cboMinute.Items.Add(minute);
            }
        }
    }
}
