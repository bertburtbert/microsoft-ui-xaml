﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "ResourceAccessor.h"
#include "NavigationViewItemAutomationPeer.h"
#include "NavigationView.h"
#include "NavigationViewItemBase.h"
#include "SharedHelpers.h"


#include "NavigationViewItemAutomationPeer.properties.cpp"

NavigationViewItemAutomationPeer::NavigationViewItemAutomationPeer(winrt::NavigationViewItem const& owner) :
    ReferenceTracker(owner)
{
}

//IAutomationPeerOverrides

winrt::hstring NavigationViewItemAutomationPeer::GetNameCore()
{
    winrt::hstring returnHString = __super::GetNameCore();

    // If a name hasn't been provided by AutomationProperties.Name in markup:
    if (returnHString.empty())
    {
        if (auto lvi = Owner().try_as<winrt::NavigationViewItem>())
        {
            returnHString = SharedHelpers::TryGetStringRepresentationFromObject(lvi.Content());
        }
    }

    if (returnHString.empty())
    {
        // NB: It'll be up to the app to determine the automation label for
        // when they're using a PlaceholderValue vs. Value.

        returnHString = ResourceAccessor::GetLocalizedStringResource(SR_NavigationViewItemDefaultControlName);
    }

    return returnHString;
}

winrt::IInspectable NavigationViewItemAutomationPeer::GetPatternCore(winrt::PatternInterface const& pattern)
{
    if (pattern == winrt::PatternInterface::SelectionItem ||
        pattern == winrt::PatternInterface::Invoke)
    {
        return *this;
    }

    return __super::GetPatternCore(pattern);
}


winrt::AutomationControlType NavigationViewItemAutomationPeer::GetAutomationControlTypeCore()
{
    // To be compliant with MAS 4.1.2, in DisplayMode 'Top',
    //  a NavigationViewItem should report itsself as TabItem
    if (IsOnTopNavigation())
    {
        return winrt::AutomationControlType::TabItem;
    }
    else
    {
        return winrt::AutomationControlType::ListItem;
    }
}


int32_t NavigationViewItemAutomationPeer::GetPositionInSetCore()
{
    int32_t positionInSet = 0;

    if (IsSettingsItem())
    {
        return 1;
    }

    if (IsOnTopNavigation())
    {
        if (auto navigationView = GetParentNavigationView())
        {
            auto topDataProvider = winrt::get_self<NavigationView>(navigationView)->GetTopDataProvider();
            positionInSet = GetPositionOrSetCountInTopNavHelper(IsOnTopNavigationOverflow() 
                ? topDataProvider.GetOverflowItems() : topDataProvider.GetPrimaryItems(), AutomationOutput::Position);
        }
    }
    else
    {
        positionInSet = GetPositionOrSetCountInLeftNavHelper(AutomationOutput::Position);
    }

    return positionInSet;
}

int32_t NavigationViewItemAutomationPeer::GetSizeOfSetCore()
{
    int32_t sizeOfSet = 0;

    if (IsSettingsItem())
    {
        return 1;
    }

    if (IsOnTopNavigation())
    {
        if (auto navview = GetParentNavigationView())
        {
            auto topNavDataProvider = winrt::get_self<NavigationView>(navview)->GetTopDataProvider();
            sizeOfSet = GetPositionOrSetCountInTopNavHelper(IsOnTopNavigationOverflow() 
                ? topNavDataProvider.GetOverflowItems() : topNavDataProvider.GetPrimaryItems(), AutomationOutput::Size);

        }
    }
    else
    {
        sizeOfSet = GetPositionOrSetCountInLeftNavHelper(AutomationOutput::Size);
    }

    return sizeOfSet;
}

void NavigationViewItemAutomationPeer::Invoke()
{
    if (auto const navView = GetParentNavigationView())
    {
        if (auto const navigationViewItem = Owner().try_as<winrt::NavigationViewItem>())
        {
            if (navigationViewItem == navView.SettingsItem())
            {
                winrt::get_self<NavigationView>(navView)->OnSettingsInvoked();
            }
            else
            {
                winrt::get_self<NavigationView>(navView)->OnNavigationViewItemInvoked(navigationViewItem);
            }
        }
    }
}

winrt::NavigationView NavigationViewItemAutomationPeer::GetParentNavigationView()
{
    winrt::NavigationView navigationView{ nullptr };

    winrt::NavigationViewItemBase navigationViewItem = Owner().try_as<winrt::NavigationViewItemBase>();
    if (navigationViewItem)
    {
        navigationView = winrt::get_self<NavigationViewItemBase>(navigationViewItem)->GetNavigationView();
    }
    return navigationView;
}

int32_t NavigationViewItemAutomationPeer::GetNavigationViewItemCountInPrimaryList()
{
    int32_t count = 0;
    if (auto navigationView = GetParentNavigationView())
    {
        count = winrt::get_self<NavigationView>(navigationView)->GetNavigationViewItemCountInPrimaryList();
    }
    return count;
}

int32_t NavigationViewItemAutomationPeer::GetNavigationViewItemCountInTopNav()
{
    int32_t count = 0;
    if (auto navigationView = GetParentNavigationView())
    {
        count = winrt::get_self<NavigationView>(navigationView)->GetNavigationViewItemCountInTopNav();
    }
    return count;
}

bool NavigationViewItemAutomationPeer::IsSettingsItem()
{
    if (auto navView = GetParentNavigationView())
    {
        winrt::NavigationViewItem item = Owner().try_as<winrt::NavigationViewItem>();
        auto settingsItem = navView.SettingsItem();
        if (item && settingsItem && (item == settingsItem || item.Content() == settingsItem))
        {
            return true;
        }
    }
    return false;
}

bool NavigationViewItemAutomationPeer::IsOnTopNavigation()
{
    return GetNavigationViewRepeaterPosition() != NavigationViewRepeaterPosition::LeftNav;
}

bool NavigationViewItemAutomationPeer::IsOnTopNavigationOverflow()
{
    return GetNavigationViewRepeaterPosition() == NavigationViewRepeaterPosition::TopOverflow;
}

NavigationViewRepeaterPosition NavigationViewItemAutomationPeer::GetNavigationViewRepeaterPosition()
{
    if (winrt::NavigationViewItemBase navigationViewItem = Owner().try_as<winrt::NavigationViewItemBase>())
    {
        return winrt::get_self<NavigationViewItemBase>(navigationViewItem)->Position();
    }
    return NavigationViewRepeaterPosition::LeftNav;
}

// Get either the position or the size of the set for this particular item in the case of left nav. 
// We go through all the items and then we determine if the listviewitem from the left listview can be a navigation view item header
// or a navigation view item. If it's the former, we just reset the count. If it's the latter, we increment the counter.
// In case of calculating the position, if this is the NavigationViewItemAutomationPeer we're iterating through we break the loop.
int32_t NavigationViewItemAutomationPeer::GetPositionOrSetCountInLeftNavHelper(AutomationOutput automationOutput)
{
    int returnValue = 0;
    
    if (auto const navview = GetParentNavigationView())
    {
        if (auto const repeater = winrt::get_self<NavigationView>(navview)->LeftNavRepeater())
        {
            if (auto const parent = Navigate(winrt::AutomationNavigationDirection::Parent).try_as<winrt::AutomationPeer>())
            {
                if (auto const children = parent.GetChildren())
                {
                    int index = 0;
                    bool itemFound = false;

                    for (auto const& child : children)
                    {
                        if (auto dependencyObject = repeater.TryGetElement(index))
                        {
                            if (dependencyObject.try_as<winrt::NavigationViewItemHeader>())
                            {
                                if (automationOutput == AutomationOutput::Size && itemFound)
                                {
                                    break;
                                }
                                else
                                {
                                    returnValue = 0;
                                }
                            }
                            else if (auto navviewItem = dependencyObject.try_as<winrt::NavigationViewItem>())
                            {
                                if (navviewItem.Visibility() == winrt::Visibility::Visible)
                                {
                                    returnValue++;

                                    if (child.try_as<winrt::NavigationViewItemAutomationPeer>() == static_cast<winrt::NavigationViewItemAutomationPeer>(*this))
                                    {
                                        if (automationOutput == AutomationOutput::Position)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            itemFound = true;
                                        }
                                    }
                                }
                            }
                        }
                        index++;
                    }
                }
            }
        }
    }

    return returnValue;
}

// Get either the position or the size of the set for this particular item in the case of top nav (primary/overflow items). 
// Basically, we do the same here as GetPositionOrSetCountInLeftNavHelper without dealing with the listview directly, because 
// TopDataProvider provcides two methods: GetOverflowItems() and GetPrimaryItems(), so we can break the loop (in case of position) by 
// comparing the value of the FrameworkElementAutomationPeer we can get from the item we're iterating through to this object.
int32_t NavigationViewItemAutomationPeer::GetPositionOrSetCountInTopNavHelper(winrt::IVector<winrt::IInspectable> navigationViewElements, AutomationOutput automationOutput)
{
    int32_t returnValue = 0;
    
    if (auto const navview = GetParentNavigationView())
    {
        bool itemFound = false;

        for (auto const& child : navigationViewElements)
        {
            if (auto const childAsNavViewItem = navview.ContainerFromMenuItem(child))
            {
                if (child.try_as<winrt::NavigationViewItemHeader>())
                {
                    if (automationOutput == AutomationOutput::Size && itemFound)
                    {
                        break;
                    }
                    else
                    {
                        returnValue = 0;
                    }
                }
                else if (auto const navviewitem = childAsNavViewItem.try_as<winrt::NavigationViewItem>())
                {
                    if (navviewitem.Visibility() == winrt::Visibility::Visible)
                    {
                        returnValue++;

                        if (winrt::FrameworkElementAutomationPeer::FromElement(navviewitem) == static_cast<winrt::NavigationViewItemAutomationPeer>(*this))
                        {
                            if (automationOutput == AutomationOutput::Position)
                            {
                                break;
                            }
                            else
                            {
                                itemFound = true;
                            }
                        }
                    }
                }

            }
        }
    }

    return returnValue;
}

bool NavigationViewItemAutomationPeer::IsSelected()
{
    if (auto const nvi = Owner().try_as<winrt::NavigationViewItem>())
    {
        return nvi.IsSelected();
    }
    return false;
}

winrt::IRawElementProviderSimple NavigationViewItemAutomationPeer::SelectionContainer()
{
    if (auto const navview = GetParentNavigationView())
    {
        if (auto const peer = winrt::FrameworkElementAutomationPeer::CreatePeerForElement(navview))
        {
            return ProviderFromPeer(peer);
        }
    }

    return nullptr;
}

void NavigationViewItemAutomationPeer::AddToSelection()
{
    ChangeSelection(true);
}

void NavigationViewItemAutomationPeer::Select()
{
    ChangeSelection(true);
}

void NavigationViewItemAutomationPeer::RemoveFromSelection()
{
    ChangeSelection(false);
}

void NavigationViewItemAutomationPeer::ChangeSelection(bool isSelected)
{
    if (auto nvi = Owner().try_as<winrt::NavigationViewItem>())
    {
        nvi.IsSelected(isSelected);
    }
}