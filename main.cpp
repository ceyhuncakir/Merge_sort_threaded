#include <string>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono;

template <typename T>
std::vector<T> merge_sort(std::vector<T> array_01, std::vector<T> array_02) {

  if(array_01.size() == 0 && array_02.size() == 0) {
    return {};
  } else {
    if(array_01.size() == 0) {

      // head van array_02
      std::vector<T> array_02_head = {array_02.begin(), array_02.end() - (array_02.size() - 1)};
      // tail van array_02
      std::vector<T> array_02_tail = {array_02.begin() + 1, array_02.end()};
      // sorted lijst van array_01 en array_02_tail
      std::vector<T> sorted = merge_sort(array_01, array_02_tail);

      // output is array_02_head + sorted
      std::vector<T> output = {};
      // we reserve voor de output vector de size van array_02_head en de sorted list size
      output.reserve(array_02_head.size() + sorted.size());

      output.emplace_back(array_02_head[0]);

      for(auto e : sorted) {
        output.emplace_back(e);
      }

      // returning output
      return output;
    } else if(array_02.size() == 0) {

      // head van array_01
      std::vector<T> array_01_head = {array_01.begin(), array_01.end() - (array_01.size() - 1)};
      // tail van array_01
      std::vector<T> array_01_tail = {array_01.begin() + 1, array_01.end()};
      // sorted lijst van array_02 en array_01_tail
      std::vector<T> sorted = merge_sort(array_01_tail, array_02);

      // output is array_01_head + sorted
      std::vector<T> output = {};
      // we reserve voor de output vector de size van array_01_head en de sorted list size
      output.reserve(array_01_head.size() + sorted.size());

      output.emplace_back(array_01_head[0]);

      for(auto e : sorted) {
        output.emplace_back(e);
      }

      // returning output
      return output;
    } else {
      // head van array_01
      std::vector<T> array_01_head = {array_01.begin(), array_01.end() - (array_01.size() - 1)};
      // tail van array_01
      std::vector<T> array_01_tail = {array_01.begin() + 1, array_01.end()};
      // sorted lijst van array_02 en array_01_tail
      std::vector<T> sorted_01 = merge_sort(array_01_tail, array_02);

      // head van array_02
      std::vector<T> array_02_head = {array_02.begin(), array_02.end() - (array_02.size() - 1)};
      // tail van array_02
      std::vector<T> array_02_tail = {array_02.begin() + 1, array_02.end()};
      // sorted lijst van array_01 en array_02_tail
      std::vector<T> sorted_02 = merge_sort(array_01, array_02_tail);

      // output is array_01_head + sorted_01
      std::vector<T> output_01 = {};
      // we reserve voor de output vector de size van array_01_head en de sorted list size
      output_01.reserve(array_01_head.size() + sorted_01.size());

      // output is array_02_head + sorted_02
      std::vector<T> output_02 = {};
      // we reserve voor de output vector de size van array_02_head en de sorted list size
      output_02.reserve(array_02_head.size() + sorted_02.size());

      if(array_01_head < array_02_head) {
        output_01.emplace_back(array_01_head[0]);

        for(auto e : sorted_01) {
          output_01.push_back(e);
        }

        return output_01;
      } else {
        output_02.emplace_back(array_02_head[0]);

        for(auto e : sorted_02) {
          output_02.emplace_back(e);
        }

        return output_02;
      }
    }
  }
}

template <typename T>
std::vector<T> recursive_merge_sort(std::vector<T> data) {
  if(data.size() == 1) {
    return data;
  } else {
    std::size_t const middle = data.size() / 2;
    std::vector<T> first(data.begin(), data.begin() + middle);
    std::vector<T> second(data.begin() + middle, data.end());

    std::vector<T> first_output = recursive_merge_sort(first);
    std::vector<T> second_output = recursive_merge_sort(second);

    return merge_sort(first_output, second_output);
  }
}

int main() {
  std::vector<int> data = {75, 83, 40, 24, 42, 63, 23, 22, 34, 58, 65, 100, 30, 57, 90, 14, 46};
  std::vector<int> sorted_array;

  auto start = high_resolution_clock::now();
  sorted_array = recursive_merge_sort(data);
  auto stop = high_resolution_clock::now();

  auto duration = duration_cast<microseconds>(stop - start);

  for(auto e : sorted_array) {
    std::cout << e << " | ";
  }

  std::cout << "Time taken by merge sort algoritme: " << duration.count() << " microseconds" << std::endl;

  return 0;
}
