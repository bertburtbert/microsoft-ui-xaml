﻿#pragma once
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//       LottieGen version:
//           7.0.0-build.57+g2cc4f0c931
//       
//       Command:
//           LottieGen -Language Cppwinrt -InputFile Controls_10_Search.json
//       
//       Input file:
//           Controls_10_Search.json (14321 bytes created 18:13-08:00 Nov 19 2020)
//       
//       LottieGen source:
//           http://aka.ms/Lottie
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#include "common.h"
#include "Controls_10_Search.g.h"

// Name:        XAMLControl - 03 - Navigation - D - Search
// Frame rate:  60 fps
// Frame count: 180
// Duration:    3000.0 mS
// _________________________________________________________
// | Theme property |  Accessor  | Type  |  Default value  |
// |________________|____________|_______|_________________|
// | Foreground     | Foreground | Color | #FF000000 Black |
// ---------------------------------------------------------
class Controls_10_Search
    : public winrt::implementation::Controls_10_SearchT<Controls_10_Search>
{
    winrt::Windows::UI::Composition::CompositionPropertySet _themeProperties{ nullptr };
    winrt::Windows::UI::Color _themeForeground{ 0xFF, 0x00, 0x00, 0x00 };
    winrt::Windows::UI::Composition::CompositionPropertySet EnsureThemeProperties(winrt::Windows::UI::Composition::Compositor compositor);

    static winrt::Windows::Foundation::Numerics::float4 ColorAsVector4(winrt::Windows::UI::Color color);
public:
    // Animation duration: 3.000 seconds.
    static constexpr int64_t c_durationTicks{ 30000000L };

    // Theme property: Foreground.
    static inline const winrt::Windows::UI::Color c_themeForeground{ 0xFF, 0x00, 0x00, 0x00 };


    winrt::Windows::UI::Color Foreground();
    void Foreground(winrt::Windows::UI::Color value);

    winrt::Microsoft::UI::Xaml::Controls::IAnimatedVisual TryCreateAnimatedVisual(
        winrt::Windows::UI::Composition::Compositor const& compositor,
        winrt::Windows::Foundation::IInspectable& diagnostics);

    // Converts a frame number to the corresponding progress value.
    double FrameToProgress(double frameNumber);

    // Returns a map from marker names to corresponding progress values.
    winrt::Windows::Foundation::Collections::IMapView<hstring, double> Markers();

    // Sets the color property with the given name, or does nothing if no such property
    // exists.
    void SetColorProperty(hstring const& propertyName, winrt::Windows::UI::Color value);

    // Sets the scalar property with the given name, or does nothing if no such property
    // exists.
    void SetScalarProperty(hstring const& propertyName, double value);
};
