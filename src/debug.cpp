#include "debug.hpp"
#include <vector>
#include <string>

struct DebugImpl {
    std::vector<std::string> lines;
};

Debug::Debug() : impl_(new DebugImpl()) {}

Debug::~Debug() {
    delete impl_;
}

void Debug::writeln(const char* line) {
    if (line) {
        impl_->lines.push_back(line);
    }
}

void Debug::clean() {
    impl_->lines.clear();
}

int Debug::get_line_count() const {
    return static_cast<int>(impl_->lines.size());
}

const char* Debug::get_line(int index) const {
    if (index >= 0 && index < static_cast<int>(impl_->lines.size())) {
        return impl_->lines[index].c_str();
    }
    return nullptr;
}
