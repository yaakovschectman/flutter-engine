// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_PLATFORM_AX_PLATFORM_NODE_TEXTRANGEPROVIDER_WIN_H_
#define UI_ACCESSIBILITY_PLATFORM_AX_PLATFORM_NODE_TEXTRANGEPROVIDER_WIN_H_

#include <UIAutomationCore.h>

#include "ax/ax_node_position.h"

namespace ui {

class AXPlatformNodeTextRangeProviderWin {
 public:
  static ITextRangeProvider* CreateTextRangeProvider(AXNodePosition::AXPositionInstance start, AXNodePosition::AXPositionInstance end);
};

}

#endif  // UI_ACCESSIBILITY_PLATFORM_AX_PLATFORM_NODE_TEXTRANGEPROVIDER_WIN_H_
