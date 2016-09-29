/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Laura Schlimmer <laura@eventql.io>
 *   - Paul Asmuth <paul@eventql.io>
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
#include <queue>
#include <eventql/eventql.h>
#include "eventql/cli/commands/cli_command.h"
#include "eventql/config/process_config.h"
#include "eventql/transport/native/client_tcp.h"

namespace eventql {
namespace cli {

class TableImport : public CLICommand {
public:
  TableImport(RefPtr<ProcessConfig> process_cfg);

  static const uint64_t kDefaultBatchSize = 25; //FIXME
  static const uint64_t kDefaultNumThreads = 2; //FIXME

  Status execute(
      const std::vector<std::string>& argv,
      FileInputStream* stdin_is,
      OutputStream* stdout_os,
      OutputStream* stderr_os) override;


  const String& getName() const override;
  const String& getDescription() const override;
  void printHelp(OutputStream* stdout_os) const override;

protected:
  void runThread(size_t idx);

  static const String kName_;
  static const String kDescription_;
  RefPtr<ProcessConfig> process_cfg_;
  bool done_;
  std::mutex mutex_;
  std::condition_variable cv_;
  size_t num_threads_;
  std::vector<std::thread> threads_;
  std::vector<std::unique_ptr<native_transport::TCPClient>> clients_;
  std::queue<std::vector<std::string>> batches_;
  std::string database_;
  std::string table_;
};

} // namespace cli
} // namespace eventql

