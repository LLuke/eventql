/**
 * Copyright (c) 2016 zScale Technology GmbH <legal@zscale.io>
 * Authors:
 *   - Paul Asmuth <paul@zscale.io>
 *   - Laura Schlimmer <laura@zscale.io>
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
#include <eventql/cli/commands/table_disable_replication.h>
#include "eventql/util/random.h"
#include <eventql/util/cli/flagparser.h>
#include "eventql/util/logging.h"
#include "eventql/config/config_directory.h"
#include "eventql/db/metadata_operation.h"
#include "eventql/db/metadata_coordinator.h"
#include "eventql/db/server_allocator.h"

namespace eventql {
namespace cli {

const String TableDisableReplication::kName_ = "table-disable-replication";
const String TableDisableReplication::kDescription_ = "Disable replication";

TableDisableReplication::TableDisableReplication(
    RefPtr<ProcessConfig> process_cfg) :
    process_cfg_(process_cfg) {}

Status TableDisableReplication::execute(
    const std::vector<std::string>& argv,
    FileInputStream* stdin_is,
    OutputStream* stdout_os,
    OutputStream* stderr_os) {
  ::cli::FlagParser flags;
  flags.defineFlag(
      "cluster_name",
      ::cli::FlagParser::T_STRING,
      true,
      NULL,
      NULL,
      "node name",
      "<string>");

  flags.defineFlag(
      "namespace",
      ::cli::FlagParser::T_STRING,
      true,
      NULL,
      NULL,
      "namespace",
      "<string>");

  flags.defineFlag(
      "table_name",
      ::cli::FlagParser::T_STRING,
      true,
      NULL,
      NULL,
      "table name",
      "<string>");

  try {
    flags.parseArgv(argv);

    ScopedPtr<ConfigDirectory> cdir;
    {
      auto rc = ConfigDirectoryFactory::getConfigDirectoryForClient(
          process_cfg_.get(),
          &cdir);

      if (rc.isSuccess()) {
        rc = cdir->start();
      }

      if (!rc.isSuccess()) {
        stderr_os->write(StringUtil::format("ERROR: $0\n", rc.message()));
        return rc;
      }
    }

    auto cfg = cdir->getTableConfig(
        flags.getString("namespace"),
        flags.getString("table_name"));

    cfg.mutable_config()->set_disable_replication(true);

    iputs("cfg: $0", cfg.DebugString());
    cdir->updateTableConfig(cfg);
    cdir->stop();
  } catch (const Exception& e) {
    stderr_os->write(StringUtil::format(
        "$0: $1\n",
        e.getTypeName(),
        e.getMessage()));
    return Status(e);
  }

  return Status::success();
}

const String& TableDisableReplication::getName() const {
  return kName_;
}

const String& TableDisableReplication::getDescription() const {
  return kDescription_;
}

void TableDisableReplication::printHelp(OutputStream* stdout_os) const {
  stdout_os->write(StringUtil::format(
      "\nevqlctl-$0 - $1\n\n", kName_, kDescription_));

  stdout_os->write(
      "Usage: evqlctl table-disable-replication [OPTIONS]\n"
      "  --namespace              The name of the namespace.\n"
      "  --cluster_name           The name of the cluster.\n"
      "  --table_name             The name of the table to split.\n");
}

} // namespace cli
} // namespace eventql
