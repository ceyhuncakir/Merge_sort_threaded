#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <thread>
#include <future>
#include <algorithm>
#include "json.hpp"
#include <fstream>

using namespace std::chrono;
using jsonf = nlohmann::json;

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

// de functie die splits maakt aan de hand van de hoeveelheid threads die je gebruikt
std::vector<std::vector<int>> get_splits(std::vector<int> data, int threads) {

  // we vinden de midden punt van het lijst
  std::size_t const middle = data.size() / 2;
  // de linker deel van de split
  std::vector<int> first(data.begin(), data.begin() + middle);
  // de rechter deel van de split
  std::vector<int> second(data.begin() + middle, data.end());

  std::vector<std::vector<int>> split_data = {first, second};

  while(split_data.size() < threads) {
    std::vector<std::vector<int>> temp = {};
    for(int i = 0; i < split_data.size(); i++) {
      std::size_t const middle = split_data[i].size() / 2;
      std::vector<int> first(split_data[i].begin(), split_data[i].begin() + middle);
      std::vector<int> second(split_data[i].begin() + middle, split_data[i].end());

      temp.push_back(first);
      temp.push_back(second);
    }
    split_data = temp;
  }

  return split_data;
}


// functie de de threading van de merge sort doet
std::vector<int> threaded_merge_sort(std::vector<int> data, int max_threads) {
  // de futures die opgeslagen worden
  std::vector<std::future<std::vector<int>>> futures = {};
  // de output van de threading
  std::vector<std::vector<int>> threading_output = {};

  // er word gekeken of de max_threads wat binnen komt groter is dan 1
  if(max_threads > 1) {
    std::vector<std::vector<int>> splits = get_splits(data, max_threads);

    // voor de hoeveelheid splits die er zijn worden er threads aangemaakt
    for(int i = 0; i < splits.size(); i++) {
      futures.push_back(std::async(recursive_merge_sort, std::ref(splits[i])));
    }

    // we loopen over de threads om elke uitkomst te krijgen. die uitkomsten worden weer naar de threading_output gestuurd.
    for(int i = 0; i < futures.size(); i++) {
      threading_output.push_back(futures[i].get());
    }
  } else {
    // als de max_threads wat binnen komt niet groter is dan 1 dan voert die gewoon de recursieve merge sort uit (de default)
    return recursive_merge_sort(data);
  }

  int i = 0;
  // we kijken of de threading output size groter is dan 1
  while(threading_output.size() > 1) {
    // voor elke threading output sorteren we ze op de juiste manier voor de left en de right
    threading_output[i] = merge_sort(threading_output[i], threading_output[i + 1]);
    // we verwijderen die output zodat het steeds meer terug komt naar een gesorteerde lijst
    threading_output.erase(threading_output.begin() + (i + 1));

    // uiteindelijk word de index verhoog als de index kleiner is dan de threading output size - 2
    if(i < threading_output.size() - 2) {
      i += 1;
    } else {
      i = 0;
    }
  }

  // de sorteerde lijst word teruggestuurd
  return threading_output[0];
}


int main() {
  jsonf jsonfile;

  std::vector<int> data(1000);
  std::generate(data.begin(), data.end(), std::rand);

  // de runs waar threads worden gebruikt of niet

  auto start_01 = high_resolution_clock::now();
  std::vector<int> sorted_array_without_threads = threaded_merge_sort(data, 0);
  auto stop_01 = high_resolution_clock::now();

  auto duration_without_threads = duration_cast<microseconds>(stop_01 - start_01);

  auto start_02 = high_resolution_clock::now();
  std::vector<int> sorted_array_with_2_threads = threaded_merge_sort(data, 2);
  auto stop_02 = high_resolution_clock::now();

  auto duration_with_2_threads = duration_cast<microseconds>(stop_02 - start_02);

  auto start_03 = high_resolution_clock::now();
  std::vector<int> sorted_array_with_4_threads = threaded_merge_sort(data, 4);
  auto stop_03 = high_resolution_clock::now();

  auto duration_with_4_threads = duration_cast<microseconds>(stop_03 - start_03);

  auto start_04 = high_resolution_clock::now();
  std::vector<int> sorted_array_with_8_threads = threaded_merge_sort(data, 8);
  auto stop_04 = high_resolution_clock::now();

  auto duration_with_8_threads = duration_cast<microseconds>(stop_04 - start_04);


  // de json structuur waar de data in word gestoppt
  jsonfile = {
    {"merge_sort", {
      {"thread_1", duration_without_threads.count()},
      {"thread_2", duration_with_2_threads.count()},
      {"thread_4", duration_with_4_threads.count()},
      {"thread_8", duration_with_8_threads.count()},
    }}
  };

  std::ofstream file("data/data.json");
  // we schrijven de json structuur naar de file
  file << jsonfile;

  std::cout << std::endl;
  std::cout << "array with the size of " << data.size() << " elements" << std::endl;
  std::cout << std::endl;
  std::cout << "merge_sort without threads: " << duration_without_threads.count() << " microseconds" << std::endl;
  std::cout << "merge_sort with 2 threads: " << duration_with_2_threads.count() << " microseconds" << std::endl;
  std::cout << "merge_sort with 4 threads: " << duration_with_4_threads.count() << " microseconds" << std::endl;
  std::cout << "merge_sort with 8 threads: " << duration_with_8_threads.count() << " microseconds" << std::endl;

  return 0;
}
