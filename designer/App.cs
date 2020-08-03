using Avalonia;
using System;

namespace designer
{
    sealed class MainApp : Application {
        
    }

    public static class App
    {
        private static AppBuilder m_builder;

        public static bool isInitialized { get; private set; }
        public static void Init(string[] args) {
            m_builder = AppBuilder.Configure<MainApp>().UsePlatformDetect();
            m_builder.StartWithClassicDesktopLifetime(args);
            isInitialized = true;
        }

        [STAThread]
        private static int Main(string[] args) {
            return 0;
        }
    }
}
