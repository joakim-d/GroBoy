#ifndef CPU_H
#define CPU_H

#include <memory>
#include <tr1/functional>

class Memory;

class Cpu {
public:
    Cpu();
    ~Cpu();
    int run();
    void reset();
    void set_memory(Memory *memory);
    void make_request(int type);
    void set_update_callback(std::tr1::function<void (int)> const &callback);

private:
    class CpuPrivate;
    std::unique_ptr<CpuPrivate> d_;
};

#endif

