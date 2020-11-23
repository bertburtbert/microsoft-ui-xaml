using AnimatedVisuals;
using Microsoft.UI.Xaml.Controls;
using System;
using System.Collections.Generic;
using Windows.UI;
using Windows.UI.Composition;
using Windows.UI.Xaml;
//using Controls_09_Hamburger = AnimatedVisuals.Controls_09_Hamburger;
using Controls_09_Hamburger = Microsoft.UI.Xaml.Controls.Controls_09_Hamburger;

namespace MUXControlsTestApp
{
    class MockIRichAnimatedIconSource : IRichAnimatedVisualSource, IRichAnimatedVisualSource2
    {
        Dictionary<string, double> markers = new Dictionary<string, double>();

        IRichAnimatedVisualSource visual = new Controls_09_Hamburger();
        public MockIRichAnimatedIconSource()
        {
            markers.Add("NormalToPointerOverStart", 0.0);
            markers.Add("NormalToPointerOverEnd", 0.316666663);
            markers.Add("NormalToPressedStart", 0.0);
            markers.Add("NormalToPressedEnd", 0.816666663);
            markers.Add("PointerOverToNormalStart", 0.316666663);
            markers.Add("PointerOverToNormalEnd", 0.0);
            markers.Add("PointerOverToPressedStart", 0.333333343);
            markers.Add("PointerOverToPressedEnd", 0.816666663);
            markers.Add("PressedToNormalStart", 0.833333313);
            markers.Add("PressedToNormalEnd", 0.983333349);
            markers.Add("PressedToPointerOverStart", 0.816666663);
            markers.Add("PressedToPointerOverEnd", 0.333333343);
            markers.Add("Disabled", 0.5);
        }
        public IAnimatedVisual TryCreateAnimatedVisual(Compositor compositor, out object diagnostics)
        {
            diagnostics = null;
            return visual.TryCreateAnimatedVisual(Window.Current.Compositor, out diagnostics);
        }

        public void SetColorProperty(string propertyName, Color value)
        {
            visual.SetColorProperty(propertyName, value);
        }

        public IReadOnlyDictionary<string, double> Markers => markers;

        public double GetPositionFromMarker(string marker)
        {
            try
            {
                return Math.Min(Math.Max(Double.Parse(marker), 0.0), 1.0);
            }
            catch (FormatException)
            {
                return -1;
            }
        }
    }
}
