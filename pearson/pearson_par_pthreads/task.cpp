#include "task.hpp"

namespace Threads {
void Task::execute() const { func(args); }
} // namespace Threads
