/*
 * Copyright (c) 2017-2018, The Triton Developers.
 * Portions Copyright (c) 2012-2017, The CryptoNote Developers, The Bytecoin Developers.
 *
 * This file is part of Triton.
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "BlockingQueue.h"
#include "ConsoleTools.h"

namespace Common {

class AsyncConsoleReader {

public:

  AsyncConsoleReader();
  ~AsyncConsoleReader();

  void start();
  bool getline(std::string& line);
  void stop();
  bool stopped() const;

private:

  void consoleThread();
  bool waitInput();

  std::atomic<bool> m_stop;
  std::thread m_thread;
  BlockingQueue<std::string> m_queue;
};


class ConsoleHandler {
public:

  ~ConsoleHandler();

  typedef std::function<bool(const std::vector<std::string> &)> ConsoleCommandHandler;

  std::string getUsage() const;
  void setHandler(const std::string& command, const ConsoleCommandHandler& handler, const std::string& usage = "");
  void requestStop();
  bool runCommand(const std::vector<std::string>& cmdAndArgs);

  void start(bool startThread = true, const std::string& prompt = "", Console::Color promptColor = Console::Color::Default);
  void stop();
  void wait(); 

private:

  typedef std::map<std::string, std::pair<ConsoleCommandHandler, std::string>> CommandHandlersMap;

  virtual void handleCommand(const std::string& cmd);

  void handlerThread();

  std::thread m_thread;
  std::string m_prompt;
  Console::Color m_promptColor = Console::Color::Default;
  CommandHandlersMap m_handlers;
  AsyncConsoleReader m_consoleReader;
};

}
