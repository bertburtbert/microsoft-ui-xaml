#pragma once

#include "AnimatedIcon.h"

#include "AnimatedIconTestHooks.g.h"

class AnimatedIconTestHooks :
    public winrt::implementation::AnimatedIconTestHooksT<AnimatedIconTestHooks>
{
public:
    static com_ptr<AnimatedIconTestHooks> GetGlobalTestHooks()
    {
        return s_testHooks;
    }

    static com_ptr<AnimatedIconTestHooks> EnsureGlobalTestHooks();

    static void SetAnimationQueueBehavior(const winrt::AnimatedIcon& animatedIcon, winrt::AnimatedIconAnimationQueueBehavior);

private:
    static com_ptr<AnimatedIconTestHooks> s_testHooks;
};
