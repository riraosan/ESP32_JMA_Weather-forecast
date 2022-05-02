
#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>

class Task {
public:
  Task(std::string taskName = "AutoConnect", uint16_t taskSize = 4096, uint8_t priority = 5);
  ~Task();
  void start(void* taskData = nullptr);
  void stop();

  void delay(int ms);

  virtual void run(void* data) = 0;

  void setTaskSize(uint16_t size);
  void setTaskPriority(uint8_t priority);
  void setTaskName(std::string name);
  void setCore(BaseType_t coreID);

private:
  xTaskHandle m_handle;
  void*       m_taskdata;
  static void runTask(void* data);
  std::string m_taskname;
  uint16_t    m_tasksize;
  uint8_t     m_priority;
  BaseType_t  m_coreid;
};
