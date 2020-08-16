using System;
using Eto.Forms;
using Eto.Drawing;
using System.Threading;

namespace designer {
    public static class App {
        //private static Application m_app;
        //public static void Init() {
        //    if (m_app == null) {
        //        m_app = new Application();
        //        m_app.Run(new Form() { Size = new Size(200, 200) });
        //    }
        //}
        static void Main() {
            var newThread = new Thread(() =>{
                new Application().Run(new Form() { Title = "Just Another Frame" });
            });
            newThread.Start();
            while (newThread.IsAlive) {
                Thread.Sleep(1);
            }
            newThread.Join();
        }
    }
}