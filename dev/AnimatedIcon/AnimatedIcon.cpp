// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "AnimatedIcon.h"
#include "RuntimeProfiler.h"
#include "ResourceAccessor.h"
#include <AnimatedVisuals\ProgressRingDeterminate.h>
using namespace AnimatedVisuals;

AnimatedIcon::AnimatedIcon()
{
    __RP_Marker_ClassById(RuntimeProfiler::ProfId_AnimatedIcon);
    m_progressPropertySet = winrt::Window::Current().Compositor().CreatePropertySet();
    m_progressPropertySet.InsertScalar(L"Progress", 0);
}

void AnimatedIcon::OnApplyTemplate()
{
    Loaded({ this, &AnimatedIcon::OnLoaded });
}

void AnimatedIcon::OnLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args)
{
    winrt::IInspectable diagnostics{};
    auto const grid = winrt::VisualTreeHelper::GetChild(*this, 0).as<winrt::Grid>();
    m_rootGrid.set(grid);

    if (auto const visual = m_animatedVisual.get())
    {
        winrt::ElementCompositionPreview::SetElementChildVisual(grid, visual.RootVisual());
    }
}

winrt::Size AnimatedIcon::MeasureOverride(winrt::Size const& availableSize)
{
    if (auto const visual = m_animatedVisual.get())
    {
        auto const visualSize = visual.Size();
        if (visualSize != winrt::float2::zero())
        {
            const auto widthScale = availableSize.Width == std::numeric_limits<double>::infinity() ? std::numeric_limits<float>::infinity() : availableSize.Width / visualSize.x;
            const auto heightScale = availableSize.Height == std::numeric_limits<double>::infinity() ? std::numeric_limits<float>::infinity() : availableSize.Height / visualSize.y;
            if (widthScale == std::numeric_limits<double>::infinity() && heightScale == std::numeric_limits<double>::infinity())
            {
                return winrt::Size{ visualSize.x, visualSize.y };
            }
            else if (widthScale == std::numeric_limits<double>::infinity())
            {
                return winrt::Size{ visualSize.x * heightScale, availableSize.Height };
            }
            else if (heightScale == std::numeric_limits<double>::infinity())
            {
                return winrt::Size{ availableSize.Width, visualSize.y * widthScale };
            }
            else
            {
                return (heightScale > widthScale)
                    ? winrt::Size{ availableSize.Width, visualSize.y * widthScale }
                : winrt::Size{ visualSize.x * heightScale, availableSize.Height };
            }
        }
    }
    return { 0, 0 };
}

winrt::Size AnimatedIcon::ArrangeOverride(winrt::Size const& finalSize)
{
    winrt::float2 scale;
    winrt::float2 arrangedSize;

    if (auto const visual = m_animatedVisual.get())
    {
        auto const visualSize = visual.Size();
        scale = static_cast<winrt::float2>(finalSize) / visual.Size();
        if (scale.x < scale.y)
        {
            scale.y = scale.x;
        }
        else
        {
            scale.x = scale.y;
        }

        arrangedSize = {
            std::min(finalSize.Width / scale.x, visualSize.x),
            std::min(finalSize.Height / scale.y, visualSize.y)
        };
        const auto offset = (finalSize - (visualSize * scale)) / 2;
        const auto z = 0.0F;
        const auto rootVisual = visual.RootVisual();
        rootVisual.Offset({ offset, z });
        rootVisual.Size(arrangedSize);
        rootVisual.Scale({ scale, z });
    }

    return finalSize;
}

void AnimatedIcon::OnAnimatedIconStatePropertyChanged(
    const winrt::DependencyObject& sender,
    const winrt::DependencyPropertyChangedEventArgs& args)
{
    if (auto const senderAsAnimatedIcon = sender.try_as<AnimatedIcon>())
    {
        senderAsAnimatedIcon->OnStatePropertyChanged();
    }
    else if (winrt::VisualTreeHelper::GetChildrenCount(sender) > 0)
    {
        if (auto const childAsAnimatedIcon = winrt::VisualTreeHelper::GetChild(sender, 0).try_as<winrt::AnimatedIcon>())
        {
            childAsAnimatedIcon.SetValue(AnimatedIconProperties::s_StateProperty, args.NewValue());
        }
    }
}

void AnimatedIcon::OnStatePropertyChanged()
{
    auto const toState = ValueHelper<winrt::hstring>::CastOrUnbox(this->GetValue(AnimatedIconStateProperty()));
    switch (m_queueBehavior)
    {
    case winrt::AnimatedIconAnimationQueueBehavior::Cut:
        TransitionStates(m_currentState, toState);
        m_previousState = m_currentState;
        m_currentState = toState;
        m_queuedState = L"";
        break;
    case winrt::AnimatedIconAnimationQueueBehavior::QueueOne:
        if (m_isPlaying)
        {
            m_queuedState = toState;
        }
        else
        {
            TransitionStates(m_currentState, toState);
            m_previousState = m_currentState;
            m_currentState = toState;
            m_queuedState = L"";
        }
        break;
    case winrt::AnimatedIconAnimationQueueBehavior::SpeedUpQueueOne:
        if (m_isPlaying)
        {
            m_queuedState = toState;
            //Cancel the previous animation completed handler, before we cancel that animation by starting a new one.
            if (m_batch)
            {
                m_batch.Completed(m_batchCompletedToken);
                m_batchCompletedToken = { 0 };
            }
            auto const markers = Source().Markers();
            winrt::hstring transitionEndName = m_previousState + L"To" + m_currentState + L"_End";
            auto const hasEndMarker = markers.HasKey(transitionEndName);
            if (hasEndMarker)
            {
                PlaySegment(NAN, static_cast<float>(markers.Lookup(transitionEndName)), 7.0f);
            }
        }
        else
        {
            TransitionStates(m_currentState, toState);
            m_previousState = m_currentState;
            m_currentState = toState;
            m_queuedState = L"";
        }
        break;
    }
}

void AnimatedIcon::TransitionStates(const winrt::hstring& fromState, const winrt::hstring& toState)
{
    auto const markers = Source().Markers();

    winrt::hstring transitionStartName = fromState + L"To" + toState + L"_Start";
    winrt::hstring transitionEndName = fromState + L"To" + toState + L"_End";
    winrt::hstring transitionName = fromState + L"To" + toState;

    auto const hasStartMarker = markers.HasKey(transitionStartName);
    auto const hasEndMarker = markers.HasKey(transitionEndName);
    if (hasStartMarker && hasEndMarker)
    {
        auto const fromProgress = static_cast<float>(markers.Lookup(transitionStartName));
        auto const toProgress = static_cast<float>(markers.Lookup(transitionEndName));
        PlaySegment(fromProgress, toProgress);
    }
    else if (markers.HasKey(transitionEndName))
    {
        auto const toProgress = static_cast<float>(markers.Lookup(transitionEndName));
        m_progressPropertySet.InsertScalar(L"Progress", toProgress);
    }
    else if (markers.HasKey(transitionName))
    {
        auto const toProgress = static_cast<float>(markers.Lookup(transitionName));
        m_progressPropertySet.InsertScalar(L"Progress", toProgress);
    }

    // Hack to support slider scenario.
    if (auto const rich2 = Source().try_as<winrt::IRichAnimatedVisualSource2>())
    {
        auto const progress = rich2.GetPositionFromMarker(toState);
        if (progress != -1.0)
        {
            m_progressPropertySet.InsertScalar(L"Progress", static_cast<float>(progress));
        }
    }
}

void AnimatedIcon::PlaySegment(float from, float to, float playbackMultiplier)
{
    auto const segmentLength = [from, to, previousSegmentLength = m_currentSegmentLength]()
    {
        if (std::isnan(from))
        {
            return previousSegmentLength;
        }
        return std::abs(to - from);
    }();

    auto compositor = m_progressPropertySet.Compositor();
    auto animation = compositor.CreateScalarKeyFrameAnimation();
    animation.Duration(std::chrono::duration_cast<winrt::TimeSpan>(m_animatedVisual.get().Duration() * segmentLength * (1.0 / playbackMultiplier)));
    auto linearEasing = compositor.CreateLinearEasingFunction();

    // Play from fromProgress.
    if (!std::isnan(from))
    {
        //m_progressPropertySet.InsertScalar(L"Progress", from);
        animation.InsertKeyFrame(0, from);
    }

    // Play to toProgress
    animation.InsertKeyFrame(1, to, linearEasing);

    animation.IterationBehavior(winrt::AnimationIterationBehavior::Count);
    animation.IterationCount(1);

    if (m_batch)
    {
        m_batch.Completed(m_batchCompletedToken);
        m_batchCompletedToken = { 0 };
    }
    m_batch = compositor.CreateScopedBatch(winrt::CompositionBatchTypes::Animation);
    m_batchCompletedToken = m_batch.Completed({ this, &AnimatedIcon::OnAnimationCompleted });

    m_isPlaying = true;
    m_currentSegmentLength = segmentLength;
    m_progressPropertySet.StartAnimation(L"Progress", animation);
    m_batch.End();
}

void AnimatedIcon::OnSourcePropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args)
{
    auto const visual = [this]()
    {
        if (auto const source = Source())
        {
            winrt::IInspectable diagnostics{};
            auto const visual = source.TryCreateAnimatedVisual(winrt::Window::Current().Compositor(), diagnostics);
            m_animatedVisual.set(visual);
            return visual.RootVisual();
        }
        else
        {
            m_animatedVisual.set(nullptr);
            return static_cast<winrt::Visual>(nullptr);
        }
    }();

    if (auto const rootGrid = m_rootGrid.get())
    {
        winrt::ElementCompositionPreview::SetElementChildVisual(rootGrid, visual);
    }

    visual.Properties().InsertScalar(L"Progress", 0.0F);

    // Tie the animated visual's Progress property to the player Progress with an ExpressionAnimation.
    auto compositor = visual.Compositor();
    auto progressAnimation = compositor.CreateExpressionAnimation(L"_.Progress");
    progressAnimation.SetReferenceParameter(L"_", m_progressPropertySet);
    visual.Properties().StartAnimation(L"Progress", progressAnimation);
}

void AnimatedIcon::OnAnimationCompleted(winrt::IInspectable const&, winrt::CompositionBatchCompletedEventArgs const&)
{
    if (m_batch)
    {
        m_batch.Completed(m_batchCompletedToken);
        m_batchCompletedToken = { 0 };
    }
    m_isPlaying = false;
    m_currentSegmentLength = 1.0f;
    switch (m_queueBehavior)
    {
    case winrt::AnimatedIconAnimationQueueBehavior::Cut:
        break;
    case winrt::AnimatedIconAnimationQueueBehavior::QueueOne:
        if (!m_queuedState.empty())
        {
            TransitionStates(m_currentState, m_queuedState);
            m_previousState = m_currentState;
            m_currentState = m_queuedState;
            m_queuedState = L"";
        }
        break;
    case winrt::AnimatedIconAnimationQueueBehavior::SpeedUpQueueOne:
        if (!m_queuedState.empty())
        {
            TransitionStates(m_currentState, m_queuedState);
            m_previousState = m_currentState;
            m_currentState = m_queuedState;
            m_queuedState = L"";
        }
        break;
    }
}

// Test hooks
void AnimatedIcon::SetAnimationQueueBehavior(winrt::AnimatedIconAnimationQueueBehavior behavior)
{
    m_queueBehavior = behavior;
}
