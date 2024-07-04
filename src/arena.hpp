#ifndef ARENA_HPP
#define ARENA_HPP
#include <cstddef>
#include <cstdint>

class Arena {
public:
  Arena();
  ~Arena();
  void *Alloc(size_t size);
private:
  uint8_t *buff;
  size_t buff_size;
  size_t free_idx;
};
#endif // ARENA_HPP
