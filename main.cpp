#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std::chrono;

template <typename T>
std::vector<T> merge_sort(std::vector<T> array_01, std::vector<T> array_02) {

  if(array_01.size() == 0 && array_02.size() == 0) {
    return {};
  } else {
    if(array_01.size() == 0) {

      // head van array_02
      int array_02_head = array_02[0];
      // tail van array_02
      std::vector<T> array_02_tail = {array_02.begin() + 1, array_02.end()};
      // sorted lijst van array_01 en array_02_tail
      std::vector<T> sorted = merge_sort(array_01, array_02_tail);

      // output is array_02_head + sorted
      std::vector<T> output = {};

      output.emplace_back(array_02_head);

      output.insert(output.end(), sorted.begin(), sorted.end());

      // returning output
      return output;
    } else if(array_02.size() == 0) {

      // head van array_01
      int array_01_head = array_01[0];
      std::vector<int> array_01_tail = {array_01.begin() + 1, array_01.end()};

      std::vector<T> sorted = merge_sort(array_01_tail, array_02);
      std::vector<T> output = {};

      output.emplace_back(array_01_head);

      output.insert(output.end(), sorted.begin(), sorted.end());


      // returning output
      return output;
    } else {
      // head van array_01
      int array_01_head = array_01[0];
      // tail van array_01
      std::vector<T> array_01_tail = {array_01.begin() + 1, array_01.end()};
      // head van array_02
      int array_02_head = array_02[0];
      // tail van array_02
      std::vector<T> array_02_tail = {array_02.begin() + 1, array_02.end()};
      // sorted lijst van array_01 en array_02_tail

      if(array_01_head < array_02_head) {
        std::vector<T> output_01 = {};
        std::vector<T> sorted_01 = merge_sort(array_01_tail, array_02);
        output_01.emplace_back(array_01_head);
        output_01.insert(output_01.end(), sorted_01.begin(), sorted_01.end());

        return output_01;
      } else {
        std::vector<T> output_02 = {};
        std::vector<T> sorted_02 = merge_sort(array_01, array_02_tail);
        output_02.emplace_back(array_02_head);
        output_02.insert(output_02.end(), sorted_02.begin(), sorted_02.end());

        return output_02;
      }
    }
  }
}

void threading(function<void> func, std::thread t) {

  if(t.isjoinable()) {
    t.join()l
  } else {
    t(func);
  }
}

template <typename T>
std::vector<T> recursive_merge_sort(std::vector<T> data, int max_threads) {
  if(data.size() == 1) {
    return data;
  } else {
    // splitsingen bijhouden zodat je weet wanneer je de threads moet aanroepen.
    std::size_t const middle = data.size() / 2;
    std::vector<T> first(data.begin(), data.begin() + middle);
    std::vector<T> second(data.begin() + middle, data.end());


    // std::vector<T> first_output = {};
    // std::vector<T> second_output = {};
    //
    //
    // first_output = recursive_merge_sort(first);
    // second_output = recursive_merge_sort(second);

    return merge_sort(first_output, second_output);
  }
}

int main() {
  std::vector<int> data = {14, 5, 9, 10, 23, 3, 26, 1};
  std::vector<int> sorted_array;

  auto start = high_resolution_clock::now();
  sorted_array = recursive_merge_sort(data, 4);
  auto stop = high_resolution_clock::now();

  auto duration = duration_cast<microseconds>(stop - start);

  for(auto e : sorted_array) {
    std::cout << e << " | ";
  }

  std::cout << "Time taken by merge sort algoritme: " << duration.count() << " microseconds" << std::endl;

  return 0;
}
