#include "pch.h"
#include "common.h"
#include "DropDownWrapper.h"
#include "AnimatedVisuals/AnimatedVisuals.Controls_02_UpDown_Dropdown.h"

namespace winrt::Microsoft::UI::Xaml::Controls
{
    CppWinRTActivatableClassWithBasicFactory(DropDownWrapper)
}

#include "DropDownWrapper.g.cpp"

DropDownWrapper::DropDownWrapper()
{
    m_visual = winrt::make<Controls_02_UpDown_Dropdown>();

    m_markers = winrt::single_threaded_map<winrt::hstring, double>();
    m_markers.Insert(L"NormalToPointerOverStart", 0.0);
    m_markers.Insert(L"NormalToPointerOverEnd", 0.316666663);
    m_markers.Insert(L"NormalToPressedStart", 0.0);
    m_markers.Insert(L"NormalToPressedEnd", 0.816666663);
    m_markers.Insert(L"PointerOverToNormalStart", 0.316666663);
    m_markers.Insert(L"PointerOverToNormalEnd", 0.0);
    m_markers.Insert(L"PointerOverToPressedStart", 0.333333343);
    m_markers.Insert(L"PointerOverToPressedEnd", 0.816666663);
    m_markers.Insert(L"PressedToNormalStart", 0.833333313);
    m_markers.Insert(L"PressedToNormalEnd", 0.983333349);
    m_markers.Insert(L"PressedToPointerOverStart", 0.816666663);
    m_markers.Insert(L"PressedToPointerOverEnd", 0.333333343);
    m_markers.Insert(L"Disabled", 0.0);
}

winrt::IAnimatedVisual DropDownWrapper::TryCreateAnimatedVisual(winrt::Compositor compositor, winrt::IInspectable diagnostics)
{
    diagnostics = nullptr;
    return m_visual.TryCreateAnimatedVisual(winrt::Window::Current().Compositor(), diagnostics);
}

void DropDownWrapper::SetColorProperty(winrt::hstring propertyName, winrt::Color value)
{
}

