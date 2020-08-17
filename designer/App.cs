using System;
using Eto.Forms;
using Eto.Drawing;
using System.Threading;

namespace designer {
    public static class App {
        public static Thread Thread { get; private set; }
        public static Application Application { get; private set; }
        public static Form Form { get; private set; }
        public static bool isRunning { get; private set; }

        public static void Init() {
            
        }
        public static void Run(bool force = false) {
            if (!isRunning)
            {
                Thread = new Thread(() =>
                {
                    isRunning = true;
                    Application = new Application();
                    Form = new Form();
                    App.Form.WindowStyle = WindowStyle.None;
                    Application.Run(Form);
                });
                Thread.Start();
                while (Thread.IsAlive)
                {
                    Thread.Sleep(1);
                }
                Thread.Join();
                isRunning = false;
            }
        }
        public static void Close() {
          
        }
    }
}