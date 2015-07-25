// This file is part of the "libcortex" project
//   (c) 2009-2015 Christian Parpart <https://github.com/christianparpart>
//   (c) 2014-2015 Paul Asmuth <https://github.com/paulasmuth>
//
// libcortex is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License v3.0.
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <cortex-base/Api.h>
#include <cortex-base/Buffer.h>
#include <cortex-base/RefCounted.h>
#include <functional>

namespace cortex {

class DatagramConnector;

class CORTEX_API DatagramEndPoint : public RefCounted {
 public:
  DatagramEndPoint(DatagramConnector* connector, Buffer&& msg)
    : connector_(connector), message_(std::move(msg)) {}

  ~DatagramEndPoint();

  DatagramConnector* connector() const noexcept { return connector_; }

  const Buffer& message() const { return message_; }

  virtual size_t send(const BufferRef& response) = 0;

 private:
  //! the connector that was used to receive the message
  DatagramConnector* connector_;

  //! message received
  Buffer message_;
};

} // namespace cortex
