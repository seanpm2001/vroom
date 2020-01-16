#include "mmap_vector.h"

int mmap_vector::num = 0;

bool fallocate(int fd, size_t len) {
  fstore_t store = {F_ALLOCATECONTIG, F_PEOFPOSMODE, 0, (off_t)len};
  // Try to get a continuous chunk of disk space
  int ret = fcntl(fd, F_PREALLOCATE, &store);
  if (-1 == ret) {
    // OK, perhaps we are too fragmented, allocate non-continuous
    store.fst_flags = F_ALLOCATEALL;
    ret = fcntl(fd, F_PREALLOCATE, &store);
    if (-1 == ret)
      return false;
  }
  return 0 == ftruncate(fd, len);
}