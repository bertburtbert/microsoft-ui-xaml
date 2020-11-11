using Microsoft.UI.Xaml.Controls;
using System;
using System.Collections.Generic;
using System.Text;

namespace MUXControlsTestApp
{
    class MockAnimatedIconSource : AnimatedIconSource
    {
        public MockAnimatedIconSource()
        {
            this.Source = new MockIRichAnimatedIconSource();
        }
    }
}
