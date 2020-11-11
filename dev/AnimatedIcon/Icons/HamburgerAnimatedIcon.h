#pragma once

#include "pch.h"
#include "common.h"
#include "HamburgerAnimatedIcon.g.h"
#include "HamburgerAnimatedIcon.properties.h"
#include <AnimatedIcon.h>

class HamburgerAnimatedIcon : public ReferenceTracker<HamburgerAnimatedIcon, winrt::implementation::HamburgerAnimatedIconT, AnimatedIcon>,
    public HamburgerAnimatedIconProperties
{
public:
    using HamburgerAnimatedIconProperties::EnsureProperties;
    using HamburgerAnimatedIconProperties::ClearProperties;
    HamburgerAnimatedIcon();

private:
    winrt::IMap<winrt::hstring, double> m_markers;
    winrt::IAnimatedVisualSource m_visual;
};
