/**
 * Copyright (c) 2015 - The CM Authors <legal@clickmatcher.com>
 *   All Rights Reserved.
 *
 * This file is CONFIDENTIAL -- Distribution or duplication of this material or
 * the information contained herein is strictly forbidden unless prior written
 * permission is obtained.
 */
#pragma once
#include "stx/stdtypes.h"
#include "stx/autoref.h"
#include <jsapi.h>

using namespace stx;

namespace zbase {

class JavaScriptContext : public RefCounted {
public:

  JavaScriptContext(JSRuntime* runtime);
  ~JavaScriptContext();

protected:
  JSContext* ctx_;
  JS::PersistentRooted<JSObject*> global_;
};

} // namespace zbase
