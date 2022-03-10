#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <thread>

using namespace std::chrono;

std::vector<int> merge_sort(std::vector<int> array_01, std::vector<int> array_02) {

  if(array_01.size() == 0 && array_02.size() == 0) {
    return {};
  } else {
    if(array_01.size() == 0) {

      // head van array_02
      int array_02_head = array_02[0];
      // tail van array_02
      std::vector<int> array_02_tail = {array_02.begin() + 1, array_02.end()};
      // sorted lijst van array_01 en array_02_tail
      std::vector<int> sorted = merge_sort(array_01, array_02_tail);

      // output is array_02_head + sorted
      std::vector<int> output = {};

      output.emplace_back(array_02_head);

      output.insert(output.end(), sorted.begin(), sorted.end());

      // returning output
      return output;
    } else if(array_02.size() == 0) {

      // head van array_01
      int array_01_head = array_01[0];
      std::vector<int> array_01_tail = {array_01.begin() + 1, array_01.end()};

      std::vector<int> sorted = merge_sort(array_01_tail, array_02);
      std::vector<int> output = {};

      output.emplace_back(array_01_head);

      output.insert(output.end(), sorted.begin(), sorted.end());


      // returning output
      return output;
    } else {
      // head van array_01
      int array_01_head = array_01[0];
      // tail van array_01
      std::vector<int> array_01_tail = {array_01.begin() + 1, array_01.end()};
      // head van array_02
      int array_02_head = array_02[0];
      // tail van array_02
      std::vector<int> array_02_tail = {array_02.begin() + 1, array_02.end()};
      // sorted lijst van array_01 en array_02_tail

      if(array_01_head < array_02_head) {
        std::vector<int> output_01 = {};
        std::vector<int> sorted_01 = merge_sort(array_01_tail, array_02);
        output_01.emplace_back(array_01_head);
        output_01.insert(output_01.end(), sorted_01.begin(), sorted_01.end());

        return output_01;
      } else {
        std::vector<int> output_02 = {};
        std::vector<int> sorted_02 = merge_sort(array_01, array_02_tail);
        output_02.emplace_back(array_02_head);
        output_02.insert(output_02.end(), sorted_02.begin(), sorted_02.end());

        return output_02;
      }
    }
  }
}

std::vector<int> recursive_merge_sort(std::vector<int> data) {
  if(data.size() == 1) {
    return data;
  } else {

    std::size_t const middle = data.size() / 2;
    std::vector<int> first(data.begin(), data.begin() + middle);
    std::vector<int> second(data.begin() + middle, data.end());

    return merge_sort(recursive_merge_sort(first), recursive_merge_sort(second));
  }
}

std::vector<std::vector<int>> get_splits(std::vector<int> data, int threads) {

  std::vector<std::vector<int>> split_data = {};

  for(int i = 0; i < threads; i++) {
    split_data.push_back({data.begin() + i, data.end() + threads});
  }

  return split_data;
}

std::vector<int> threaded_merge_sort(std::vector<int> data, int max_threads) {
  std::vector<int> output = {};

  if(max_threads > 1) {
    std::vector<std::vector<int>> splits = get_splits(data, max_threads);
    std::vector<std::thread> threads;

    for(int i = 0; i < splits.size(); i++) {
      threads.emplace_back(std::thread(recursive_merge_sort, std::ref(splits[i])));
    }
    for(auto& t1 : threads) {
      t1.join();
    }
  } else {
    return recursive_merge_sort(data);
  }
  // we returnen de output van de threads
  return output;
}

// testen of

int main() {
  std::vector<int> data = {10000};
  std::generate(data.begin(), data.end(), std::rand());
  std::vector<int> sorted_array;

  auto start = high_resolution_clock::now();
  sorted_array = threaded_merge_sort(data, 4);
  auto stop = high_resolution_clock::now();

  auto duration = duration_cast<microseconds>(stop - start);

  for(auto e : sorted_array) {
    std::cout << e << " | ";
  }

  std::cout << "Time taken by merge sort algoritme: " << duration.count() << " microseconds" << std::endl;

  return 0;
}
