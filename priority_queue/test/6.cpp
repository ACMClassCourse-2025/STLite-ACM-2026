/**
 * The old testcase 6 does not cooperate with this version of requirements.
 * So I discarded it and rewrite a new one (still using its testing strategy,
 * though). See to 6.cpp.bak for the old testcase 6.
 */

#include <cstdint>
#include <iostream>
#include <vector>

#include "../include/priority_queue.hpp"

struct ComparisonError : public std::exception {
    const char* what() const noexcept override {
        return "Comparison error";
    }
};

std::uint32_t rand_u32() {
    static std::uint32_t seed = 7'355'608;
    seed = seed * 137 + 67'418'001;
    return seed;
}

// Extended natural number, or "N + {+infty}".
//
// I could just use std::optional<int> here... Whatever.
struct ExtNat {
    // Not default constructible.
    ExtNat() = delete;
    // construct a finite ExtNat.
    // intentionally allowing implicit conversion from std::uint32_t.
    ExtNat(std::uint32_t val) : val(val), is_infty(false) {
    }
    ExtNat(const ExtNat&) = default;
    ExtNat& operator=(const ExtNat&) = default;
    ~ExtNat() = default;

    // construct an infty ExtNat.
    static ExtNat infty() {
        static std::uint32_t infty_counter = 0;
        return ExtNat(infty_counter++, true);
    }

    static ExtNat random() {
        return ExtNat(rand_u32());
    }

    static ExtNat random_range(std::uint32_t min, std::uint32_t max) {
        return ExtNat(rand_u32() % (max - min + 1) + min);
    }

    std::string as_string() const {
        // return is_infty ? "infty[no." + std::to_string(val) + "]" : "nat[" +
        // std::to_string(val) + "]";
        return is_infty ? "infty" : std::to_string(val);
    }

    // Always a right and valid less-than comparison.
    friend bool operator<(const ExtNat& lhs, const ExtNat& rhs) {
        if (lhs.is_infty || rhs.is_infty) {
            // infty is always considered larger than finite data.
            // lhs infty and rhs finite: lhs > rhs, returns false
            // lhs finite and rhs infty: lhs < rhs, returns true
            // lhs infty and rhs infty: lhs = rhs, returns false
            return !lhs.is_infty;
        }
        return lhs.val < rhs.val;
    }

    // Always a right and valid equality comparison.
    friend bool operator==(const ExtNat& lhs, const ExtNat& rhs) {
        if (lhs.is_infty || rhs.is_infty) {
            return lhs.is_infty && rhs.is_infty;
        }
        return lhs.val == rhs.val;
    }

    // For valid ExtNat, val stands for the natural number it represents.
    // For infty ExtNat, val stands for the counter that represents how many
    // infty ExtNats have been constructed.
    std::uint32_t val;
    bool is_infty;

   private:
    ExtNat(std::uint32_t val, bool is_infty) : val(val), is_infty(is_infty) {
    }
};

struct DizzyCompare {
    static void be_bad() {
        DisableExtendedComparison = true;
    }
    static void be_good() {
        DisableExtendedComparison = false;
    }

    bool operator()(const ExtNat& lhs, const ExtNat& rhs) const {
        if (lhs.is_infty || rhs.is_infty) {
            if (DisableExtendedComparison) throw ComparisonError();
            return !lhs.is_infty;
        }
        return lhs.val < rhs.val;
    }

   private:
    // Whether to allow extended comparison.
    static bool DisableExtendedComparison;
};
bool DizzyCompare::DisableExtendedComparison = false;

/** copies a priority queue and flattens it into a vector.
 * The largest element is at the front of the vector.
 * @note This function shall not throw exceptions.
 *       Not only because it is used in testing, but also because it calls the
 *copy constructor of sjtu::priority_queue.
 **/
template <typename T, typename Compare>
std::vector<T> deheapify(sjtu::priority_queue<T, Compare> queue) {
    std::vector<T> res;
    res.reserve(queue.size());
    try {
        while (!queue.empty()) {
            res.push_back(queue.top());
            queue.pop();
        }
    } catch (...) {
        std::cerr
            << "Fatal error: Exception thrown in deheapify. The test logic "
               "is flawed. Recheck it."
            << std::endl;
        exit(0);
    }
    return res;
}

bool compare_state(const std::vector<ExtNat>& expected_state,
                   const std::vector<ExtNat>& replica_state) {
    if (replica_state == expected_state) {
        // std::cout << "States are identical." << std::endl;
        return true;
    } else {
        std::cout << "States are different." << std::endl;
        std::cout << "Expected state (size " << expected_state.size() << "): ";
        for (const auto& item : expected_state) {
            std::cout << item.as_string() << " ";
        }
        std::cout << std::endl;
        std::cout << "Replica state (size " << replica_state.size() << "): ";
        for (const auto& item : replica_state) {
            std::cout << item.as_string() << " ";
        }
        std::cout << std::endl;
        return false;
    }
}

// Test 1: Basic push/pop test (no exceptions triggered)
bool test1() {
    std::cout << "[Test1] Running..." << std::endl;
    sjtu::priority_queue<ExtNat>
        pq;  // the default compare is std::less<ExtNat>, which is a good one.

    try {
        for (int i = 0; i < 1000; i++) {
            pq.push(ExtNat::random());
        }
        auto vec = deheapify(pq);
        for (size_t i = 1; i < vec.size(); i++) {
            if (vec[i - 1] < vec[i]) {
                std::cout << "Test1 Failed. Heap property violated!"
                          << std::endl;
                return false;
            }
        }
        std::cout << "Test1 Passed." << std::endl;
        return true;
    } catch (...) {
        std::cout << "Test1 Failed. Unexpected exception thrown." << std::endl;
        return false;
    }
}

// Test 2: Test push when Compare throws exception
template <std::uint32_t InftyCount>
bool test2() {
    static_assert(InftyCount <= 2);
    std::string test_name = "Test2<" + std::to_string(InftyCount) + ">";
    std::cout << "[" << test_name << "] Running..." << std::endl;
    sjtu::priority_queue<ExtNat, DizzyCompare> pq;

    DizzyCompare::be_good();
    if (InftyCount == 0) {
        for (int j = 0; j < 142; j++) {
            pq.push(ExtNat::random_range(1, 100));
        }
    } else {
        for (int i = 0; i < InftyCount; i++) {
            pq.push(ExtNat::infty());
            for (int j = 0; j < 142 / InftyCount; j++) {
                pq.push(ExtNat::random_range(1, 100));
            }
        }
    }
    // do not trigger exception at copy construction.
    auto expected_state = deheapify(pq);
    sjtu::priority_queue<ExtNat, DizzyCompare> pq_replica = pq;

    DizzyCompare::be_bad();
    bool exceptionRight = false;
    try {
        pq_replica.push(ExtNat::infty());
    } catch (const ComparisonError& e) {
        exceptionRight = true;
    } catch (...) {
        std::cout << test_name
                  << " Failed. Exception caught but not the expected one."
                  << std::endl;
        return false;
    }
    if (!exceptionRight) {
        // Not throwing an exception is also acceptable (But how did you do it?
        // :P). Update the state to expect.
        std::cerr << test_name << ": Not triggering exception? Wow."
                  << std::endl;
        expected_state.insert(expected_state.begin(), ExtNat::infty());
    }

    DizzyCompare::be_good();
    auto replica_state = deheapify(pq_replica);

    if (!compare_state(expected_state, replica_state)) {
        std::cout << test_name
                  << " Failed. Queue state changed after exception."
                  << std::endl;
        return false;
    }

    std::cout << test_name << " Passed." << std::endl;
    return true;
}

// Test 3: Test pop when Compare throws exception
template <std::uint32_t InftyCount>
bool test3() {
    static_assert(1 <= InftyCount && InftyCount <= 3);
    std::string test_name = "Test3<" + std::to_string(InftyCount) + ">";
    std::cout << "[" << test_name << "] Running..." << std::endl;
    sjtu::priority_queue<ExtNat, DizzyCompare> pq;

    DizzyCompare::be_good();
    for (int i = 0; i < InftyCount; i++) {
        pq.push(ExtNat::infty());
        for (int j = 0; j < 900 / InftyCount; j++) {
            pq.push(ExtNat::random_range(1, 1000));
        }
    }
    auto expected_state = deheapify(pq);
    sjtu::priority_queue<ExtNat, DizzyCompare> pq_replica = pq;

    DizzyCompare::be_bad();
    bool exceptionCaught = false;
    try {
        pq_replica.pop();
    } catch (const ComparisonError& e) {
        exceptionCaught = true;
    } catch (...) {
        std::cout << test_name
                  << " Failed. Exception caught but not the expected one."
                  << std::endl;
        return false;
    }

    if (!exceptionCaught) {
        // This situation is acceptable.
        std::cerr << test_name << ": Not triggering exception? Wow."
                  << std::endl;
        expected_state.erase(expected_state.begin());
    }

    DizzyCompare::be_good();
    auto replica_state = deheapify(pq_replica);

    if (!compare_state(expected_state, replica_state)) {
        std::cout << test_name
                  << " Failed. Queue state changed after exception."
                  << std::endl;
        return false;
    }

    std::cout << test_name << " Passed." << std::endl;
    return true;
}

// Test 4: Test merge operation when exception is triggered
template <std::uint32_t InftyCount>
bool test4() {
    static_assert(1 <= InftyCount && InftyCount <= 3);
    std::string test_name = "Test4<" + std::to_string(InftyCount) + ">";
    std::cout << "[" << test_name << "] Running..." << std::endl;
    sjtu::priority_queue<ExtNat, DizzyCompare> pq1, pq2;

    DizzyCompare::be_good();
    for (int i = 0; i < InftyCount; i++) {
        pq1.push(ExtNat::infty());
        pq2.push(ExtNat::infty());
        for (int j = 0; j < 1500 / InftyCount; j++) {
            pq1.push(ExtNat::random_range(1, 1110));
            pq2.push(ExtNat::random_range(1, 1110));
        }
    }

    DizzyCompare::be_good();
    auto expected_state1 = deheapify(pq1);
    auto expected_state2 = deheapify(pq2);
    sjtu::priority_queue<ExtNat, DizzyCompare> pq1_replica = pq1;
    sjtu::priority_queue<ExtNat, DizzyCompare> pq2_replica = pq2;

    DizzyCompare::be_bad();
    bool exceptionCaught = false;
    try {
        pq1_replica.merge(pq2_replica);
    } catch (const ComparisonError& e) {
        exceptionCaught = true;
    } catch (...) {
        std::cout << test_name
                  << " Failed. Exception caught but not the expected one."
                  << std::endl;
        return false;
    }

    if (!exceptionCaught) {
        // This situation is acceptable.
        std::cerr << test_name << ": Not triggering exception? Wow."
                  << std::endl;
        expected_state1.insert(expected_state1.end(), expected_state2.begin(),
                               expected_state2.end());
        std::sort(expected_state1.begin(), expected_state1.end(),
                  [](const ExtNat& lhs, const ExtNat& rhs) {
                      return rhs < lhs;
                  });
        expected_state2.clear();
    }

    DizzyCompare::be_good();
    auto replica_state1 = deheapify(pq1_replica);
    auto replica_state2 = deheapify(pq2_replica);

    if (!compare_state(expected_state1, replica_state1) ||
        !compare_state(expected_state2, replica_state2)) {
        std::cout << test_name
                  << " Failed. Queue state changed after exception."
                  << std::endl;
        return false;
    }

    std::cout << test_name << " Passed." << std::endl;
    return true;
}

// Test 5: Verify that after exceptions, queue can perform normal push/pop
// operations
bool test5() {
    std::cout << "[Test5] Running..." << std::endl;
    sjtu::priority_queue<ExtNat, DizzyCompare> pq;

    for (int i = 0; i < 1000; i++) {
        pq.push(ExtNat::random_range(1, 800));
    }

    DizzyCompare::be_good();
    auto expected_state = deheapify(pq);
    sjtu::priority_queue<ExtNat, DizzyCompare> pq_rebuilt = pq;

    DizzyCompare::be_bad();
    bool exceptionCaught = false;
    try {
        pq_rebuilt.push(ExtNat::infty());
    } catch (const ComparisonError& e) {
        exceptionCaught = true;
    } catch (...) {
        std::cout << "Test5: Exception caught but not the expected one."
                  << std::endl;
        return false;
    }
    if (!exceptionCaught) {
        std::cerr << "Test5: Not triggering exception? Wow." << std::endl;
        expected_state.insert(expected_state.begin(), ExtNat::infty());
    }

    DizzyCompare::be_good();  // maybe that infty is still there...
    for (int i = 0; i < 1000; i++) {
        ExtNat element = ExtNat::random_range(1, 1711);
        pq_rebuilt.push(element);
        expected_state.push_back(element);
    }
    std::sort(expected_state.begin(), expected_state.end(),
              [](const ExtNat& lhs, const ExtNat& rhs) {
                  return rhs < lhs;
              });

    auto replica_state = deheapify(pq_rebuilt);
    if (!compare_state(expected_state, replica_state)) {
        std::cout << "Test5: Failed. Queue state changed after recovery."
                  << std::endl;
        return false;
    }
    std::cout << "Test5: Passed." << std::endl;
    return true;
}

int main() {
    std::cout << "# Some information printed through stderr might appear in "
                 "your console.\n"
                 "# The following is what shall be output through stdout.\n"
              << std::flush;
    test1();
    test2<0>(); test2<1>(); test2<2>();
    test3<1>(); test3<2>(); test3<3>();
    test4<1>(); test4<2>(); test4<3>();
    test5();
    return 0;
}