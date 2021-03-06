/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
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
#include <eventql/sql/qtree/nodes/cluster_show_servers.h>
#include <eventql/sql/statements/cluster_show_servers.h>

namespace csql {

Vector<RefPtr<QueryTreeNode>> ClusterShowServersNode::inputTables() const {
  return Vector<RefPtr<QueryTreeNode>>{};
}

RefPtr<QueryTreeNode> ClusterShowServersNode::deepCopy() const {
  return new ClusterShowServersNode();
}

Vector<String> ClusterShowServersNode::getResultColumns() const {
  std::vector<std::string> list;
  for (const auto& e : ClusterShowServersExpression::kOutputColumns) {
    list.emplace_back(e.first);
  }
  return list;
}

Vector<QualifiedColumn> ClusterShowServersNode::getAvailableColumns() const {
  Vector<QualifiedColumn> cols;

  for (const auto& e : ClusterShowServersExpression::kOutputColumns) {
    cols.emplace_back(e.first, e.first, e.second);
  }

  return cols;
}

size_t ClusterShowServersNode::getComputedColumnIndex(
    const String& column_name,
    bool allow_add /* = false */) {
  return -1; // FIXME
}

size_t ClusterShowServersNode::getNumComputedColumns() const {
  return ClusterShowServersExpression::kOutputColumns.size();
}

String ClusterShowServersNode::toString() const {
  return "(describe-servers)";
}

SType ClusterShowServersNode::getColumnType(size_t idx) const {
  assert(idx < ClusterShowServersExpression::kOutputColumns.size());
  return ClusterShowServersExpression::kOutputColumns[idx].second;
}

void ClusterShowServersNode::encode(
    QueryTreeCoder* coder,
    const ClusterShowServersNode& node,
    OutputStream* os) {
}

RefPtr<QueryTreeNode> ClusterShowServersNode::decode (
    QueryTreeCoder* coder,
    InputStream* is) {
  return new ClusterShowServersNode();
}

} // namespace csql

