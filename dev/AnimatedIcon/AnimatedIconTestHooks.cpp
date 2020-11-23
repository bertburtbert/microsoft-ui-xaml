#include "pch.h"
#include "common.h"
#include "AnimatedIconTestHooks.h"

#include "AnimatedIconTestHooks.properties.cpp"

com_ptr<AnimatedIconTestHooks> AnimatedIconTestHooks::s_testHooks{};

com_ptr<AnimatedIconTestHooks> AnimatedIconTestHooks::EnsureGlobalTestHooks()
{
    static bool s_initialized = []() {
        s_testHooks = winrt::make_self<AnimatedIconTestHooks>();
        return true;
    }();
    return s_testHooks;
}

void AnimatedIconTestHooks::SetAnimationQueueBehavior(const winrt::AnimatedIcon& animatedIcon, winrt::AnimatedIconAnimationQueueBehavior behavior)
{
    if (animatedIcon)
    {
        winrt::get_self<AnimatedIcon>(animatedIcon)->SetAnimationQueueBehavior(behavior);
    }
}

void AnimatedIconTestHooks::SetDurationMultiplier(const winrt::AnimatedIcon& animatedIcon, double multiplier)
{
    if (animatedIcon)
    {
        winrt::get_self<AnimatedIcon>(animatedIcon)->SetDurationMultiplier(multiplier);
    }
}
