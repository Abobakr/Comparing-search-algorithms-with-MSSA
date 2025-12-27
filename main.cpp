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
#include <tuple>

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

int MSSA_Binary(const vector<double> &arr, double target, int pos, int d)
{
      // OLD 1: int left = target / key - 1 - d;
      // OLD 2: int left = target * inv_key - 1 - d;
      // OLD 2: int right = left + d + d;
      int left = pos - d;
      int right = pos + d;
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

int MSSA_Interpolation(const vector<double> &arr, double target, int pos, int d)
{
      // OLD 1: int left = target / key - 1 - d;
      // OLD 2: int left = target * inv_key - 1 - d;
      // OLD 2: int right = left + d + d;
      int left = pos - d;
      int right = pos + d;
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

pair<long double, int> classic_MSSKCA(const vector<double> &y)
{
      auto start = high_resolution_clock::now();

      const int N = y.size();
      long double d = 0;
      long double key = y[0];
      for (int i = 0; i < N - 1; i++)
      {
            if (i % (N / 10) == 0)
            {
                  cout << "- Classic MSSKCA, " << i * 1.0 / N * 100 << "% is done" << endl;
            }
            for (int j = i + 1; j < N; j++)
            {
                  long double xi_over_i = y[i] / (i + 1);
                  long double xj_over_j = y[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        long double dd = (i * y[j] - j * y[i] + y[j] - y[i]) / (y[i] + y[j]);
                        if (dd > d)
                        {
                              key = y[j] / (j + 1 + dd);
                              d = dd;
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        long double dd = (j * y[i] - i * y[j] + y[i] - y[j]) / (y[i] + y[j]);
                        if (dd > d)
                        {
                              key = y[i] / (i + 1 + dd); // the denominator is always + to avoid a possible NaN case
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
      cout << "- Classic MSSKCA results: " << " key = " << key << ", max error = " << d << endl;
      cout << "- Classic MSSKCA: Performing MSSA with Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < y.size(); i++)
      {
            if (-1 == MSSA_Binary(y, y[i], y[i] * inv_key - 1, d))
            {
                  printf("! Error: MSSA with Binary missed y[%d] = %f\n", i, y[i]);
                  break;
            }
      }

      return {inv_key, d};
}

pair<long double, int> fast_MSSKCA(const vector<double> &y)
{
      long double key = y[0] / (0 + 1.0); // initial key
      long double d = 0.0;                // d in ideal case
      vector<int> game_changer = {0};     // i = 0, the first element
      int counter = 0;
      while (counter < game_changer.size())
      {
            int i = game_changer[counter];
            int gc = -1;
            for (int j = 0; j < y.size(); j++)
            {
                  if (i == j)
                        continue;

                  long double xi_over_i = y[i] / (i + 1);
                  long double xj_over_j = y[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        long double dd = (i * y[j] - j * y[i] + y[j] - y[i]) / (y[i] + y[j]);
                        if (dd > d)
                        {
                              key = y[j] / (j + 1 + dd);
                              d = dd;
                              gc = j;
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        long double dd = (j * y[i] - i * y[j] + y[i] - y[j]) / (y[i] + y[j]);
                        if (dd > d)
                        {
                              key = y[i] / (i + 1 + dd); // the denominator is always + to avoid a possible NaN case
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
      cout << "- Fast MSSKCA results: " << " key = " << key << ", max error = " << d << endl;
      cout << "- Fast MSSKCA: performing MSSA with Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < y.size(); i++)
      {
            if (-1 == MSSA_Binary(y, y[i], y[i] * inv_key - 1, d))
            {
                  printf("! Error: MSSA with Binary missed y[%d] = %f\n", i, y[i]);
                  break;
            }
      }

      return {inv_key, d};
}

tuple<long double, long double, int> linearRegression(const vector<double> &y)
{
      int n = y.size();
      if (n == 0)
            return {0.0, 0.0, 0.0};

      double sum_x = 0.0;
      double sum_y = 0.0;
      double sum_xy = 0.0;
      double sum_x2 = 0.0;

      for (int i = 0; i < n; ++i)
      {
            sum_x += y[i];
            sum_y += i;
            sum_xy += y[i] * i;
            sum_x2 += y[i] * y[i];
      }

      long double denom = n * sum_x2 - sum_x * sum_x;
      if (denom == 0.0)
      {
            return {0.0, sum_y / n, 0.0};
      }

      long double m = (n * sum_xy - sum_x * sum_y) / denom;
      long double b = (sum_y - m * sum_x) / n;
      double max_error = 0; // max error
      for (int i = 0; i < n; ++i)
      {
            double predicted = m * y[i] + b;
            if (max_error < abs(predicted - i))
                  max_error = abs(predicted - i);
      }

      max_error = ceil(max_error);

      cout
          << "- Linear Regression results: slope = " << m << ", intercept = " << b << ", max error = " << max_error << endl;
      cout << "- Linear Regression: performing MSSA with Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < y.size(); i++)
      {
            if (-1 == MSSA_Binary(y, y[i], y[i] * m + b, max_error))
            {
                  printf("! Error: MSSA with Binary missed y[%d] = %f\n", i, y[i]);
                  break;
            }
      }

      return {m, b, max_error};
}

tuple<long double, long double, long double, int> quadraticRegression(const vector<double> &y)
{
      int n = y.size();
      if (n < 3)
      {
            return make_tuple(0, 0, 0, 0); // or some default
      }
      double sum1 = 0, sumx = 0, sumx2 = 0, sumx3 = 0, sumx4 = 0, sumxy = 0, sumx2y = 0;
      for (int i = 0; i < n; ++i)
      {
            double x = y[i];
            double yy = i;
            sum1 += yy;
            sumx += x;
            sumx2 += x * x;
            sumx3 += x * x * x;
            sumx4 += x * x * x * x;
            sumxy += x * yy;
            sumx2y += x * x * yy;
      }
      long double detA = n * (sumx2 * sumx4 - sumx3 * sumx3) - sumx * (sumx * sumx4 - sumx3 * sumx2) + sumx2 * (sumx * sumx3 - sumx2 * sumx2);
      if (abs(detA) < 1e-10)
      {
            return make_tuple(0, 0, 0, 0);
      }
      long double detC = sum1 * (sumx2 * sumx4 - sumx3 * sumx3) - sumx * (sumxy * sumx4 - sumx3 * sumx2y) + sumx2 * (sumxy * sumx3 - sumx2 * sumx2y);
      double c = detC / detA;
      long double detB = n * (sumxy * sumx4 - sumx3 * sumx2y) - sum1 * (sumx * sumx4 - sumx3 * sumx2) + sumx2 * (sumx * sumx2y - sumxy * sumx2);
      long double b = detB / detA;
      long double detA_a = n * (sumx2 * sumx2y - sumx3 * sumxy) - sumx * (sumx * sumx2y - sumx3 * sum1) + sumx2 * (sumx * sumxy - sumx2 * sum1);
      long double a = detA_a / detA;
      double max_error = 0;
      for (int i = 0; i < n; ++i)
      {
            double x = y[i];
            double pred = a * x * x + b * x + c;
            int err = abs(pred - i);
            if (err > max_error)
                  max_error = err;
      }
      max_error = ceil(max_error);

      cout << "- Quadratic Regression results: a = " << a << ", b = " << b << ", c = " << c << ", max error = " << max_error << endl;

      cout << "- Quadratic Regression: performing MSSA with Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < y.size(); i++)
      {
            if (-1 == MSSA_Binary(y, y[i], a * y[i] * y[i] + b * y[i] + c, max_error))
            {
                  printf("! Error: MSSA with Binary missed y[%d] = %f\n", i, y[i]);
                  break;
            }
      }

      return make_tuple(a, b, c, max_error);
}

double timeBinarySearch(const vector<double> &data, const vector<double> &targets)
{
      cout << "+ Performing binary searches..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            binarySearch(data, target);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

double timeInterpolationSearch(const vector<double> &data, const vector<double> &targets)
{
      cout << "+ Performing interpolation searches..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            interpolationSearch(data, target);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

double timeMSSA_Binary(const vector<double> &data, const vector<double> &targets, long double inv_key, int max_error)
{
      cout << "+ Performing MSSA with Binary searches (fast MSSKCA)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            MSSA_Binary(data, target, target * inv_key - 1, max_error);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

double timeMSSA_Interpolation(const vector<double> &data, const vector<double> &targets, long double inv_key, int max_error)
{
      cout << "+ Performing MSSA with Interpolation searches (fast MSSKCA)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            MSSA_Interpolation(data, target, target * inv_key - 1, max_error);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

// overloaded functions for linear regression parameters
double timeMSSA_Binary(const vector<double> &data, const vector<double> &targets, long double slope, long double intercept, int max_error)
{
      cout << "+ Performing MSSA with Binary searches (linear regression)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            MSSA_Binary(data, target, target * slope + intercept, max_error);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

// overloaded functions for linear regression parameters
double timeMSSA_Interpolation(const vector<double> &data, const vector<double> &targets, long double slope, long double intercept, int max_error)
{
      cout << "+ Performing MSSA with Interpolation searches (linear regression)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            MSSA_Interpolation(data, target, target * slope + intercept, max_error);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

// overloaded functions for quadratic regression parameters
double timeMSSA_Binary(const vector<double> &data, const vector<double> &targets, long double a, long double b, long double c, int max_error)
{
      cout << "+ Performing MSSA with Binary searches (quadratic regression)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            MSSA_Binary(data, target, a * target * target + b * target + c, max_error);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

// overloaded functions for quadratic regression parameters
double timeMSSA_Interpolation(const vector<double> &data, const vector<double> &targets, long double a, long double b, long double c, int max_error)
{
      cout << "+ Performing MSSA with Interpolation searches (quadratic regression)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            MSSA_Interpolation(data, target, a * target * target + b * target + c, max_error);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      return elapsed.count();
}

int main()
{
      const int N = 10'000'000;
      cout << fixed << setprecision(2) << endl;
      cout << "+ Total number of elements: " << N << endl;
      const int SEARCH_COUNT = N * 10; // Number of searches to perform for timing

      random_device rd;
      mt19937 gen(rd());

      cout << endl;
      double mean = N * 10;   // helps to manipulate range of numbers
      double std_dev = N * 5; // mean distance around the mean
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
            classic_MSSKCA(data_normal);
      }

      cout << "+ Performing Fast MSSKCA for key and d ..." << endl;
      auto [inv_key, d] = fast_MSSKCA(data_normal);

      cout << "+ Performing Linear Regression for slope and intercept ..." << endl;
      auto [slope, intercept, lr_max_error] = linearRegression(data_normal);

      cout << "+ Performing Quadratic Regression for a, b, c ..." << endl;
      auto [a, b, c, qr_max_error] = quadraticRegression(data_normal);

      // cout << "+ Generating " << SEARCH_COUNT << " search targets..." << endl;
      // vector<double> targets_normal(SEARCH_COUNT);
      // for (int i = 0; i < SEARCH_COUNT; i++)
      // {
      //       targets_normal[i] = abs(normal_dist(gen));
      // }
      cout << "+ Using the same data as search targets..." << endl;
      vector<double> targets_normal = data_normal;

      double binary_time = timeBinarySearch(data_normal, targets_normal);

      double interpolation_time = timeInterpolationSearch(data_normal, targets_normal);

      double MSSA_binary_time = timeMSSA_Binary(data_normal, targets_normal, inv_key, d);

      double MSSA_interpolation_time = timeMSSA_Interpolation(data_normal, targets_normal, inv_key, d);

      double MSSA_binary_lg_time = timeMSSA_Binary(data_normal, targets_normal, slope, intercept, lr_max_error);

      double MSSA_interpolation_lg_time = timeMSSA_Interpolation(data_normal, targets_normal, slope, intercept, lr_max_error);

      double MSSA_binary_qr_time = timeMSSA_Binary(data_normal, targets_normal, a, b, c, qr_max_error);

      double MSSA_interpolation_qr_time = timeMSSA_Interpolation(data_normal, targets_normal, a, b, c, qr_max_error);

      cout << "-------------------------------------------" << endl;
      cout << "Outputting results for Normal Distribution" << endl;
      cout << "-------------------------------------------" << endl;

      cout << "+ Binary Search Time: " << binary_time << " seconds" << endl;
      cout << "+ Interpolation Search Time: " << interpolation_time << " seconds" << endl;
      cout << "+ MSSA with Binary Search (fast MSSKCA) Time: " << MSSA_binary_time << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search (fast MSSKCA) Time: " << MSSA_interpolation_time << " seconds" << endl;
      cout << "+ MSSA with Binary Search (Linear Regression) Time: " << MSSA_binary_lg_time << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search (Linear Regression) Time: " << MSSA_interpolation_lg_time << " seconds" << endl;
      cout << "+ MSSA with Binary Search (Quadratic Regression) Time: " << MSSA_binary_qr_time << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search (Quadratic Regression) Time: " << MSSA_interpolation_qr_time << " seconds" << endl;

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
            classic_MSSKCA(data_uniform);
      }

      cout << "+ Performing Fast MSSKCA for key and d ..." << endl;
      tie(inv_key, d) = fast_MSSKCA(data_uniform);

      cout << "+ Performing Linear Regression for slope and intercept ..." << endl;
      tie(slope, intercept, lr_max_error) = linearRegression(data_uniform);

      cout << "+ Performing Quadratic Regression for a, b, c ..." << endl;
      tie(a, b, c, qr_max_error) = quadraticRegression(data_uniform);

      // cout << "+ Generating " << SEARCH_COUNT << " search targets..." << endl;
      // vector<double> targets_uniform(SEARCH_COUNT);
      // for (int i = 0; i < SEARCH_COUNT; i++)
      // {
      //       targets_uniform[i] = uniform_dist(gen);
      // }

      cout << "+ Using the same data as search targets..." << endl;
      vector<double> targets_uniform = data_uniform;

      binary_time = timeBinarySearch(data_uniform, targets_uniform);

      interpolation_time = timeInterpolationSearch(data_uniform, targets_uniform);

      MSSA_binary_time = timeMSSA_Binary(data_uniform, targets_uniform, inv_key, d);

      MSSA_interpolation_time = timeMSSA_Interpolation(data_uniform, targets_uniform, inv_key, d);

      MSSA_binary_lg_time = timeMSSA_Binary(data_uniform, targets_uniform, slope, intercept, lr_max_error);

      MSSA_interpolation_lg_time = timeMSSA_Interpolation(data_uniform, targets_uniform, slope, intercept, lr_max_error);

      MSSA_binary_qr_time = timeMSSA_Binary(data_uniform, targets_uniform, a, b, c, qr_max_error);

      MSSA_interpolation_qr_time = timeMSSA_Interpolation(data_uniform, targets_uniform, a, b, c, qr_max_error);

      cout << "-------------------------------------------" << endl;
      cout << "Outputting results for Uniform Distribution" << endl;
      cout << "-------------------------------------------" << endl;

      cout << "+ Binary Search Time: " << binary_time << " seconds" << endl;
      cout << "+ Interpolation Search Time: " << interpolation_time << " seconds" << endl;
      cout << "+ MSSA with Binary Search (fast MSSKCA) Time: " << MSSA_binary_time << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search (fast MSSKCA) Time: " << MSSA_interpolation_time << " seconds" << endl;
      cout << "+ MSSA with Binary Search (Linear Regression) Time: " << MSSA_binary_lg_time << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search (Linear Regression) Time: " << MSSA_interpolation_lg_time << " seconds" << endl;
      cout << "+ MSSA with Binary Search (Quadratic Regression) Time: " << MSSA_binary_qr_time << " seconds" << endl;
      cout << "+ MSSA with Interpolation Search (Quadratic Regression) Time: " << MSSA_interpolation_qr_time << " seconds" << endl;

      cout << endl;

      return 0;
}