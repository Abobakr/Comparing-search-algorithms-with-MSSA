#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <ctime>
#include <cmath>
#include <utility>
#include <numeric>
#include <functional>
#include <iomanip>

using namespace std;
using namespace chrono;

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
            int pos = low + (double)(high - low) / (arr[high] - arr[low]) * (target - arr[low]);
            if (arr[pos] == target)
                  return pos;
            if (arr[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;
      }
      return -1;
}

int MSSA_Binary(const vector<double> &arr, double target, long double inv_key, int d)
{
      int left = target * inv_key - 1 - d; // OLD: int left = target / key - 1 - d;
      int right = left + d + d;
      int last = arr.size() - 1;

      if (left < 0)
            left = 0;
      if (right > last)
            right = last;

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

int MSSA_Interpolation(const vector<double> &arr, double target, long double inv_key, int d)
{
      int left = target * inv_key - 1 - d; // OLD: int left = target / key - 1 - d;
      int right = left + d + d;
      int last = arr.size() - 1;

      if (left < 0)
            left = 0;
      if (right > last)
            right = last;

      while (left <= right && target >= arr[left] && target <= arr[right])
      {
            if (left == right)
            {
                  if (arr[left] == target)
                        return left;
                  return -1;
            }
            int pos = left + (double)(right - left) / (arr[right] - arr[left]) * (target - arr[left]);
            if (arr[pos] == target)
                  return pos;
            if (arr[pos] < target)
                  left = pos + 1;
            else
                  right = pos - 1;
      }
      return -1;
}

pair<long double, int> classic_MSSKCA(const vector<double> &X)
{
      auto start = high_resolution_clock::now();

      const int N = X.size();
      long double d = 0;
      long double key = X[0];
      for (int i = 0; i < N - 1; i++)
      {
            if (i % (N / 10) == 0)
            {
                  cout << "- Classic MSSKCA, " << i * 1.0 / N * 100 << "% is done" << endl;
            }
            for (int j = i + 1; j < N; j++)
            {
                  long double xi_over_i = X[i] / (i + 1);
                  long double xj_over_j = X[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        long double dd = (i * X[j] - j * X[i] + X[j] - X[i]) / (X[i] + X[j]);
                        if (dd > d)
                        {
                              key = X[j] / (j + 1 + dd);
                              d = dd;
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        long double dd = (j * X[i] - i * X[j] + X[i] - X[j]) / (X[i] + X[j]);
                        if (dd > d)
                        {
                              key = X[i] / (i + 1 + dd); // the denominator is always + to avoid a possible NaN case
                              d = dd;
                        }
                  }
            }
      }
      d = ceil(d);
      long double inv_key = 1.0L / key;

      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Classic MSSKCA, 100% is done" << endl;

      // optional output and test part
      cout << "- Classic MSSKCA ended in: " << elapsed.count() << " seconds" << endl;
      cout << "- Classic MSSKCA results: " << " key = " << key << ", d = " << d << endl;
      cout << "- Classic MSSKCA: Performing MSSA with Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < X.size(); i++)
      {
            if (-1 == MSSA_Binary(X, X[i], inv_key, d))
            {
                  printf("! Error: MSSA with Binary missed X[%d] = %d\n", i, X[i]);
                  break;
            }
      }

      return {inv_key, d};
}

pair<long double, int> fast_MSSKCA(const vector<double> &X)
{
      long double key = X[0] / (0 + 1.0); // initial key
      long double d = 0.0;                // d in ideal case
      vector<int> game_changer = {0};     // i = 0, the first element
      int counter = 0;
      while (counter < game_changer.size())
      {
            int i = game_changer[counter];
            int gc = -1;
            for (int j = 0; j < X.size(); j++)
            {
                  if (i == j)
                        continue;

                  long double xi_over_i = X[i] / (i + 1);
                  long double xj_over_j = X[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        long double dd = (i * X[j] - j * X[i] + X[j] - X[i]) / (X[i] + X[j]);
                        if (dd > d)
                        {
                              key = X[j] / (j + 1 + dd);
                              d = dd;
                              gc = j;
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        long double dd = (j * X[i] - i * X[j] + X[i] - X[j]) / (X[i] + X[j]);
                        if (dd > d)
                        {
                              key = X[i] / (i + 1 + dd); // the denominator is always + to avoid a possible NaN case
                              d = dd;
                              gc = j;
                        }
                  }
            }
            if (gc != -1)
                  game_changer.push_back(gc); // the largest dd is the game changer
            counter++;
      }

      d = ceil(d);
      long double inv_key = 1.0L / key;

      // optional output and test part
      cout << "- Fast MSSKCA results: " << " key = " << key << ", d = " << d << endl;
      cout << "- Fast MSSKCA: performing MSSA with Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < X.size(); i++)
      {
            if (-1 == MSSA_Binary(X, X[i], inv_key, d))
            {
                  printf("! Error: MSSA with Binary missed X[%d] = %d\n", i, X[i]);
                  break;
            }
      }

      return {inv_key, d};
}

int main()
{
      const int N = 1'000'000;
      cout << fixed << setprecision(3) << endl;
      cout << "+ Total number of elements: " << N << endl;
      const int SEARCH_COUNT = N; // Number of searches to perform for timing

      random_device rd;
      mt19937 gen(rd());

      cout << endl;
      double mean = N * 5;    // helps to manipulate range of numbers
      double std_dev = N * 2; // mean distance around the mean
      cout << "Test 1: Normal Distribution (Mean: " << mean << ", Std Dev:" << std_dev << ")" << endl;
      cout << "**************************************************************************" << endl;

      cout << "+ Generating normally distributed numbers..." << endl;
      normal_distribution<> normal_dist(mean, std_dev);
      vector<double> data_normal(N);
      for (int i = 0; i < N; i++)
      {
            data_normal[i] = abs(normal_dist(gen));
      }

      cout << "+ Sorting the data..." << endl;
      sort(data_normal.begin(), data_normal.end());
      cout << "+ Data range: [" << data_normal.front() << ", " << data_normal.back() << "]" << endl;

      //! Warning: The Classic MSSKCA is too slow for large N
      if (N <= 100'000)
      {
            cout << "+ Performing Classic MSSKCA for key and d ..." << endl;
            auto [inv_key_normal_classic, d_normal_classic] = classic_MSSKCA(data_normal);
      }

      cout << "+ Performing Fast MSSKCA for key and d ..." << endl;
      auto [inv_key_normal_fast, d_normal_fast] = fast_MSSKCA(data_normal);

      // cout << "+ Generating search targets..." << endl;
      // vector<double> targets_normal(SEARCH_COUNT);
      // for (int i = 0; i < SEARCH_COUNT; i++)
      // {
      //       targets_normal[i] = abs(normal_dist(gen));
      // }
      cout << "+ Using the same data as search targets..." << endl;
      vector<double> targets_normal = data_normal;

      cout << "+ Performing binary searches..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            binarySearch(data_normal, target);
      }
      auto end = high_resolution_clock::now();
      duration<double> binary_time = end - start;

      cout << "+ Performing interpolation searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            interpolationSearch(data_normal, target);
      }
      end = high_resolution_clock::now();
      duration<double> interpolation_time = end - start;

      cout << "+ Performing MSSA with Binary searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            MSSA_Binary(data_normal, target, inv_key_normal_fast, d_normal_fast);
      }
      end = high_resolution_clock::now();
      duration<double> MSSA_binary_time = end - start;

      cout << "+ Performing MSSA with Interpolation searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_normal)
      {
            MSSA_Interpolation(data_normal, target, inv_key_normal_fast, d_normal_fast);
      }
      end = high_resolution_clock::now();
      duration<double> MSSA_interpolation_time = end - start;

      cout << "-------------------------------------------" << endl;
      cout << "Outputting results for Normal Distribution" << endl;
      cout << "-------------------------------------------" << endl;

      cout << "+ Binary Search Time: " << binary_time.count() << " seconds" << endl;
      cout << "+ Interpolation Search Time: " << interpolation_time.count() << " seconds" << endl;
      cout << "+ MSSA with Binary Search Time: " << MSSA_binary_time.count() << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search Time: " << MSSA_interpolation_time.count() << " seconds" << endl;

      cout << endl;

      int max = N * 1;
      cout << "Test 2: Uniform Distribution (Min: 0, Max: " << max << ")" << endl;
      cout << "**************************************************************************" << endl;

      cout << "+ Generating uniformly distributed numbers..." << endl;
      uniform_real_distribution<> uniform_dist(0, max);
      vector<double> data_uniform(N);
      for (int i = 0; i < N; i++)
      {
            data_uniform[i] = uniform_dist(gen);
      }

      cout << "+ Sorting the data..." << endl;
      sort(data_uniform.begin(), data_uniform.end());
      cout << "+ Data range: [" << data_uniform.front() << ", " << data_uniform.back() << "]" << endl;

      //! Warning: The Classic MSSKCA is too slow for large N
      if (N <= 100'000)
      {
            cout << "+ Performing Classic MSSKCA for key and d ..." << endl;
            auto [inv_key_uniform_classic, d_uniform_classic] = classic_MSSKCA(data_uniform);
      }

      cout << "+ Performing Fast MSSKCA for key and d ..." << endl;
      auto [inv_key_uniform_fast, d_uniform_fast] = fast_MSSKCA(data_uniform);

      // cout << "+ Generating search targets..." << endl;
      // vector<double> targets_uniform(SEARCH_COUNT);
      // for (int i = 0; i < SEARCH_COUNT; i++)
      // {
      //       targets_uniform[i] = uniform_dist(gen);
      // }

      cout << "+ Using the same data as search targets..." << endl;
      vector<double> targets_uniform = data_uniform;

      cout << "+ Performing binary searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_uniform)
      {
            binarySearch(data_uniform, target);
      }
      end = high_resolution_clock::now();
      binary_time = end - start;

      cout << "+ Performing interpolation searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_uniform)
      {
            interpolationSearch(data_uniform, target);
      }
      end = high_resolution_clock::now();
      interpolation_time = end - start;

      cout << "+ Performing MSSA with Binary searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_uniform)
      {
            MSSA_Binary(data_uniform, target, inv_key_uniform_fast, d_uniform_fast);
      }
      end = high_resolution_clock::now();
      MSSA_binary_time = end - start;

      cout << "+ Performing MSSA with Interpolation searches..." << endl;
      start = high_resolution_clock::now();
      for (double target : targets_uniform)
      {
            MSSA_Interpolation(data_uniform, target, inv_key_uniform_fast, d_uniform_fast);
      }
      end = high_resolution_clock::now();
      MSSA_interpolation_time = end - start;

      cout << "-------------------------------------------" << endl;
      cout << "Outputting results for Uniform Distribution" << endl;
      cout << "-------------------------------------------" << endl;

      cout << "+ Binary Search Time: " << binary_time.count() << " seconds" << endl;
      cout << "+ Interpolation Search Time: " << interpolation_time.count() << " seconds" << endl;
      cout << "+ MSSA with Binary Search Time: " << MSSA_binary_time.count() << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search Time: " << MSSA_interpolation_time.count() << " seconds" << endl;

      cout << endl;

      return 0;
}