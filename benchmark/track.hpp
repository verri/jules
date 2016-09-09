#ifndef JULES_BENCHMARK_MEMORY_TRACK_H
#define JULES_BENCHMARK_MEMORY_TRACK_H

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>

constexpr double ToMB = 1.0 / 1024.0 / 1024.0;

template <typename T> struct SafeAllocator : public std::allocator<T> {
    using pointer = typename std::allocator<T>::pointer;
    using size_type = typename std::allocator<T>::size_type;
    using value_type = typename std::allocator<T>::value_type;

    template <typename U> struct rebind {
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

struct Memory {
  public:
    struct Alloc {
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

    static const auto& Current() { return Instance().current; }
    static const auto& Total() { return Instance().total; }
    static const auto& Peak() { return Instance().peak; }

    static void Summary(std::ostream& os = std::cerr)
    {
        os << "Still using: " << Current() << std::endl
           << "Peak usage:  " << Peak() << std::endl
           << "Requested:   " << Total() << std::endl;

        os << "Blocks:" << std::endl;
        for (auto&& block : Instance().blocks)
            os << "\t" << block.first << ": " << (block.second * ToMB) << " MB" << std::endl;

        os << "Freed blocks:" << std::endl;
        for (auto&& block : Instance().freed)
            os << "\t" << block.first << ": " << (block.second * ToMB) << " MB" << std::endl;
    }

    static void Reset() { Finalize(); }

    static void Finalize()
    {
        delete instance;
        instance = nullptr;
    }

    static void Track(void* ptr, std::size_t size)
    {
        auto& mem = Instance();

        mem.blocks[ptr] = size;

        mem.current.inc(size);
        mem.total.inc(size);

        if (mem.peak < mem.current)
            mem.peak = mem.current;
    }

    static void UnTrack(void* ptr)
    {
        auto& mem = Instance();

        auto it = mem.blocks.find(ptr);
        if (it != mem.blocks.end()) {
            mem.current.dec(it->second);
            mem.freed.push_back(*it);
            mem.blocks.erase(it);
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

    Alloc total, peak, current;
    std::unordered_map<void*, std::size_t, std::hash<void*>, std::equal_to<void*>,
                       SafeAllocator<std::pair<void* const, std::size_t>>>
        blocks;
    std::vector<std::pair<void* const, std::size_t>, SafeAllocator<std::pair<void* const, std::size_t>>> freed;
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

#endif // JULES_BENCHMARK_MEMORY_TRACK_H
