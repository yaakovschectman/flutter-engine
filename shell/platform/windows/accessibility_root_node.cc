// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/windows/accessibility_root_node.h"

#include "flutter/fml/logging.h"
#include "flutter/third_party/accessibility/base/win/atl_module.h"

namespace flutter {

static constexpr LONG kWindowChildId = 1;
static constexpr LONG kInvalidChildId = 3;

AccessibilityRootNode::AccessibilityRootNode() : alert_accessible_(nullptr) {}

AccessibilityRootNode::~AccessibilityRootNode() {
  if (alert_accessible_) {
    alert_accessible_->Release();
    alert_accessible_ = nullptr;
  }
}

IAccessible* AccessibilityRootNode::GetTargetAndChildID(VARIANT* var_id) {
  LONG& child_id = var_id->lVal;
  if (V_VT(var_id) != VT_I4) {
    child_id = kInvalidChildId;
    return nullptr;
  }
  child_id = V_I4(var_id);
  if (!window_accessible_) {
    return nullptr;
  }
  if (child_id == CHILDID_SELF || child_id == kWindowChildId) {
    child_id = CHILDID_SELF;
    return window_accessible_;
  }
  if (child_id == kAlertChildId && alert_accessible_) {
    child_id = CHILDID_SELF;
    return alert_accessible_;
  }
  // A negative child ID can be used to refer to an AX node directly by its ID.
  if (child_id < 0) {
    return window_accessible_;
  }
  return nullptr;
}

IFACEMETHODIMP AccessibilityRootNode::accHitTest(LONG screen_physical_pixel_x,
                                                 LONG screen_physical_pixel_y,
                                                 VARIANT* child) {
  if (window_accessible_) {
    return window_accessible_->accHitTest(screen_physical_pixel_x,
                                          screen_physical_pixel_y, child);
  }
  child->vt = VT_EMPTY;
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::accDoDefaultAction(VARIANT var_id) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->accDoDefaultAction(var_id);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::accLocation(LONG* physical_pixel_left,
                                                  LONG* physical_pixel_top,
                                                  LONG* width,
                                                  LONG* height,
                                                  VARIANT var_id) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->accLocation(physical_pixel_left, physical_pixel_top, width,
                               height, var_id);
  }
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::accNavigate(LONG nav_dir,
                                                  VARIANT start,
                                                  VARIANT* end) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&start))) {
    return target->accNavigate(nav_dir, start, end);
  }
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::get_accChild(VARIANT var_child,
                                                   IDispatch** disp_child) {
  if (V_VT(&var_child) != VT_I4) {
    return E_FAIL;
  }
  LONG child_id = V_I4(&var_child);
  if (child_id == CHILDID_SELF) {
    *disp_child = this;
  } else if (!window_accessible_) {
    return E_FAIL;
  } else if (child_id == kWindowChildId) {
    *disp_child = window_accessible_;
  } else if (child_id == kAlertChildId && alert_accessible_) {
    *disp_child = alert_accessible_;
  } else if (child_id < 0) {
    // A negative child ID can be used to refer to an AX node directly by its
    // ID.
    return window_accessible_->get_accChild(var_child, disp_child);
  } else {
    return E_FAIL;
  }
  (*disp_child)->AddRef();
  return S_OK;
}

IFACEMETHODIMP AccessibilityRootNode::get_accChildCount(LONG* child_count) {
  LONG children = 0;
  if (window_accessible_) {
    children++;
  }
  if (alert_accessible_) {
    children++;
  }
  *child_count = children;
  return S_OK;
}

IFACEMETHODIMP AccessibilityRootNode::get_accDefaultAction(VARIANT var_id,
                                                           BSTR* def_action) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accDefaultAction(var_id, def_action);
  }
  *def_action = nullptr;
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::get_accDescription(VARIANT var_id,
                                                         BSTR* desc) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accDescription(var_id, desc);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::get_accFocus(VARIANT* focus_child) {
  if (window_accessible_) {
    return window_accessible_->get_accFocus(focus_child);
  }
  focus_child->vt = VT_EMPTY;
  return S_OK;
}

IFACEMETHODIMP AccessibilityRootNode::get_accKeyboardShortcut(VARIANT var_id,
                                                              BSTR* acc_key) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accKeyboardShortcut(var_id, acc_key);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::get_accName(VARIANT var_id,
                                                  BSTR* name_bstr) {
  if (V_I4(&var_id) == CHILDID_SELF) {
    std::wstring name = L"ROOT_NODE_VIEW";
    *name_bstr = SysAllocString(name.c_str());
    return S_OK;
  }
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accName(var_id, name_bstr);
  }
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::get_accParent(IDispatch** disp_parent) {
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::get_accRole(VARIANT var_id,
                                                  VARIANT* role) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accRole(var_id, role);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::get_accState(VARIANT var_id,
                                                   VARIANT* state) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accState(var_id, state);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::get_accHelp(VARIANT var_id, BSTR* help) {
  if (!help) {
    return E_INVALIDARG;
  }
  *help = {};
  return S_FALSE;
}

IFACEMETHODIMP AccessibilityRootNode::get_accValue(VARIANT var_id,
                                                   BSTR* value) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->get_accValue(var_id, value);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::put_accValue(VARIANT var_id,
                                                   BSTR new_value) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->put_accValue(var_id, new_value);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::get_accSelection(VARIANT* selected) {
  selected->vt = VT_EMPTY;
  return S_OK;
}

IFACEMETHODIMP AccessibilityRootNode::accSelect(LONG flagsSelect,
                                                VARIANT var_id) {
  IAccessible* target;
  if ((target = GetTargetAndChildID(&var_id))) {
    return target->accSelect(flagsSelect, var_id);
  }
  return E_FAIL;
}

IFACEMETHODIMP AccessibilityRootNode::get_accHelpTopic(BSTR* help_file,
                                                       VARIANT var_id,
                                                       LONG* topic_id) {
  if (help_file) {
    *help_file = nullptr;
  }
  if (topic_id) {
    *topic_id = -1;
  }
  return E_NOTIMPL;
}

IFACEMETHODIMP AccessibilityRootNode::put_accName(VARIANT var_id,
                                                  BSTR put_name) {
  return E_NOTIMPL;
}

//
// IAccessible2 methods.
//

IFACEMETHODIMP AccessibilityRootNode::role(LONG* role) {
  /*if (!window_accessible_) {
    return E_FAIL;
  }
  return window_accessible_->role(role);*/
  return E_NOTIMPL;
}

IFACEMETHODIMP AccessibilityRootNode::get_states(AccessibleStates* states) {
  *states = IA2_STATE_OPAQUE;
  return S_OK;
}

IFACEMETHODIMP AccessibilityRootNode::get_uniqueID(LONG* unique_id) {
  *unique_id = 0;
  return S_OK;
}

IFACEMETHODIMP AccessibilityRootNode::get_windowHandle(HWND* window_handle) {
  /*if (!window_accessible_) {
    return E_FAIL;
  }
  return window_accessible_->get_windowHandle(window_handle);*/
  return E_NOTIMPL;
}

IFACEMETHODIMP AccessibilityRootNode::get_relationTargetsOfType(BSTR type,
                                          LONG max_targets,
                                          IUnknown*** targets,
                                          LONG* n_targets) { return E_NOTIMPL; }

IFACEMETHODIMP AccessibilityRootNode::get_attributes(BSTR* attributes) {
  return E_NOTIMPL;
}

IFACEMETHODIMP AccessibilityRootNode::get_indexInParent(LONG* index_in_parent) {
  return E_NOTIMPL;
}

IFACEMETHODIMP AccessibilityRootNode::get_nRelations(LONG* n_relations) { return E_NOTIMPL; }

IFACEMETHODIMP AccessibilityRootNode::get_relation(LONG relation_index,
                            IAccessibleRelation** relation) { return E_NOTIMPL; }

IFACEMETHODIMP AccessibilityRootNode::get_relations(LONG max_relations,
                              IAccessibleRelation** relations,
                              LONG* n_relations) { return E_NOTIMPL; }

IFACEMETHODIMP AccessibilityRootNode::get_attribute(BSTR name, VARIANT* attribute) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_extendedRole(BSTR* extended_role) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::scrollTo(enum IA2ScrollType scroll_type) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::scrollToPoint(enum IA2CoordinateType coordinate_type,
                              LONG x,
                              LONG y) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_groupPosition(LONG* group_level,
                                  LONG* similar_items_in_group,
                                  LONG* position_in_group) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_localizedExtendedRole(
    BSTR* localized_extended_role) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_nExtendedStates(LONG* n_extended_states) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_extendedStates(LONG max_extended_states,
                                  BSTR** extended_states,
                                  LONG* n_extended_states) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_localizedExtendedStates(
    LONG max_localized_extended_states,
    BSTR** localized_extended_states,
    LONG* n_localized_extended_states) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_locale(IA2Locale* locale) { return E_NOTIMPL; }
IFACEMETHODIMP AccessibilityRootNode::get_accessibleWithCaret(IUnknown** accessible,
                                        LONG* caret_offset) { return E_NOTIMPL; }

//
// IAccessible2_3 methods.
//

IFACEMETHODIMP AccessibilityRootNode::get_selectionRanges(IA2Range** ranges, LONG* nRanges) { return E_NOTIMPL; }

//
// IAccessible2_4 methods.
//

IFACEMETHODIMP AccessibilityRootNode::setSelectionRanges(LONG nRanges, IA2Range* ranges) { return E_NOTIMPL; }

//
// IServiceProvider implementation.
//

IFACEMETHODIMP AccessibilityRootNode::QueryService(REFGUID guidService,
                                                   REFIID riid,
                                                   void** object) {
  if (!object) {
    return E_INVALIDARG;
  }

  if (guidService == IID_IAccessible || guidService == IID_IAccessible2 || guidService == IID_IAccessible2_2 || guidService == IID_IAccessible2_3 || guidService == IID_IAccessible2_4) {
    return QueryInterface(riid, object);
  }

  *object = nullptr;
  return E_FAIL;
}

void AccessibilityRootNode::SetWindow(IAccessible2* window) {
  window_accessible_ = window;
}

AccessibilityAlert* AccessibilityRootNode::GetOrCreateAlert() {
  if (!alert_accessible_) {
    CComObject<AccessibilityAlert>* instance = nullptr;
    HRESULT hr = CComObject<AccessibilityAlert>::CreateInstance(&instance);
    if (!SUCCEEDED(hr)) {
      FML_LOG(FATAL) << "Failed to create alert accessible";
    }
    instance->AddRef();
    instance->SetParent(this);
    alert_accessible_ = instance;
  }
  return alert_accessible_;
}

// static
AccessibilityRootNode* AccessibilityRootNode::Create() {
  ui::win::CreateATLModuleIfNeeded();
  CComObject<AccessibilityRootNode>* instance = nullptr;
  HRESULT hr = CComObject<AccessibilityRootNode>::CreateInstance(&instance);
  if (!SUCCEEDED(hr) || !instance) {
    FML_LOG(FATAL) << "Failed to create accessibility root node";
  }
  instance->AddRef();
  return instance;
}

}  // namespace flutter
