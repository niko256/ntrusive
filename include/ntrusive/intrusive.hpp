#pragma once

#include "base_node.hpp"
#include "iterator.hpp"
#include "list.hpp"
#include "node.hpp"
#include "policies.hpp"


/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

template <typename Tag = void>
using NoLinkHook = IntrusiveListNode<Tag, NoTrackPolicy, NoOpHandler>;

template <typename Tag = void>
using AutoUnlinkHook = IntrusiveListNode<Tag, TrackingPolicy, AutoUnlinkHandler>;

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
