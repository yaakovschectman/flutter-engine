// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_WINDOWS_ACCESSIBILITY_ROOT_NODE_H_
#define FLUTTER_SHELL_PLATFORM_WINDOWS_ACCESSIBILITY_ROOT_NODE_H_

#include <atlbase.h>
#include <atlcom.h>
#include <oleacc.h>

#include <memory>

#include "flutter/shell/platform/windows/accessibility_alert.h"
#include "flutter/third_party/iaccessible2/ia2_api_all.h"

namespace flutter {

// A parent node that wraps the window IAccessible node.
class __declspec(uuid("fedb8280-ea4f-47a9-98fe-5d1a557fe4b3"))
    AccessibilityRootNode : public CComObjectRootEx<CComMultiThreadModel>,
                            public IDispatchImpl<IAccessible2_4,
                                                 &IID_IAccessible2_4,
                                                 &LIBID_IAccessible2Lib>,
                            public IServiceProvider {
 public:
  static constexpr LONG kAlertChildId = 2;

  BEGIN_COM_MAP(AccessibilityRootNode)
  COM_INTERFACE_ENTRY2(IDispatch, IAccessible2_2)
  COM_INTERFACE_ENTRY2(IUnknown, IDispatchImpl)
  COM_INTERFACE_ENTRY(AccessibilityRootNode)
  COM_INTERFACE_ENTRY(IAccessible)
  COM_INTERFACE_ENTRY(IAccessible2)
  COM_INTERFACE_ENTRY(IAccessible2_2)
  COM_INTERFACE_ENTRY(IAccessible2_3)
  COM_INTERFACE_ENTRY(IAccessible2_4)
  //COM_INTERFACE_ENTRY(IDispatch)
  COM_INTERFACE_ENTRY(IServiceProvider)
  END_COM_MAP()

  //
  // IAccessible methods.
  //

  // Retrieves the child element or child object at a given point on the screen.
  IFACEMETHODIMP accHitTest(LONG screen_physical_pixel_x,
                            LONG screen_physical_pixel_y,
                            VARIANT* child) override;

  // Performs the object's default action.
  IFACEMETHODIMP accDoDefaultAction(VARIANT var_id) override;

  // Retrieves the specified object's current screen location.
  IFACEMETHODIMP accLocation(LONG* physical_pixel_left,
                             LONG* physical_pixel_top,
                             LONG* width,
                             LONG* height,
                             VARIANT var_id) override;

  // Traverses to another UI element and retrieves the object.
  IFACEMETHODIMP accNavigate(LONG nav_dir,
                             VARIANT start,
                             VARIANT* end) override;

  // Retrieves an IDispatch interface pointer for the specified child.
  IFACEMETHODIMP get_accChild(VARIANT var_child,
                              IDispatch** disp_child) override;

  // Retrieves the number of accessible children.
  IFACEMETHODIMP get_accChildCount(LONG* child_count) override;

  // Retrieves a string that describes the object's default action.
  IFACEMETHODIMP get_accDefaultAction(VARIANT var_id,
                                      BSTR* default_action) override;

  // Retrieves the tooltip description.
  IFACEMETHODIMP get_accDescription(VARIANT var_id, BSTR* desc) override;

  // Retrieves the object that has the keyboard focus.
  IFACEMETHODIMP get_accFocus(VARIANT* focus_child) override;

  // Retrieves the specified object's shortcut.
  IFACEMETHODIMP get_accKeyboardShortcut(VARIANT var_id,
                                         BSTR* access_key) override;

  // Retrieves the name of the specified object.
  IFACEMETHODIMP get_accName(VARIANT var_id, BSTR* name) override;

  // Retrieves the IDispatch interface of the object's parent.
  IFACEMETHODIMP get_accParent(IDispatch** disp_parent) override;

  // Retrieves information describing the role of the specified object.
  IFACEMETHODIMP get_accRole(VARIANT var_id, VARIANT* role) override;

  // Retrieves the current state of the specified object.
  IFACEMETHODIMP get_accState(VARIANT var_id, VARIANT* state) override;

  // Gets the help string for the specified object.
  IFACEMETHODIMP get_accHelp(VARIANT var_id, BSTR* help) override;

  // Retrieve or set the string value associated with the specified object.
  // Setting the value is not typically used by screen readers, but it's
  // used frequently by automation software.
  IFACEMETHODIMP get_accValue(VARIANT var_id, BSTR* value) override;
  IFACEMETHODIMP put_accValue(VARIANT var_id, BSTR new_value) override;

  // IAccessible methods not implemented.
  IFACEMETHODIMP get_accSelection(VARIANT* selected) override;
  IFACEMETHODIMP accSelect(LONG flags_sel, VARIANT var_id) override;
  IFACEMETHODIMP get_accHelpTopic(BSTR* help_file,
                                  VARIANT var_id,
                                  LONG* topic_id) override;
  IFACEMETHODIMP put_accName(VARIANT var_id, BSTR put_name) override;

  //
  // IAccessible2 methods.
  //

  IFACEMETHODIMP role(LONG* role) override;

  IFACEMETHODIMP get_states(AccessibleStates* states) override;

  IFACEMETHODIMP get_uniqueID(LONG* unique_id) override;

  IFACEMETHODIMP get_windowHandle(HWND* window_handle) override;

  IFACEMETHODIMP get_relationTargetsOfType(BSTR type,
                                           LONG max_targets,
                                           IUnknown*** targets,
                                           LONG* n_targets) override;

  IFACEMETHODIMP get_attributes(BSTR* attributes) override;

  IFACEMETHODIMP get_indexInParent(LONG* index_in_parent) override;

  IFACEMETHODIMP get_nRelations(LONG* n_relations) override;

  IFACEMETHODIMP get_relation(LONG relation_index,
                              IAccessibleRelation** relation) override;

  IFACEMETHODIMP get_relations(LONG max_relations,
                               IAccessibleRelation** relations,
                               LONG* n_relations) override;

  IFACEMETHODIMP get_attribute(BSTR name, VARIANT* attribute) override;
  IFACEMETHODIMP get_extendedRole(BSTR* extended_role) override;
  IFACEMETHODIMP scrollTo(enum IA2ScrollType scroll_type) override;
  IFACEMETHODIMP scrollToPoint(enum IA2CoordinateType coordinate_type,
                               LONG x,
                               LONG y) override;
  IFACEMETHODIMP get_groupPosition(LONG* group_level,
                                   LONG* similar_items_in_group,
                                   LONG* position_in_group) override;
  IFACEMETHODIMP get_localizedExtendedRole(
      BSTR* localized_extended_role) override;
  IFACEMETHODIMP get_nExtendedStates(LONG* n_extended_states) override;
  IFACEMETHODIMP get_extendedStates(LONG max_extended_states,
                                    BSTR** extended_states,
                                    LONG* n_extended_states) override;
  IFACEMETHODIMP get_localizedExtendedStates(
      LONG max_localized_extended_states,
      BSTR** localized_extended_states,
      LONG* n_localized_extended_states) override;
  IFACEMETHODIMP get_locale(IA2Locale* locale) override;
  IFACEMETHODIMP get_accessibleWithCaret(IUnknown** accessible,
                                         LONG* caret_offset) override;

  //
  // IAccessible2_3 methods.
  //

  IFACEMETHODIMP get_selectionRanges(IA2Range** ranges, LONG* nRanges) override;

  //
  // IAccessible2_4 methods.
  //

  IFACEMETHODIMP setSelectionRanges(LONG nRanges, IA2Range* ranges) override;

  //
  // IServiceProvider method.
  //

  IFACEMETHODIMP QueryService(REFGUID guidService,
                              REFIID riid,
                              void** object) override;

  AccessibilityRootNode();
  virtual ~AccessibilityRootNode();

  void SetWindow(IAccessible2* window);

  void SetAlert(AccessibilityAlert* alert);

  AccessibilityAlert* GetOrCreateAlert();

  static AccessibilityRootNode* Create();

 private:
  // Helper method to redirect method calls to the contained window or alert.
  IAccessible* GetTargetAndChildID(VARIANT* var_id);

  IAccessible2* window_accessible_;

  AccessibilityAlert* alert_accessible_;
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_WINDOWS_ACCESSIBILITY_ROOT_NODE_H_
