#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

// Function to perform binary search
int binarySearch(const vector<int> &arr, int target)
{
      int left = 0;
      int right = arr.size() - 1;
      while (left <= right)
      {
            int mid = left + (right - left) / 2;
            if (arr[mid] == target)
            {
                  return mid;
            }
            else if (arr[mid] < target)
            {
                  left = mid + 1;
            }
            else
            {
                  right = mid - 1;
            }
      }
      return -1; // Not found
}

// Function to perform interpolation search
int interpolationSearch(const vector<int> &arr, int target)
{
      int low = 0;
      int high = arr.size() - 1;
      while (low <= high && target >= arr[low] && target <= arr[high])
      {
            if (low == high)
            {
                  if (arr[low] == target)
                        return low;
                  return -1;
            }
            int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (target - arr[low]));
            if (arr[pos] == target)
            {
                  return pos;
            }
            if (arr[pos] < target)
            {
                  low = pos + 1;
            }
            else
            {
                  high = pos - 1;
            }
      }
      return -1; // Not found
}

int main()
{
      const int N = 10'000'000;           // 10 million elements
      const int SEARCH_COUNT = 1'000'000; // Number of searches to perform for timing

      random_device rd;
      mt19937 gen(rd());

      // Test 1: Normal Distribution
      cout << "Test 1: Normal Distribution (Mean: 5,000,000, Std Dev: 1,000,000)" << endl;
      cout << "Generating normally distributed numbers..." << endl;
      normal_distribution<> normal_dist(5'000'000, 1'000'000);

      vector<int> data_normal(N);
      for (int i = 0; i < N; ++i)
      {
            data_normal[i] = static_cast<int>(normal_dist(gen));
      }

      cout << "Sorting the data..." << endl;
      sort(data_normal.begin(), data_normal.end());

      cout << "Generating search targets..." << endl;
      vector<int> targets_normal(SEARCH_COUNT);
      for (int i = 0; i < SEARCH_COUNT; ++i)
      {
            targets_normal[i] = static_cast<int>(normal_dist(gen));
      }

      cout << "Performing binary searches..." << endl;
      auto start = chrono::high_resolution_clock::now();
      for (int target : targets_normal)
      {
            binarySearch(data_normal, target);
      }
      auto end = chrono::high_resolution_clock::now();
      chrono::duration<double> binaryTime_normal = end - start;

      cout << "Performing interpolation searches..." << endl;
      start = chrono::high_resolution_clock::now();
      for (int target : targets_normal)
      {
            interpolationSearch(data_normal, target);
      }
      end = chrono::high_resolution_clock::now();
      chrono::duration<double> interpolationTime_normal = end - start;

      cout << "Outputting results for Normal Distribution..." << endl;
      cout << "Binary Search Time: " << binaryTime_normal.count() << " seconds" << endl;
      cout << "Interpolation Search Time: " << interpolationTime_normal.count() << " seconds" << endl;
      cout << endl;

      // Test 2: Uniform Distribution
      cout << "Test 2: Uniform Distribution (Min: 0, Max: 10,000,000)" << endl;
      cout << "Generating uniformly distributed numbers..." << endl;
      uniform_int_distribution<int> uniform_dist(0, 10'000'000);

      vector<int> data_uniform(N);
      for (int i = 0; i < N; ++i)
      {
            data_uniform[i] = uniform_dist(gen);
      }

      cout << "Sorting the data..." << endl;
      sort(data_uniform.begin(), data_uniform.end());

      cout << "Generating search targets..." << endl;
      vector<int> targets_uniform(SEARCH_COUNT);
      for (int i = 0; i < SEARCH_COUNT; ++i)
      {
            targets_uniform[i] = uniform_dist(gen);
      }

      cout << "Performing binary searches..." << endl;
      start = chrono::high_resolution_clock::now();
      for (int target : targets_uniform)
      {
            binarySearch(data_uniform, target);
      }
      end = chrono::high_resolution_clock::now();
      chrono::duration<double> binaryTime_uniform = end - start;

      cout << "Performing interpolation searches..." << endl;
      start = chrono::high_resolution_clock::now();
      for (int target : targets_uniform)
      {
            interpolationSearch(data_uniform, target);
      }
      end = chrono::high_resolution_clock::now();
      chrono::duration<double> interpolationTime_uniform = end - start;

      cout << "Outputting results for Uniform Distribution..." << endl;
      cout << "Binary Search Time: " << binaryTime_uniform.count() << " seconds" << endl;
      cout << "Interpolation Search Time: " << interpolationTime_uniform.count() << " seconds" << endl;

      return 0;
}