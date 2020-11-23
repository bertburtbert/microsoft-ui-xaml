// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using MUXControlsTestApp.Utilities;

using Windows.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls;
using Common;
using Windows.UI.Xaml.Markup;
using System.Collections.Generic;
using Windows.UI.Xaml.Media;
using System.Linq;

#if USING_TAEF
using WEX.TestExecution;
using WEX.TestExecution.Markup;
using WEX.Logging.Interop;
#else
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
#endif

namespace Windows.UI.Xaml.Tests.MUXControls.ApiTests
{
    [TestClass]
    public class AnimatedIconTests : ApiTestBase
    {
        [TestMethod]
        public void SettingStateOnParentPropgatesToChildAnimatedIcon()
        {
            AnimatedIcon animatedIcon = null;
            Grid parentGrid = null;
            RunOnUIThread.Execute(() =>
            {
                animatedIcon = new AnimatedIcon();
                parentGrid = new Grid();

                Content = parentGrid;
                Content.UpdateLayout();
            });

            IdleSynchronizer.Wait();

            RunOnUIThread.Execute(() =>
            {
                //var radioButton1 = radioButtons.ContainerFromIndex(0) as RadioButton;
                //var radioButton2 = radioButtons2.ContainerFromIndex(0) as RadioButton;
                //Verify.IsNotNull(radioButton1, "Our custom ItemTemplate should have been wrapped in a RadioButton.");
                //Verify.IsNotNull(radioButton2, "Our custom ItemTemplate should have been wrapped in a RadioButton.");

                //bool testCondition = !(radioButton1.Foreground is SolidColorBrush brush && brush.Color == Colors.Blue);
                //Verify.IsTrue(testCondition, "Default foreground color of the RadioButton should not have been [blue].");

                //testCondition = radioButton2.Foreground is SolidColorBrush brush2 && brush2.Color == Colors.Blue;
                //Verify.IsTrue(testCondition, "The foreground color of the RadioButton should have been [blue].");
            });
        }
    }
}
