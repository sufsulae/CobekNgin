using Eto.Drawing;
using Eto.Forms;
using System.Threading;

namespace Designer
{
    public static class App {
        public static Thread Thread { get; private set; }
        public static Application Application { get; private set; }
        public static Form Form { get; private set; }
        public static bool isRunning { get; private set; }

        public static void Run() {
            if (!isRunning)
            {
                Thread = new Thread(() =>
                {
                    isRunning = true;
                    App.Application = new Application();
                    App.Form = new Form();
                    App.Form.Size = new Size(200, 200);
                    App.Form.WindowStyle = WindowStyle.Default;
                    App.Application.Run(App.Form);
                });
                Thread.Start();
            }
        }
        public static void Stop() {
            if (isRunning) {
                Thread.Join();
            }
        }
    }
}