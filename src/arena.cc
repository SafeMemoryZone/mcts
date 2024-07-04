#include "arena.hpp"
#include <cstdint>
#include <cstdlib>

Arena::Arena() : buff(nullptr), buff_size(0), free_idx(0) {}

Arena::~Arena() {
  if (this->buff) 
    free(this->buff);
}

void *Arena::Alloc(size_t size) {
  if (this->buff_size == 0) {
    this->buff = new uint8_t[size * 2];
    this->buff_size = size * 2;
  } 
  else if (this->free_idx + size > this->buff_size) {
    size_t new_buff_size = this->buff_size + size * 2;
    uint8_t* new_buff = static_cast<uint8_t*>(realloc(this->buff, new_buff_size));
    this->buff = new_buff;
    this->buff_size = new_buff_size;
  }

  void* ptr = this->buff + this->free_idx;
  this->free_idx += size;

  return ptr;
}
