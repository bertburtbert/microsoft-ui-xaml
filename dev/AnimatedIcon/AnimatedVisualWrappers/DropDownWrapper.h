#pragma once

#include "pch.h"
#include "common.h"
#include <AnimatedIcon.h>
#include "DropDownWrapper.g.h"


class DropDownWrapper :
    public winrt::implementation::DropDownWrapperT<DropDownWrapper>
{
public:
    DropDownWrapper();

    winrt::IAnimatedVisual TryCreateAnimatedVisual(winrt::Compositor compositor, winrt::IInspectable diagnostics);
    void SetColorProperty(winrt::hstring propertyName, winrt::Color value);

    winrt::IMapView<winrt::hstring, double> Markers()
    {
        return m_markers.GetView();
    }

private:
    winrt::IMap<winrt::hstring, double> m_markers;
    winrt::IAnimatedVisualSource m_visual;
};
