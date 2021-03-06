/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *   - Laura Schlimmer <laura@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#pragma once
#include <eventql/util/stdtypes.h>
#include <eventql/util/autoref.h>
#include <eventql/util/io/inputstream.h>
#include <eventql/util/io/outputstream.h>

#include "eventql/eventql.h"

namespace csql {
class Transaction;
class QueryTreeNode;

class QueryTreeCoder {
public:

  QueryTreeCoder(Transaction* txn);

  template <class T>
  void registerType(uint64_t wire_type_id);

  void encode(RefPtr<QueryTreeNode> tree, OutputStream* os);
  RefPtr<QueryTreeNode> decode(InputStream* is);

  Transaction* getTransaction() const;

protected:

  typedef Function<void (QueryTreeCoder*, RefPtr<QueryTreeNode>, OutputStream*)> EncodeFn;
  typedef Function<RefPtr<QueryTreeNode> (QueryTreeCoder*, InputStream*)> DecodeFn;

  struct QueryTreeCoderType {
    const std::type_info* type_id;
    uint64_t wire_type_id;
    QueryTreeCoder::EncodeFn encode_fn;
    QueryTreeCoder::DecodeFn decode_fn;
  };

  void registerType(QueryTreeCoderType f);

  Transaction* txn_;
  HashMap<const std::type_info*, QueryTreeCoderType> coders_by_type_id_;
  HashMap<uint64_t, QueryTreeCoderType> coders_by_wire_type_id_;
};

} // namespace csql

#include "qtree_coder_impl.h"
