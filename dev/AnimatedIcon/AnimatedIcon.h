// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"
#include "common.h"

#include "AnimatedIcon.g.h"
#include "AnimatedIcon.properties.h"

class AnimatedIcon :
    public ReferenceTracker<AnimatedIcon, DeriveFromBitmapIconHelper_base, winrt::AnimatedIcon>,
    public AnimatedIconProperties
{

public:
    AnimatedIcon();
    ~AnimatedIcon() {}

    // IFrameworkElement
    void OnApplyTemplate();
    void OnLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args);

    // FrameworkElement overrides
    winrt::Size MeasureOverride(winrt::Size const& availableSize);
    winrt::Size ArrangeOverride(winrt::Size const& finalSize);

    void OnSourcePropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);
    static void OnAnimatedIconStatePropertyChanged(
        const winrt::DependencyObject& sender,
        const winrt::DependencyPropertyChangedEventArgs& args);
    void OnStatePropertyChanged();

    static winrt::DependencyProperty AnimatedIconStateProperty() { return s_StateProperty; }

    // TestHooks
    void SetAnimationQueueBehavior(winrt::AnimatedIconAnimationQueueBehavior behavior);
private:
    void TransitionStates(const winrt::hstring& fromState, const winrt::hstring& toState);
    void OnAnimationCompleted(winrt::IInspectable const&, winrt::CompositionBatchCompletedEventArgs const&);
    tracker_ref<winrt::IAnimatedVisual> m_animatedVisual{ this };
    tracker_ref<winrt::Grid> m_rootGrid{ this };

    winrt::hstring m_currentState{ L"Normal" };
    winrt::hstring m_previousState{ L"" };
    winrt::hstring m_queuedState{ L"" };
    bool m_isPlaying{ false };


    winrt::Composition::CompositionPropertySet m_progressPropertySet{ nullptr };
    winrt::Composition::CompositionScopedBatch m_batch{ nullptr };
    winrt::event_token m_batchCompletedToken{ 0 };

    winrt::AnimatedIconAnimationQueueBehavior m_queueBehavior{ winrt::AnimatedIconAnimationQueueBehavior::Cut };
};
