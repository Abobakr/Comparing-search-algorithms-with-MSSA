#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <ctime>
#include <cmath>
#include <utility>
#include <numeric>

using namespace std;
using namespace std::chrono;

int binarySearch(const vector<double> &arr, double target)
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
            if (arr[mid] < target)
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

int interpolationSearch(const vector<double> &arr, double target)
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
            int pos = low + (high - low) / (arr[high] - arr[low]) * (target - arr[low]);
            if (arr[pos] == target)
                  return pos;
            if (arr[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;
      }
      return -1;
}

int MSSA_Binary(const vector<double> &arr, double target, double key, double d)
{
      int left = target / key - d - 1;
      int right = target / key + d - 1;

      if (left < 0)
            left = 0;
      if (right > arr.size() - 1)
            right = arr.size() - 1;

      while (left <= right)
      {
            int mid = left + (right - left) / 2;
            if (arr[mid] == target)
            {
                  return mid;
            }
            if (arr[mid] < target)
            {
                  left = mid + 1;
            }
            else
            {
                  right = mid - 1;
            }
      }
      return -1;
}

int MSSA_Interpolation(const vector<double> &arr, double target, double key, double d)
{
      int left = target / key - d - 1;
      int right = target / key + d - 1;

      if (left < 0)
            left = 0;
      if (right > arr.size() - 1)
            right = arr.size() - 1;

      while (left <= right && target >= arr[left] && target <= arr[right])
      {
            if (left == right)
            {
                  if (arr[left] == target)
                        return left;
                  return -1;
            }
            int pos = left + (right - left) / (arr[right] - arr[left]) * (target - arr[left]);
            if (arr[pos] == target)
                  return pos;
            if (arr[pos] < target)
                  left = pos + 1;
            else
                  right = pos - 1;
      }
      return -1;
}

pair<double, double> classic_MSSKCA(const vector<double> &X)
{
      const unsigned N = X.size();
      double d = 0;
      double key = X[0];
      unsigned ii = 0, jj = 0;
      unsigned Xi = 0, Xj = 0;

      cout << "-------------------------------------------------" << endl;
      cout << "MSSKCA, Number of elements = " << N << endl;
      cout << "-------------------------------------------------" << endl;

      auto start = high_resolution_clock::now();
      for (unsigned i = 0; i < N - 1; ++i)
      {
            if (i % (N / 10) == 0)
            {
                  cout << "MSSKCA, " << (double)i / N * 100 << "% is done" << endl;
            }
            for (unsigned j = i + 1; j < N; ++j)
            {
                  double xi_over_i = X[i] / (i + 1);
                  double xj_over_j = X[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        double dd = (i * X[j] - j * X[i] + X[j] - X[i]) / (X[i] + X[j]);
                        if (dd > d)
                        {
                              key = X[i] / (i + 1 - dd);
                              d = dd;
                              ii = i, jj = j, Xi = X[i], Xj = X[j];
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        double dd = (j * X[i] - i * X[j] + X[i] - X[j]) / (X[i] + X[j]);
                        if (dd > d)
                        {
                              key = X[j] / (j + 1 - dd);
                              d = dd;
                              ii = i, jj = j, Xi = X[i], Xj = X[j];
                        }
                  }
            }
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "MSSKCA, 100% is done" << endl;
      cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
      cout << "MSSKCA, Calculated d = " << d << endl;
      cout << "MSSKCA, Calculated key = " << key << endl;
      cout << "MSSKCA, i = " << ii << " X[i] = " << Xi << endl;
      cout << "MSSKCA, j = " << jj << " X[j] = " << Xj << endl;

      return {key, d};
}

int main()
{
      const unsigned N = 10'000;       // Total number of elements
      const unsigned SEARCH_COUNT = N; // Number of searches to perform for timing

      random_device rd;
      mt19937 gen(rd());

      // Test 1: Normal Distribution
      cout << "Test 1: Normal Distribution (Mean: " << N / 2 << ", Std Dev:" << N / 10 << ")" << endl;
      cout << "Generating normally distributed numbers..." << endl;
      normal_distribution<> normal_dist(N / 2, N / 10);

      vector<double> data_normal(N);
      for (unsigned i = 0; i < N; ++i)
      {
            data_normal[i] = (double)(normal_dist(gen));
      }

      cout << "Sorting the data..." << endl;
      sort(data_normal.begin(), data_normal.end());

      // cout << "Generating search targets..." << endl;
      // vector<double> targets_normal(SEARCH_COUNT);
      // for (unsigned i = 0; i < SEARCH_COUNT; ++i)
      // {
      //       targets_normal[i] = (double)(normal_dist(gen));
      // }
      cout << "Using the same normal data as search targets..." << endl;
      vector<double> targets_normal = data_normal;

      cout << "Performing binary searches..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            binarySearch(data_normal, target);
      }
      auto end = high_resolution_clock::now();
      duration<double> binaryTime_normal = end - start;

      cout << "Performing Classic MSSKCA for key and d ..." << endl;
      auto [key_normal, d_normal] = classic_MSSKCA(data_normal);
      cout << "Performing MSSA Binary searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            MSSA_Binary(data_normal, target, key_normal, d_normal);
      }

      end = high_resolution_clock::now();
      duration<double> MSSA_Time_normal = end - start;

      cout << "Outputting results for Normal Distribution..." << endl;
      cout << "Binary Search Time: " << binaryTime_normal.count() << " seconds" << endl;
      cout << "MSSA Binary Search Time: " << MSSA_Time_normal.count() << " seconds" << endl;
      cout << endl;

      // Test 2: Uniform Distribution
      cout << "Test 2: Uniform Distribution (Min: 0, Max: " << N << ")" << endl;
      cout << "Generating uniformly distributed numbers..." << endl;
      uniform_real_distribution<> uniform_dist(0, N);

      vector<double> data_uniform(N);
      for (unsigned i = 0; i < N; ++i)
      {
            data_uniform[i] = (double)uniform_dist(gen);
      }

      cout << "Sorting the data..." << endl;
      sort(data_uniform.begin(), data_uniform.end());

      // cout << "Generating search targets..." << endl;
      // vector<double> targets_uniform(SEARCH_COUNT);
      // for (unsigned i = 0; i < SEARCH_COUNT; ++i)
      // {
      //       targets_uniform[i] = uniform_dist(gen);
      // }
      cout << "Using the same uniform data as search targets..." << endl;
      vector<double> targets_uniform = data_uniform;

      cout << "Performing interpolation searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_uniform)
      {
            interpolationSearch(data_uniform, target);
      }
      end = high_resolution_clock::now();
      duration<double> interpolationTime_uniform = end - start;

      cout << "Performing Classic MSSKCA for key and d ..." << endl;
      auto [key_uniform, d_uniform] = classic_MSSKCA(data_uniform);
      cout << "Performing MSSA Interpolation searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            MSSA_Interpolation(data_uniform, target, key_uniform, d_uniform);
      }

      end = high_resolution_clock::now();
      duration<double> MSSA_Time_uniform = end - start;

      cout << "Outputting results for Uniform Distribution..." << endl;
      cout << "Interpolation Search Time: " << interpolationTime_uniform.count() << " seconds" << endl;
      cout << "MSSA Interpolation Search Time: " << MSSA_Time_uniform.count() << " seconds" << endl;
      cout << endl;
      return 0;
}