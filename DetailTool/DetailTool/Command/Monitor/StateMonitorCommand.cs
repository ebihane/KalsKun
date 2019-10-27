using DetailTool.Components;

namespace DetailTool.Command.Monitor
{
    public class StateMonitorCommand : CommandBase
    {
        public StateMonitorCommand()
        {
        }

        protected override EventInfo createEvent(out int expectedSize)
        {
            EventInfo retVal = new EventInfo();
            MonitorData monitor = MonitorData.GetInstance();

            retVal.Code = 2001;
            expectedSize = monitor.GetSize();

            return retVal;
        }

        protected override void analyzeCore(byte[] data)
        {
            MonitorData monitor = MonitorData.GetInstance();
            monitor.Analyze(data);
        }
    }
}
