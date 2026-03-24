// This file won't be included in the OnlineJudge test (at least not the basic one).
// Just play with it!
// You can test std::priority_queue or different implementations with these utils to see its performance.
#include "../include/priority_queue.hpp"
#include "test_utils.hpp"

class DangerousData {
public:
  explicit DangerousData(int a): ptr(new int(a)) {}
  DangerousData(const DangerousData& other): ptr(new int(*other.ptr)) {}
  DangerousData(DangerousData&& other) noexcept: ptr(other.ptr) {
    other.ptr = nullptr;
  }
  ~DangerousData() { delete ptr; }
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
  bool operator<(const DangerousData& other) const { return *ptr < *other.ptr; }
private:
  int* ptr;
};

struct DangerousCompare {
  DangerousCompare(): ptr(new int(0)) {}
  DangerousCompare(const DangerousCompare& other): ptr(new int(*other.ptr)) {}
  DangerousCompare(DangerousCompare&& other) noexcept: ptr(other.ptr) {
    other.ptr = nullptr;
  }
  ~DangerousCompare() { delete ptr; }
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

struct CountingComparator {
  static void reset() { count = 0; }
  static int get_count() { return count; }

  bool operator()(int a, int b) const {
    // const-qualifier on the function just requires 'this' pointer to be a const pointer.
    // But the static field does not belong to the object, so it's still mutable.
    count++;
    return a < b;
  }
private:
  static int count;
};
int CountingComparator::count = 0;

int rand_int() {
  static int seed = 1'235'678;
  seed = (seed * 137 + 61'415'021) % 1'000'000'021;
  return seed;
}






bool TestPushComplexity1() {
  // 0 to N
  sjtu::priority_queue<int, CountingComparator> pq;
  auto prepare = [&pq](int n) {
    pq.clear();
  };
  auto test = [&pq](int n) {
    CountingComparator::reset();
    for (int i = 0; i < n; ++i) {
      pq.push(rand_int());
    }
    return CountingComparator::get_count();
  };
  return test::test_complexity("PushTest-1", test::Complexity::O_N_LOG_N, prepare, test);
}

bool TestPushComplexity2() {
  // N to 2N
  sjtu::priority_queue<int, CountingComparator> pq;
  auto prepare = [&pq](int n) {
    pq.clear();
    for (int i = 0; i < n; ++i) {
      pq.push(rand_int());
    }
  };
  auto test = [&pq](int n) {
    CountingComparator::reset();
    for (int i = 0; i < n; ++i) {
      pq.push(rand_int());
    }
    return CountingComparator::get_count();
  };
  return test::test_complexity("PushTest-2", test::Complexity::O_N_LOG_N, prepare, test);
}

bool TestMergeComplexity1() {
  // N + N
  sjtu::priority_queue<int, CountingComparator> pq1, pq2;
  auto prepare = [&pq1, &pq2](int n) {
    // reset priority queues
    pq1.clear();
    pq2.clear();
    for (int i = 0; i < n; ++i) {
      pq1.push(rand_int());
      pq2.push(rand_int());
    }
  };
  auto test = [&pq1, &pq2](int n) {
    CountingComparator::reset();
    pq1.merge(pq2);
    return CountingComparator::get_count();
  };
  return test::test_complexity("MergeTest-1", test::Complexity::O_LOG_N, prepare, test);
}

bool TestPopComplexity1() {
  // N to 0
  sjtu::priority_queue<int, CountingComparator> pq;
  auto prepare = [&pq](int n) {
    pq.clear();
    for (int i = 0; i < n; ++i) {
      pq.push(rand_int());
    }
  };
  auto test = [&pq](int n) {
    CountingComparator::reset();
    while (!pq.empty()) {
      pq.pop();
    }
    return CountingComparator::get_count();
  };
  return test::test_complexity("PopTest-1", test::Complexity::O_N_LOG_N, prepare, test);
}

bool TestPopComplexity2() {
  // 2N to N
  sjtu::priority_queue<int, CountingComparator> pq;
  auto prepare = [&pq](int n) {
    pq.clear();
    for (int i = 0; i < 2 * n; ++i) {
      pq.push(rand_int());
    }
  };
  auto test = [&pq](int n) {
    CountingComparator::reset();
    for (int i = 0; i < n; ++i) {
      pq.pop();
    }
    return CountingComparator::get_count();
  };
  return test::test_complexity("PopTest-2", test::Complexity::O_N_LOG_N, prepare, test);
}

bool TestPushAndPopComplexity() {
  // Dijkstra?
  sjtu::priority_queue<int, CountingComparator> pq;
  std::vector<int> opers;
  auto prepare = [&pq, &opers](int n) {
    pq.clear();
    opers.clear();
    opers.reserve(2 * n);
    int push_cnt = 0;
    int pop_cnt = 0;
    auto decide_push = [push_cnt, pop_cnt, n] {
      if (push_cnt == n) return false;
      if (pop_cnt == n) return true; // ???
      if (push_cnt <= pop_cnt) return true; // must push
      return rand_int() % 5 <= 2;
    };
    for (int i = 0; i < 2 * n; ++i) {
      if (decide_push()) {
        ++push_cnt;
        opers.push_back(1);
      } else {
        ++pop_cnt;
        opers.push_back(0);
      }
    }
  };
  auto test = [&pq, &opers](int n) {
    CountingComparator::reset();
    for (int op: opers) {
      if (op == 1) {
        pq.push(rand_int());
      } else {
        pq.pop();
      }
    }
    return CountingComparator::get_count();
  };
  return test::test_complexity("PushAndPopTest", test::Complexity::O_N_LOG_N, prepare, test);
}

bool TestMergeComplexity2() {
  // 1 * N -> N
  // T(n) = 2T(n/2) + O(log n); T(n) = O(n)
  std::vector<sjtu::priority_queue<int, CountingComparator>> pq_list;
  auto prepare = [&pq_list](int n) {
    pq_list.clear();
    for (int i = 0; i < n; ++i) {
      sjtu::priority_queue<int, CountingComparator> pq;
      pq.push(rand_int());
      pq_list.emplace_back(std::move(pq));
    }
  };
  auto test = [&pq_list](int) {
    CountingComparator::reset();

    while (pq_list.size() > 1) {
      auto size = pq_list.size();
      for (int i = 0; i < size / 2; ++i) {
        pq_list[i].merge(pq_list[size - i - 1]);
      }
      if (size % 2 == 1) {
        pq_list[0].merge(pq_list[size / 2]);
      }
      pq_list.resize(size / 2);
    }

    return CountingComparator::get_count();
  };
  return test::test_complexity("MergeTest-2", test::Complexity::O_N, prepare, test);
}

bool TestClearComplexity() {
  // N to 0
  sjtu::priority_queue<DangerousData, DangerousCompare> pq;
  auto prepare = [&pq](int n) {
    pq.clear();
    for (int i = 0; i < n; ++i) {
      pq.push(DangerousData(rand_int()));
    }
  };
  auto test = [&pq](int n) {
    pq.clear();
  };
  return test::test_complexity("ClearTest", test::Complexity::O_N, prepare, test);
}

int main() {
  std::cout <<
    "# The content of this answer file doesn't matter.\n"
    "# Here's the output of a certain implementation (fib heap).\n"
    "Estimating algorithm complexity... Just for fun."
  << std::endl;
  TestPushComplexity1();
  TestPushComplexity2();
  TestPopComplexity1();
  TestPopComplexity2();
  TestPushAndPopComplexity();
  TestMergeComplexity1();
  TestMergeComplexity2();
  TestClearComplexity();
  return 0;
}