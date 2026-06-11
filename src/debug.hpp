#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <memory>

class Debug {
public:
    Debug();
    ~Debug();
    void writeln(const char* line);
    void clean();
    
    int get_line_count() const;
    const char* get_line(int index) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif // DEBUG_HPP
