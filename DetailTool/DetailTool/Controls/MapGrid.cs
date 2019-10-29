using System;
using System.Windows.Forms;
using DetailTool.Components;
using DetailTool.Components.Setting;
using DetailTool.Components.Map;

namespace DetailTool.Controls
{
    public partial class MapGrid : UserControl
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MapGrid()
        {
            InitializeComponent();
        }

        /// <summary>
        /// マップ表示作成
        /// </summary>
        public void CreateMap()
        {
            dgvMap.Rows.Clear();
            dgvMap.Columns.Clear();

            SettingData setting = SettingData.GetInstance();

            int colWidth = this.Width / setting.MapWidth;
            for (int x = 0; x < setting.MapWidth; x++)
            {
                dgvMap.Columns.Add(string.Format("{0}", x), string.Empty);
                dgvMap.Columns[x].Width = colWidth;
            }

            int rowHeight = this.Height / setting.MapLength;
            for (int y = 0; y < setting.MapLength; y++)
            {
                dgvMap.Rows.Add();
                dgvMap.Rows[y].Height = rowHeight;
            }
        }

        /// <summary>
        /// マップ表示更新
        /// </summary>
        public void UpdateMapColor()
        {
            if (dgvMap.InvokeRequired == true)
            {
                dgvMap.BeginInvoke(new Action<object>(
                (obj) =>
                {
                    MonitorData monitor = MonitorData.GetInstance();
                    MoveMap moveMap = MoveMap.GetInstance();

                    for (int y = 0; y < dgvMap.Rows.Count; y++)
                    {
                        for (int x = 0; x < dgvMap.Columns.Count; x++)
                        {
                            dgvMap[x, y].Style.BackColor = moveMap.GetColor(x, y);
                        }
                    }
                }), null);
            }
            else
            {
                MonitorData monitor = MonitorData.GetInstance();
                MoveMap moveMap = MoveMap.GetInstance();

                for (int y = 0; y < dgvMap.Rows.Count; y++)
                {
                    for (int x = 0; x < dgvMap.Columns.Count; x++)
                    {
                        dgvMap[x, y].Style.BackColor = moveMap.GetColor(x, y);
                    }
                }
            }
        }
    }
}
