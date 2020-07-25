using System;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace editor
{
    public class ManagedWindow : Window { 
        public IntPtr windowPtr { get { return this.PlatformImpl.Handle.Handle; } }
        public Window parentWindow { get; private set; }
    }
}
