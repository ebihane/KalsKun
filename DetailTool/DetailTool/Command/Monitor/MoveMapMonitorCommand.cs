using DetailTool.Components.Map;
using DetailTool.Components.Setting;

namespace DetailTool.Command.Monitor
{
    public class MoveMapMonitorCommand : CommandBase
    {
        public MoveMapMonitorCommand()
        {
        }

        protected override EventInfo createEvent(out int expectedSize)
        {
            EventInfo retVal = new EventInfo();

            retVal.Code = 2003;

            SettingData setting = SettingData.GetInstance();
            expectedSize = setting.MapLength * setting.MapWidth;

            return retVal;
        }

        protected override void analyzeCore(byte[] data)
        {
            MoveMap moveMap = MoveMap.GetInstance();
            moveMap.Update(data);
        }
    }
}
