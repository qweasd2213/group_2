
#if !defined(TASK_HPP)
#define TASK_HPP
namespace Threads {
struct Task {
    void *(*func)(void *);
    void *args;
    void execute() const;
};
} // namespace Threads

#endif
