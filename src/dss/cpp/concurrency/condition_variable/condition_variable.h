#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

namespace dss
{

class condition_variable
{
  condition_variable();
  condition_variable(const condition_variable&) = delete;
  ~condition_variable();

  // notifications to threads
  void notify_one() noexcept;
  void notify_all() noexcept;
};

} // namespace dss

#endif // CONDITION_VARIABLE_H
