#pragma once

#include "pch.h"
#include "common.h"
#include "HamburgerAnimatedIconSource.g.h"
#include "AnimatedIconSource.h"
#include "HamburgerAnimatedIconSource.properties.h"

class HamburgerAnimatedIconSource : public winrt::implementation::HamburgerAnimatedIconSourceT<HamburgerAnimatedIconSource, AnimatedIconSource>,
    public HamburgerAnimatedIconSourceProperties
{
public:
    HamburgerAnimatedIconSource();
};
