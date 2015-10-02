#ifndef JULES_BENCHMARK_MEMORY_TRACK_H
#define JULES_BENCHMARK_MEMORY_TRACK_H

#include <cstdlib>

struct Memory {
  public:
    static void Reset() { Instance().reset(); }
    static void Finalize() {
        delete instance;
        instance = nullptr;
    }

    static std::size_t Size() { return Instance().size; }
    static std::size_t Count() { return Instance().count; }

    static void Track(std::size_t size) {
        ++Instance().count;
        Instance().size += size;
    }

  private:
    static Memory& Instance() {
        if (!instance)
            instance = new Memory;
        return *instance;
    }

    void reset() {
        Instance().size = 0;
        Instance().count = 0;
    }

    static void *operator new(std::size_t) {
        return std::malloc(sizeof(Memory));
    }

    Memory() {}
    ~Memory() {}

    static Memory *instance;
    std::size_t size = 0, count = 0;
};

Memory* Memory::instance = nullptr;

void *operator new(std::size_t size) {
    Memory::Track(size);
    return std::malloc(size);
}

#endif // JULES_BENCHMARK_MEMORY_TRACK_H
