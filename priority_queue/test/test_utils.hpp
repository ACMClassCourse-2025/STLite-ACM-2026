#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <type_traits>

namespace test {

template <typename T>
struct TimerResult {
  template <typename U>
  TimerResult(std::chrono::duration<double> duration, U&& result)
  : duration_(duration), result_(std::forward<U>(result)) {}

  double duration_ms() const { return duration_.count() * 1000; }
  double duration_sec() const { return duration_.count(); }
  T result() const { return result_; }

private:
  std::chrono::duration<double> duration_;
  T result_;
};

template <>
struct TimerResult<void> {
  explicit TimerResult(std::chrono::duration<double> duration)
  : duration_(duration) {}

  double duration_ms() const { return duration_.count() * 1000; }
  double duration_sec() const { return duration_.count(); }

private:
  std::chrono::duration<double> duration_;
};

// A wrapper function to measure the time cost of a function call.
// Inspired by Unix's `time` command.
template <typename F, typename... Args>
auto timer(F&& f, Args&&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  if constexpr (std::is_void_v<std::invoke_result_t<F, Args...>>) {
    std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);
    return TimerResult<void>(duration);
  } else {
    decltype(auto) result = std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);
    return TimerResult<decltype(result)>(duration, result);
  }
}

enum class Complexity {
  O_1,
  O_LOG_N,
  O_SQRT_N,
  O_N,
  O_N_LOG_N,
  O_N_SQRT_N,
  O_N_SQUARE,
  // O_TOO_LARGE,
  UNKNOWN,
};

std::string to_string(Complexity complexity) {
  switch (complexity) {
    case Complexity::O_1: return "O(1)";
    case Complexity::O_LOG_N: return "O(logn)";
    case Complexity::O_SQRT_N: return "O(n^0.5)";
    case Complexity::O_N: return "O(n)";
    case Complexity::O_N_LOG_N: return "O(nlogn)";
    case Complexity::O_N_SQRT_N: return "O(n^1.5)";
    case Complexity::O_N_SQUARE: return "O(n^2)";
    case Complexity::UNKNOWN: return "Unknown";
  }
  throw std::runtime_error("Invalid Complexity");
}

std::pair<Complexity, double> estimate_complexity(const std::vector<std::pair<double, double>>& vec) {
  std::vector<std::pair<Complexity, std::function<double(double, double)>>>
  models = {
    {Complexity::O_1, [](double, double y) { return y; }},
    {Complexity::O_LOG_N, [](double x, double y) { return y / std::log2(x); }},
    {Complexity::O_SQRT_N, [](double x, double y) { return y / std::sqrt(x); }},
    {Complexity::O_N, [](double x, double y) { return y / x; }},
    {Complexity::O_N_LOG_N, [](double x, double y) { return y / (x * std::log2(x)); }},
    {Complexity::O_N_SQRT_N, [](double x, double y) { return y / (x * std::sqrt(x)); }},
    {Complexity::O_N_SQUARE, [](double x, double y) { return y / (x * x); } }
  };

  Complexity best_model = Complexity::UNKNOWN;
  // Coefficient of Variation
  double min_cv = std::numeric_limits<double>::max();
  double best_k = 0.0;

  // 3. 遍历每个模型进行测试
  for (const auto& model: models) {
    std::vector<double> ratios;
    ratios.reserve(vec.size());

    for (const auto& point : vec) {
      double n = point.first;
      double c = point.second;
      if (n <= 1.0) continue;

      double val = 0;
      try {
        val = model.second(n, c);
      } catch (...) {
        val = std::numeric_limits<double>::infinity();
      }

      if (std::isfinite(val) && val >= 0) {
        ratios.push_back(val);
      }
    }

    if (ratios.size() < 2) continue;

    double sum = std::accumulate(ratios.begin(), ratios.end(), 0.0);
    double mean = sum / ratios.size();

    double variance = 0.0;
    for (double r : ratios) {
      variance += (r - mean) * (r - mean);
    }
    variance /= ratios.size();

    if (mean > 1e-9) {
      double cv = std::sqrt(variance) / mean;
      if (cv < min_cv) {
        min_cv = cv;
        best_model = model.first;
        best_k = mean;
      }
    }
  }

  // raise this threshold to decrease the possibility that Unknown emerges.
  if (min_cv > 0.35) {
    return {Complexity::UNKNOWN, min_cv};
  }

  return {best_model, best_k};
}


std::string format_double(double val) {
  if (val > 1e5 - 1) return std::to_string(static_cast<int>(val));
  if (val < 1e-5) return std::string("0");
  std::ostringstream oss;
  oss << std::setprecision(6) << val;
  return oss.str();
};
std::string string_fill(const std::string& str, int width) {
  auto left_padding = (width - str.size()) / 2;
  auto right_padding = width - str.size() - left_padding;
  return std::string(left_padding, ' ') + str + std::string(right_padding, ' ');
};

// The time complexity of a heap is mainly measured by the number of comparisons.
// Here we test the speed of the heap with the @timer function and @CountingComparator altogether.
template <typename Prepare, typename Test>
bool test_complexity(
  const std::string& test_name,
  Complexity expected_complexity,
  Prepare&& prepare,
  Test&& test) {

  std::cout << "\n\n" << std::flush;

  static_assert(std::is_invocable_v<Prepare, int> && std::is_invocable_v<Test, int>,
    "Prepare and Test must be a callable type that takes an integer as the problem scale argument");

  using cost_t = std::invoke_result_t<Test, int>;

  static_assert(
    std::is_void_v<cost_t> ||
    (std::is_arithmetic_v<cost_t> && std::is_convertible_v<cost_t, double>),
    "Test function shall return void, or an arithmetic value standing for the operation cost");

  // NlogN (with large constants) accepts N = 1e6
  static constexpr int problem_sizes[7] = {3000, 10001, 30002, 100003, 300004, 500005, 1000006};

  struct TestResult {
    int problem_size;
    TimerResult<cost_t> result;
  };
  std::vector<TestResult> test_results;
  std::vector<std::pair<double, double>> data_time;
  std::vector<std::pair<double, double>> data_comp;

  for (int problem_size: problem_sizes) {
    // Prepare.
    std::invoke(std::forward<Prepare>(prepare), problem_size);
    // Test.
    TimerResult<cost_t> timer_result = timer([&test, problem_size] {
      return std::invoke(std::forward<Test>(test), problem_size);
    });
    test_results.emplace_back(TestResult{
      problem_size,
      timer_result
    });
    data_time.emplace_back(problem_size, timer_result.duration_ms());
    if constexpr (!std::is_void_v<cost_t>) {
      data_comp.emplace_back(problem_size, timer_result.result());
    }
  }

  std::cout << "-------------[" << test_name << "] (expected complexity: " << to_string(expected_complexity) << ")\n";

  std::vector<std::string> lines = {
    string_fill("[Problem Size]", 20),
    string_fill("[Time Cost (ms)]", 20),
  };
  for (const auto &test_result: test_results) {
    lines[0] += string_fill(format_double(test_result.problem_size), 10);
    lines[1] += string_fill(format_double(test_result.result.duration_ms()), 10);
  }
  if constexpr (!std::is_void_v<cost_t>) {
    lines.push_back(string_fill("[Operation Cost]", 20));
    for (const auto &test_result: test_results) {
      lines.back() += string_fill(format_double(test_result.result.result()), 10);
    }
  }

  for (const auto &line: lines) {
    std::cout << line << '\n';
  }

  std::cout << "--------------------------------------\n";
  auto [complexity_time, constant_time] = estimate_complexity(data_time);
  std::cout << "Estimated function complexity (from time cost): " << to_string(complexity_time) << '\n';
  std::cout << "Estimated algorithmic constant (from time cost): " << std::to_string(constant_time) << '\n';
  bool verdict = complexity_time <= expected_complexity;
  if constexpr (!std::is_void_v<cost_t>) {
    auto [complexity_comp, constant_comp] = estimate_complexity(data_comp);
    std::cout << "Estimated comparison complexity (from operation cost): " << to_string(complexity_comp) << '\n';
    std::cout << "Estimated algorithmic constant (from operation cost): " << std::to_string(constant_comp) << '\n';
    verdict |= complexity_comp <= expected_complexity;
  }
  std::cout << "--------------------------------------\n";
  std::cout << "Final verdict (with the most optimistic estimation above): " << (verdict ? "Pass" : "Fail") << "\n";
  std::cout << "--------------------------------------\n" << std::flush;

  return verdict;
}
}