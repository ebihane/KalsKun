using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Command.Monitor
{
    public class MoveMapMonitorCommand : CommandBase
    {
        public MoveMapMonitorCommand()
        {
        }

        protected override EventInfo createEvent(out int expectedSize)
        {
            expectedSize = 0;
            return null;
        }

        protected override void analyzeCore(byte[] data)
        {
        }
    }
}
