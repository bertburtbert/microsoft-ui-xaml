#include "pch.h"
#include "common.h"
#include "HamburgerAnimatedIconSource.h"
#include "HamburgerWrapper.h"

HamburgerAnimatedIconSource::HamburgerAnimatedIconSource()
{
    this->Source(winrt::make<HamburgerWrapper>());
}

