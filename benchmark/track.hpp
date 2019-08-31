#ifndef JULES_BENCHMARK_MEMORY_TRACK_H
#define JULES_BENCHMARK_MEMORY_TRACK_H

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>

constexpr double ToMB = 1.0 / 1024.0 / 1024.0;

template <typename T> struct SafeAllocator : public std::allocator<T>
{
  using pointer = typename std::allocator<T>::pointer;
  using size_type = typename std::allocator<T>::size_type;
  using value_type = typename std::allocator<T>::value_type;

  template <typename U> struct rebind
  {
    using other = SafeAllocator<U>;
  };

  SafeAllocator() {}

  template <typename U> SafeAllocator(const SafeAllocator<U>& u) : std::allocator<T>(u) {}

  pointer allocate(size_type size, std::allocator<void>::const_pointer = nullptr)
  {
    void* p = std::malloc(size * sizeof(T));
    if (p == 0) {
      throw std::bad_alloc{};
    }
    return static_cast<pointer>(p);
  }

  void deallocate(pointer p, size_type) { std::free(p); }
};

struct Memory
{
public:
  struct Alloc
  {
    std::size_t size = 0, count = 0;

    void inc(std::size_t i)
    {
      size += i;
      ++count;
    }

    void dec(std::size_t i)
    {
      size -= i;
      --count;
    }

    bool operator<(const Alloc& other) const { return size < other.size; }

    friend std::ostream& operator<<(std::ostream& os, const Alloc& alloc)
    {
      return os << (alloc.size * ToMB) << " MB in " << alloc.count
                << " block (mean: " << (alloc.count > 0 ? (alloc.size * ToMB) / alloc.count : 0.0) << " MB/block)";
    }
  };

  static void Summary(std::ostream& os = std::cerr)
  {
    const auto& mem = Instance();

    os << "  Total usage:  " << mem.total_usage << std::endl << "  Still using:  " << mem.still_using << std::endl;

    if (mem.recent_usage.count > 0u)
      os << "  Recent usage: " << mem.recent_usage << std::endl;

    if (mem.recently_allocated.size() > 0u)
      os << "  Recently allocated blocks:" << std::endl;
    for (auto&& block : mem.recently_allocated)
      os << "\t" << block.first << ": " << (block.second * ToMB) << " MB" << std::endl;

    if (mem.recently_freed.size() > 0u)
      os << "  Recently freed blocks:" << std::endl;
    for (auto&& block : mem.recently_freed)
      os << "\t" << block.first << ": " << (block.second * ToMB) << " MB" << std::endl;
  }

  static void Reset()
  {
    auto& mem = Instance();

    mem.recently_allocated.clear();
    mem.recently_freed.clear();

    mem.recent_usage = {};
  }

  static void Finalize()
  {
    delete instance;
    instance = nullptr;
  }

  static void Track(void* ptr, std::size_t size)
  {
    auto& mem = Instance();

    mem.all_blocks[ptr] = size;
    mem.recently_allocated[ptr] = size;

    mem.total_usage.inc(size);
    mem.recent_usage.inc(size);
    mem.still_using.inc(size);
  }

  static void UnTrack(void* ptr)
  {
    auto& mem = Instance();

    auto it = mem.all_blocks.find(ptr);
    if (it != mem.all_blocks.end()) {
      mem.still_using.dec(it->second);

      mem.recently_freed.push_back(*it);
      mem.all_blocks.erase(it);
    }
  }

private:
  static Memory& Instance()
  {
    if (!instance)
      instance = new Memory;
    return *instance;
  }

  static void* operator new(std::size_t) { return std::malloc(sizeof(Memory)); }
  static void operator delete(void* ptr) noexcept { std::free(ptr); }

  Memory() {}
  ~Memory() {}

  static Memory* instance;

  Alloc total_usage, recent_usage, still_using;

  std::unordered_map<void*, std::size_t, std::hash<void*>, std::equal_to<void*>,
                     SafeAllocator<std::pair<void*const, std::size_t>>>
    all_blocks, recently_allocated;

  std::vector<std::pair<void* const, std::size_t>, SafeAllocator<std::pair<void* const, std::size_t>>> recently_freed;
};

Memory* Memory::instance = nullptr;

void* operator new(std::size_t size)
{
  auto ptr = std::malloc(size);
  Memory::Track(ptr, size);
  return ptr;
}

void operator delete(void* ptr) noexcept
{
  if (ptr)
    Memory::UnTrack(ptr);
  std::free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept
{
  if (ptr)
    Memory::UnTrack(ptr);
  std::free(ptr);
}

#endif // JULES_BENCHMARK_MEMORY_TRACK_H
