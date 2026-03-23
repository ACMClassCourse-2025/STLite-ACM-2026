#include <iostream>

#include "../include/priority_queue.hpp"


int rand_int() {
    static int seed = 1'235'678;
    seed = (seed * 137 + 61'415'021) % 1'000'000'021;
    return seed;
}

class DangerousData {
   public:
    explicit DangerousData(int a) : ptr(new int(a)) {
    }
    DangerousData(const DangerousData& other) : ptr(new int(*other.ptr)) {
    }
    DangerousData(DangerousData&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    ~DangerousData() {
        delete ptr;
    }
    DangerousData& operator=(const DangerousData& other) {
        if (this == &other) return *this;
        delete ptr;
        ptr = new int(*other.ptr);
        return *this;
    }
    DangerousData& operator=(DangerousData&& other) noexcept {
        if (this == &other) return *this;
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }
    bool operator<(const DangerousData& other) const {
        return *ptr < *other.ptr;
    }

   private:
    int* ptr;
};

struct DangerousCompare {
    DangerousCompare() : ptr(new int(0)) {
    }
    DangerousCompare(const DangerousCompare& other) : ptr(new int(*other.ptr)) {
    }
    DangerousCompare(DangerousCompare&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    ~DangerousCompare() {
        delete ptr;
    }
    DangerousCompare& operator=(const DangerousCompare& other) {
        if (this == &other) return *this;
        delete ptr;
        ptr = new int(*other.ptr);
        return *this;
    }
    DangerousCompare& operator=(DangerousCompare&& other) noexcept {
        if (this == &other) return *this;
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    bool operator()(const DangerousData& lhs, const DangerousData& rhs) const {
        return lhs < rhs;
    }

   private:
    int* ptr;
};

struct TaggedData {
    int prior_;
    char tag_;

    bool operator<(const TaggedData& other) const {
        return prior_ > other.prior_;
    }
    bool operator==(const TaggedData& other) const {
        return prior_ == other.prior_;
    }
    std::string as_string() const {
        return "{" + std::to_string(prior_) + ", " + tag_ + "}";
    }
};

// But why might someone use placement new to manage data?
bool TestDataManagementAndSelfMerge() {
    std::cout << "Testing data management and self merge...";
    sjtu::priority_queue<DangerousData, DangerousCompare> pq1, pq2;
    for (int i = 0; i < 1000; ++i) {
        pq1.push(DangerousData(rand_int()));
        pq2.push(DangerousData(rand_int()));
    }
    for (int i = 0; i < 500; ++i) {
        pq1.pop();
    }
    pq1.merge(pq2);
    pq2.merge(pq1);
    pq2.merge(pq1);
    pq2.merge(pq2);  // nothing shall happen;
    for (int i = 0; i < 1000; ++i) {
        pq1.push(DangerousData(rand_int()));
    }
    pq1.merge(pq1);  // nothing shall happen;
    pq1.merge(pq2);

    bool verdict = pq1.size() == 2500 && pq2.empty();
    std::cout << (verdict ? " Pass." : " Fail.") << std::endl;
    return verdict;
}

bool TestMultipleMaximums() {
    std::cout << "Testing multiple maximums...";
    std::vector<TaggedData> original_data;
    std::string tags = "abcdefg";
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            original_data.emplace_back(
                TaggedData{.prior_ = i, .tag_ = tags[j]});
        }
    }
    sjtu::priority_queue<TaggedData> pq;
    for (const auto& data : original_data) {
        pq.push(data);
    }
    std::vector<TaggedData> sorted_data;
    while (!pq.empty()) {
        sorted_data.push_back(pq.top());
        pq.pop();
    }
    bool verdict = original_data == sorted_data;
    std::cout << (verdict ? " Pass." : " Fail.") << std::endl;
    if (verdict) {
        std::cerr << "original list:  ";
        for (const auto& data : original_data)
            std::cerr << data.as_string() << ' ';
        std::cerr << std::endl;
        std::cerr << "pq-sorted list: ";
        for (const auto& data : sorted_data)
            std::cerr << data.as_string() << ' ';
        std::cerr << std::endl;
        std::cerr << "They might not look same (sometimes they do)... But that "
                     "does not matter!"
                  << std::endl;
    }
    return verdict;
}

bool TestClear() {
    std::cout << "Testing clear function...";
    sjtu::priority_queue<DangerousData, DangerousCompare> pq;
    pq.clear();
    for (int i = 0; i < 10000; ++i) {
        pq.push(DangerousData(rand_int()));
        if (rand_int() % 10 == 0) {
            pq.pop();
        }
    }
    pq.clear();
    bool verdict = pq.empty();
    std::cout << (verdict ? " Pass." : " Fail.") << std::endl;
    return verdict;
}

bool TestExceptions() {
    std::cout << "Testing exceptions...";
    bool flag1 = false;
    try {
        sjtu::priority_queue<int> pq;
        for (int i = 0; i < 100; ++i) pq.push(i);
        for (int i = 0; i < 100; ++i) pq.pop();
        pq.pop();
    } catch (sjtu::container_is_empty&) {
        flag1 = true;
    } catch (...) {
        // Nah.
    }
    bool flag2 = false;
    try {
        sjtu::priority_queue<int> pq;
        for (int i = 0; i < 100; ++i) pq.push(i);
        for (int i = 0; i < 100; ++i) pq.pop();
        int _top = pq.top();
    } catch (sjtu::container_is_empty&) {
        flag2 = true;
    } catch (...) {
        // Nah.
    }
    bool verdict = flag1 && flag2;
    std::cout << (verdict ? " Pass." : " Fail.") << std::endl;
    return verdict;
}

int main() {
    std::cout << "# Some information printed through stderr might appear in "
                 "your console.\n"
                 "# The following is what shall be output through stdout.\n"
              << std::flush;
    TestDataManagementAndSelfMerge();
    TestMultipleMaximums();
    TestClear();
    TestExceptions();
    return 0;
}