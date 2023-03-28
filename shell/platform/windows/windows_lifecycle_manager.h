// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_WINDOWS_WINDOWS_LIFECYCLE_MANAGER_H_
#define FLUTTER_SHELL_PLATFORM_WINDOWS_WINDOWS_LIFECYCLE_MANAGER_H_

#include <Windows.h>

#include <cstdint>

namespace flutter {

class FlutterWindowsEngine;

/// A manager for lifecycle events of the top-level window.
///
/// Currently handles the following events:
/// WM_CLOSE
class WindowsLifecycleManager {
 public:
  WindowsLifecycleManager(FlutterWindowsEngine* engine);
  virtual ~WindowsLifecycleManager();

  // The window paramter optionally passes a handle to a window. When the
  // parameter is null, the call to this method is interpreted as a signal to
  // quit the application. When it is a valid window handle, the method call is
  // interpreted as a signal to destroy that window. When window is non-null,
  // exit-code is not used.
  virtual void Quit(UINT exit_code, HWND window) const;

  bool WindowProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l, LRESULT* result);

 private:
  bool IsLastWindowOfProcess();

  FlutterWindowsEngine* engine_;
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_WINDOWS_WINDOWS_LIFECYCLE_MANAGER_H_
