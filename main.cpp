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
#include <limits>

using namespace std;
using namespace chrono;

int binarySearch(const vector<double> &data, double target)
{
      int low = 0;
      int high = data.size() - 1;
      while (low <= high)
      {
            int mid = (low + high) >> 1; // * 0.5;
            if (data[mid] == target)
                  return mid;
            if (data[mid] < target)
                  low = mid + 1;
            else
                  high = mid - 1;
      }
      return -1; // Not found
}

int interpolationSearch(const vector<double> &data, double target)
{
      int low = 0;
      int high = data.size() - 1;
      while (low < high && target >= data[low] && target <= data[high])
      {
            int pos = low + (target - data[low]) * (high - low) / (data[high] - data[low]);
            if (data[pos] == target)
                  return pos;
            if (data[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;
      }
      if (low == high)
            if (data[low] == target)
                  return low;
      return -1;
}

int MSSA_Binary(const vector<double> &data, double target, int pos, int low, int high)
{
      int last = data.size() - 1;

      if (low < 0)
            low = 0;
      if (high > last)
            high = last;
      if (pos < low)
            pos = low;
      if (pos > high)
            pos = high;

      while (low <= high)
      {
            if (data[pos] == target)
                  return pos;
            if (data[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;

            pos = (low + high) >> 1; // * 0.5;
      }
      return -1;
}

int MSSA_PartFinder(const vector<double> &data, double target, int pos, int low, int high)
{
      int last = data.size() - 1;

      if (low < 1) // data start from index 1
            low = 1;
      if (high > last)
            high = last;
      if (pos < low)
            pos = low;
      if (pos > high)
            pos = high;

      while (low < high)
      {
            pos = (low + high) >> 1; // * 0.5;
            // if (data[pos] == target)
            //       return pos;
            if (data[pos] < target)
                  low = pos + 1;
            else
                  high = pos;
      }
      return high;
}

int MSSA_Interpolation(const vector<double> &data, double target, int pos, int low, int high)
{
      int last = data.size() - 1;

      if (low < 0)
            low = 0;
      if (high > last)
            high = last;
      if (pos < low)
            pos = low;
      if (pos > high)
            pos = high;

      while (low < high && target >= data[low] && target <= data[high])
      {
            if (data[pos] == target)
                  return pos;
            if (data[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;
            pos = low + (target - data[low]) * (high - low) / (data[high] - data[low]);
      }
      if (low == high)
            if (data[low] == target)
                  return low;
      return -1;
}

int Reg_Binary(const vector<double> &data, double target, int pos, int low, int high)
{

      int last = data.size() - 1;
      if (low < 0)
            low = 0;
      if (high > last)
            high = last;
      if (pos < low)
            pos = low;
      if (pos > high)
            pos = high;

      while (low <= high)
      {
            if (data[pos] == target)
                  return pos;
            if (data[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;

            pos = (low + high) >> 1; //*0.5;
      }
      return -1;
}

int Reg_Interpolation(const vector<double> &data, double target, int pos, int low, int high)
{

      int last = data.size() - 1;
      if (low < 0)
            low = 0;
      if (pos < 0)
            pos = 0;
      if (pos > last)
            pos = last;
      if (high > last)
            high = last;

      while (low < high && target >= data[low] && target <= data[high])
      {
            if (data[pos] == target)
                  return pos;
            if (data[pos] < target)
                  low = pos + 1;
            else
                  high = pos - 1;

            pos = low + (target - data[low]) * (high - low) / (data[high] - data[low]);
      }
      if (low == high)
            return (data[low] == target) ? low : -1;
      return -1;
}

pair<int, long double> classic_MSSKCA(const vector<double> &data)
{
      auto start = high_resolution_clock::now();

      const int n = data.size();
      if (n == 0)
            return {0.0, 0.0};

      long double d = 0;
      long double key = data[0];
      for (int i = 0; i < n - 1; i++)
      {
            if (i % (n / 10) == 0)
            {
                  cout << "- Progress: " << i * 1.0 / n * 100 << "% is done" << endl;
            }
            for (int j = i + 1; j < n; j++)
            {
                  long double xi_over_i = data[i] / (i + 1);
                  long double xj_over_j = data[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        long double dd = (i * data[j] - j * data[i] + data[j] - data[i]) / (data[i] + data[j]);
                        if (dd > d)
                        {
                              key = data[j] / (j + 1 + dd);
                              d = dd;
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        long double dd = (j * data[i] - i * data[j] + data[i] - data[j]) / (data[i] + data[j]);
                        if (dd > d)
                        {
                              key = data[i] / (i + 1 + dd); // the denominator is always + to avoid a possible NaN case
                              d = dd;
                        }
                  }
            }
      }
      int max_err = ceil(d);
      long double inv_key = 1.0L / key; // for faster calculations

      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Progress: 100% is done" << endl;
      cout << "- Results: " << " key = " << key << ", max_err = left_err = right_err = " << max_err << endl;
      cout << "- Validity: performing MSSA NO-MISSED element test..." << endl;
      for (int i = 0; i < n; i++)
      {
            int pos = data[i] * inv_key - 1;
            if (-1 == MSSA_Binary(data, data[i], pos, pos - max_err, pos + max_err))
            {
                  printf("! Error: MSSA_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
            if (-1 == MSSA_Interpolation(data, data[i], pos, pos - max_err, pos + max_err))
            {
                  printf("! Error: MSSA_Interpolation missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }

      return {max_err, inv_key};
}

pair<int, long double> game_changer_MSSKCA(const vector<double> &data, int low = 0, int high = -1)
{

      int n = high + 1;                      // the upper limit (the nth element is not included)
      if (high == -1 || high >= data.size()) // when high is not passed
            n = data.size();

      // cout << "- Range details:  Start = " << low << " , End = " << n - 1 << endl;

      long double key = data[low] / (low + 1.0); // initial key
      long double d = 0.0;                       // d in ideal case
      vector<int> game_changer = {low};          // i = low, the first gc
      int counter = 0;
      while (counter < game_changer.size())
      {
            int i = game_changer[counter];
            int gc = -1;
            for (int j = low; j < n; j++)
            {
                  if (i == j)
                        continue;

                  long double xi_over_i = data[i] / (i + 1);
                  long double xj_over_j = data[j] / (j + 1);
                  if (xi_over_i < xj_over_j)
                  {
                        long double dd = (i * data[j] - j * data[i] + data[j] - data[i]) / (data[i] + data[j]);
                        if (dd > d)
                        {
                              key = data[j] / (j + 1 + dd);
                              d = dd;
                              gc = j;
                        }
                  }
                  else if (xi_over_i > xj_over_j)
                  {
                        long double dd = (j * data[i] - i * data[j] + data[i] - data[j]) / (data[i] + data[j]);
                        if (dd > d)
                        {
                              key = data[i] / (i + 1 + dd); // the denominator is always + to avoid a possible NaN case
                              d = dd;
                              gc = j;
                        }
                  }
            }
            if (gc != -1) // the largest dd is the game changer
            {
                  game_changer.push_back(gc);
                  // cout << "- Game changer tracking: gc No " << counter << " is   i   = " << game_changer[counter] << " ,  j = " << gc << endl;
                  // cout << "- Game changer details : gc No " << counter << " has  key = " << key << " ,  d = " << d << endl;
                  // cout << "-------------------------------------------------------------------" << endl;
            }
            counter++;
      }

      int max_err = ceil(d);
      long double inv_key = 1.0L / key; // for faster calculations

      // cout << "- Results: " << " key = " << key << ", max_err = left_err = right_err = " << max_err << endl;
      // cout << "- Validity: performing MSSA NO-MISSED element test..." << endl
      // << endl;
      for (int i = low; i < n; i++)
      {
            int pos = data[i] * inv_key - 1;
            if (-1 == MSSA_Binary(data, data[i], pos, pos - max_err, pos + max_err))
            {
                  printf("! Error: MSSA_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
            if (-1 == MSSA_Interpolation(data, data[i], pos, pos - max_err, pos + max_err))
            {
                  printf("! Error: MSSA_Interpolation missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }

      return {max_err, inv_key};
}

tuple<vector<double>, vector<int>, vector<long double>> parts_game_changer_MSSKCA(const vector<double> &data, const int parts_count = 1)
{

      if (parts_count <= 1)
            return {vector<double>{0}, vector<int>{0}, vector<long double>{0}};

      const int part_size = data.size() / parts_count;

      vector<long double> parts_inv_key_list(parts_count + 1);
      vector<int> parts_max_err_list(parts_count + 1);
      vector<double> parts_high_list(parts_count + 1);

      int low, high = -1;
      for (int part = 1; part <= parts_count; part++)
      {
            low = high + 1;
            high = part * part_size - 1;
            parts_high_list[part] = data[high];
            tie(parts_max_err_list[part], parts_inv_key_list[part]) = game_changer_MSSKCA(data, low, high);
      }
      auto [parts_max_error, parts_inv_key] = game_changer_MSSKCA(parts_high_list, 1, parts_count);
      parts_inv_key_list[0] = parts_inv_key;
      parts_max_err_list[0] = parts_max_error;
      parts_high_list[0] = data[data.size() - 1];

      cout << "- Results: parts_high_list, parts_max_err_list, and parts_inv_key_list are calculated successfully" << endl;
      cout << "- Validity: performing MSSA NO-MISSED element test..." << endl;
      cout << "-------------------------------------------------------------------" << endl;

      for (int i = 0; i < data.size(); i++)
      {
            double target = data[i];
            int part_pos = target * parts_inv_key_list[0]; // - 1 + 1 (real pos starts from 1)

            int part = MSSA_PartFinder(parts_high_list, target, part_pos, part_pos - parts_max_err_list[0], part_pos + parts_max_err_list[0]);
            int pos = target * parts_inv_key_list[part] - 1;
            if (-1 == MSSA_Binary(data, target, pos, pos - parts_max_err_list[part], pos + parts_max_err_list[part]))
            {
                  printf("! Error: MSSA_Binary missed data[%d] = %f\n", i, target);
                  break;
            }
            if (-1 == MSSA_Interpolation(data, target, pos, pos - parts_max_err_list[part], pos + parts_max_err_list[part]))
            {
                  printf("! Error: MSSA_Interpolation missed data[%d] = %f\n", i, target);
                  break;
            }
      }

      return {parts_high_list, parts_max_err_list, parts_inv_key_list};
}

tuple<long double, long double, int, int> linearRegression(const vector<double> &data)
{
      int n = data.size();
      if (n == 0)
            return {0.0, 0.0, 0.0, 0.0};

      double sum_x = 0.0;
      double sum_y = 0.0;
      double sum_xy = 0.0;
      double sum_x2 = 0.0;

      for (int i = 0; i < n; ++i)
      {
            sum_x += data[i];
            sum_y += i;
            sum_xy += data[i] * i;
            sum_x2 += data[i] * data[i];
      }

      long double denom = n * sum_x2 - sum_x * sum_x;
      if (denom == 0.0)
      {
            return {0.0, sum_y / n, 0.0, 0.0};
      }

      long double m = (n * sum_xy - sum_x * sum_y) / denom;
      long double b = (sum_y - m * sum_x) / n;

      long double left_err = 0, right_err = 0;

      for (int i = 0; i < n; i++)
      {
            double x = data[i];
            long double pred = m * x + b;
            long double diff = pred - i;

            if (diff > 0 && diff > left_err)
                  left_err = diff;
            if (diff < 0 && fabsl(diff) > right_err)
                  right_err = fabsl(diff);
      }

      left_err = ceill(left_err);
      right_err = ceill(right_err);

      cout << "- Results: m = " << m << ", intercept = " << b << ", left_err = " << left_err << ", right_err = " << right_err << endl;
      cout << "- Validity: performing Reg_Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < n; i++)
      {
            int pos = data[i] * m + b;
            if (-1 == Reg_Binary(data, data[i], pos, pos - left_err, pos + right_err))
            {
                  printf("! Error: Reg_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }

      return {m, b, left_err, right_err};
}

tuple<long double, long double, long double, int, int> quadraticRegression(const vector<double> &data)
{
      int n = data.size();
      if (n < 3)
      {
            return {0, 0, 0, 0, 0}; // or some default
      }
      double sum1 = 0, sumx = 0, sumx2 = 0, sumx3 = 0, sumx4 = 0, sumxy = 0, sumx2y = 0;
      for (int i = 0; i < n; ++i)
      {
            double x = data[i];
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
            return {0, 0, 0, 0, 0};
      }
      long double detC = sum1 * (sumx2 * sumx4 - sumx3 * sumx3) - sumx * (sumxy * sumx4 - sumx3 * sumx2y) + sumx2 * (sumxy * sumx3 - sumx2 * sumx2y);
      double c = detC / detA;
      long double detB = n * (sumxy * sumx4 - sumx3 * sumx2y) - sum1 * (sumx * sumx4 - sumx3 * sumx2) + sumx2 * (sumx * sumx2y - sumxy * sumx2);
      long double b = detB / detA;
      long double detA_a = n * (sumx2 * sumx2y - sumx3 * sumxy) - sumx * (sumx * sumx2y - sumx3 * sum1) + sumx2 * (sumx * sumxy - sumx2 * sum1);
      long double a = detA_a / detA;

      long double left_err = 0, right_err = 0;

      for (int i = 0; i < n; i++)
      {
            double x = data[i];
            long double pred = a * x * x + b * x + c;
            long double diff = pred - i;

            if (diff > 0 && diff > left_err)
                  left_err = diff;
            if (diff < 0 && fabsl(diff) > right_err)
                  right_err = fabsl(diff);
      }

      left_err = ceill(left_err);
      right_err = ceill(right_err);

      cout << "- Results: a = " << a << ", b = " << b << ", c = " << c << ", left_err = " << left_err << ", right_err = " << right_err << endl;

      cout << "- Validity: performing Reg_Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < n; i++)
      {
            int pos = a * data[i] * data[i] + b * data[i] + c;

            if (-1 == Reg_Binary(data, data[i], pos, pos - left_err, pos + right_err))
            {
                  printf("! Error: Reg_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }

      return {a, b, c, left_err, right_err};
}

tuple<long double, long double, long double, long double, int, int> cubicRegression(const vector<double> &data)
{
      int n = data.size();
      long double sX[7] = {0}, sY[4] = {0};

      for (int i = 0; i < n; ++i)
      {
            long double x = data[i], target = i;
            long double x2 = x * x, x3 = x2 * x;
            sX[0] += 1;
            sX[1] += x;
            sX[2] += x2;
            sX[3] += x3;
            sX[4] += x3 * x;
            sX[5] += x3 * x2;
            sX[6] += x3 * x3;
            sY[0] += target;
            sY[1] += target * x;
            sY[2] += target * x2;
            sY[3] += target * x3;
      }

      long double mat[4][5] = {
          {sX[0], sX[1], sX[2], sX[3], sY[0]}, {sX[1], sX[2], sX[3], sX[4], sY[1]}, {sX[2], sX[3], sX[4], sX[5], sY[2]}, {sX[3], sX[4], sX[5], sX[6], sY[3]}};

      for (int i = 0; i < 4; i++)
      {
            for (int k = i + 1; k < 4; k++)
            {
                  long double t = mat[k][i] / mat[i][i];
                  for (int j = i; j <= 4; j++)
                        mat[k][j] -= t * mat[i][j];
            }
      }

      long double res[4];
      for (int i = 3; i >= 0; i--)
      {
            res[i] = mat[i][4];
            for (int j = i + 1; j < 4; j++)
                  res[i] -= mat[i][j] * res[j];
            res[i] /= mat[i][i];
      }

      long double d = res[0], c = res[1], b = res[2], a = res[3];

      long double left_err = 0, right_err = 0;

      for (int i = 0; i < n; i++)
      {
            long double pred = a * pow(data[i], 3) + b * pow(data[i], 2) + c * data[i] + d;
            long double diff = pred - i;

            if (diff > 0 && diff > left_err)
                  left_err = diff;
            if (diff < 0 && fabsl(diff) > right_err)
                  right_err = fabsl(diff);
      }

      left_err = ceill(left_err);
      right_err = ceill(right_err);
      cout << "- Results: a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << ", left_err = " << left_err << ", right_err = " << right_err << endl;

      cout << "- Validity: performing Reg_Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < n; i++)
      {
            int pos = a * data[i] * data[i] * data[i] + b * data[i] * data[i] + c * data[i] + d;
            if (-1 == Reg_Binary(data, data[i], pos, pos - left_err, pos + right_err))
            {
                  printf("! Error: Reg_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }

      return {a, b, c, d, left_err, right_err};
}

tuple<long double, long double, int, int> exponentialRegression(const vector<double> &data)
{
      int n = data.size();
      long double sumY = 0, sumLnI = 0, sumY2 = 0, sumYLnI = 0;

      for (int i = 0; i < n; ++i)
      {
            long double valY = data[i];
            long double lnI = logl(i + 1.0);
            sumY += valY;
            sumLnI += lnI;
            sumY2 += valY * valY;
            sumYLnI += valY * lnI;
      }

      long double denominator = (n * sumY2 - sumY * sumY);
      if (fabsl(denominator) < 1e-20)
            denominator = 1e-20;
      long double b = (n * sumYLnI - sumY * sumLnI) / denominator;
      long double lnA = (sumLnI - b * sumY) / n;
      long double a = expl(lnA);

      long double left_err = 0, right_err = 0;
      for (int i = 0; i < n; i++)
      {
            long double pred = a * expl(b * data[i]) - 1.0;
            long double diff = pred - i;

            if (diff > 0 && diff > left_err)
                  left_err = diff;
            if (diff < 0 && fabsl(diff) > right_err)
                  right_err = fabsl(diff);
      }
      left_err = ceill(left_err);
      right_err = ceill(right_err);

      cout << "- Results: a = " << a << ", b = " << b << ", left_err = " << left_err << ", right_err = " << right_err << endl;
      cout << "- Validity: performing Reg_Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < n; i++)
      {
            long double pos = a * expl(b * data[i]) - 1.0;

            if (-1 == Reg_Binary(data, data[i], pos, pos - left_err, pos + right_err))
            {
                  printf("! Error: Reg_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }
      return {a, b, left_err, right_err};
}

tuple<long double, long double, int, int> powerRegression(const vector<double> &data)
{
      int n = data.size();
      long double sumLnY = 0, sumLnI = 0, sumLnY2 = 0, sumLnYLnI = 0;

      for (int i = 0; i < n; ++i)
      {
            long double lnY = logl(data[i]);
            long double lnI = logl(i + 1.0);
            sumLnY += lnY;
            sumLnI += lnI;
            sumLnY2 += lnY * lnY;
            sumLnYLnI += lnY * lnI;
      }

      long double b = (n * sumLnYLnI - sumLnY * sumLnI) / (n * sumLnY2 - sumLnY * sumLnY);
      long double a = expl((sumLnI - b * sumLnY) / n);

      long double left_err = 0, right_err = 0;
      for (int i = 0; i < n; i++)
      {
            long double pred = a * powl(data[i], b) - 1.0;
            long double diff = pred - i;
            if (diff > 0 && diff > left_err)
                  left_err = diff;
            if (diff < 0 && fabsl(diff) > right_err)
                  right_err = fabsl(diff);
      }
      left_err = ceill(left_err);
      right_err = ceill(right_err);

      cout << "- Results: a = " << a << ", b = " << b << ", left_err = " << left_err << ", right_err = " << right_err << endl;
      cout << "- Validity: performing Reg_Binary NO-MISSED element test..." << endl;
      for (int i = 0; i < n; i++)
      {
            int pos = a * powl(data[i], b) - 1.0;
            if (-1 == Reg_Binary(data, data[i], pos, pos - left_err, pos + right_err))
            {
                  printf("! Error: Reg_Binary missed data[%d] = %f\n", i, data[i]);
                  break;
            }
      }

      return {a, b, left_err, right_err};
}

void time_bin(const vector<double> &data, const vector<double> &targets)
{
      cout << "- Performing Binary Search (Original)..." << endl;
      auto low = high_resolution_clock::now();
      for (double target : targets)
      {
            binarySearch(data, target);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - low;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_inter(const vector<double> &data, const vector<double> &targets)
{
      cout << "+ Performing Interpolation Search (Original)..." << endl;
      auto low = high_resolution_clock::now();
      for (double target : targets)
      {
            interpolationSearch(data, target);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - low;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_ms_bin(const vector<double> &data, const vector<double> &targets, int max_err, long double inv_key)
{
      cout << "- Performing MSSA_Binary Search (MSSKCA params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = target * inv_key - 1;
            MSSA_Binary(data, target, pos, pos - max_err, pos + max_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_parts_ms_bin(const vector<double> &data, const vector<double> &targets, vector<double> &parts_high_list, vector<int> &parts_max_err_list, vector<long double> &parts_inv_key_list)
{
      cout << "- Performing GAME CHANGER MSSA_Binary Search (MSSKCA params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = target * parts_inv_key_list[0]; // - 1 + 1 (real pos starts from 1)
            int part = MSSA_PartFinder(parts_high_list, target, pos, pos - parts_max_err_list[0], pos + parts_max_err_list[0]);
            pos = target * parts_inv_key_list[part] - 1;
            MSSA_Binary(data, target, pos, pos - parts_max_err_list[part], pos + parts_max_err_list[part]);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_lin_bin(const vector<double> &data, const vector<double> &targets, long double m, long double b, int left_err, int right_err)
{
      cout << "- Performing Reg_Binary Search (linear regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = target * m + b;
            Reg_Binary(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_quad_bin(const vector<double> &data, const vector<double> &targets, long double a, long double b, long double c, int left_err, int right_err)
{
      cout << "- Performing Reg_Binary Search (quadratic regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = a * target * target + b * target + c;
            Reg_Binary(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_cub_bin(const vector<double> &data, const vector<double> &targets, long double a, long double b, long double c, long double d, int left_err, int right_err)
{
      cout << "- Performing Reg_Binary Search (cubic regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = a * target * target * target + b * target * target + c * target + d;
            Reg_Binary(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_exp_bin(const vector<double> &data, const vector<double> &targets, long double a, long double b, int left_err, int right_err)
{
      cout << "- Performing Reg_Binary Search (exponential regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            long double pos = a * expl(b * target) - 1.0;
            Reg_Binary(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_pow_bin(const vector<double> &data, const vector<double> &targets, long double a, long double b, int left_err, int right_err)
{
      cout << "- Performing Reg_Binary Search (power regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = a * powl(target, b) - 1.0;
            Reg_Binary(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_ms_inter(const vector<double> &data, const vector<double> &targets, int max_err, long double inv_key)
{
      cout << "- Performing MSSA_Interpolation Search (MSSKCA params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = target * inv_key - 1;
            MSSA_Interpolation(data, target, pos, pos - max_err, pos + max_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_parts_ms_inter(const vector<double> &data, const vector<double> &targets, vector<double> &parts_high_list, vector<int> &parts_max_err_list, vector<long double> &parts_inv_key_list)
{
      cout << "- Performing GAME CHANGER MSSA_Interpolation Search (MSSKCA params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = target * parts_inv_key_list[0]; // - 1 + 1 (real pos starts from 1)
            int part = MSSA_PartFinder(parts_high_list, target, pos, pos - parts_max_err_list[0], pos + parts_max_err_list[0]);
            pos = target * parts_inv_key_list[part] - 1;
            MSSA_Interpolation(data, target, pos, pos - parts_max_err_list[part], pos + parts_max_err_list[part]);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_lin_inter(const vector<double> &data, const vector<double> &targets, long double m, long double b, int left_err, int right_err)
{
      cout << "- Performing Reg_Interpolation Search (linear regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = target * m + b;
            Reg_Interpolation(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_quad_inter(const vector<double> &data, const vector<double> &targets, long double a, long double b, long double c, int left_err, int right_err)
{
      cout << "- Performing Reg_Interpolation Search (quadratic regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = a * target * target + b * target + c;
            Reg_Interpolation(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_cub_inter(const vector<double> &data, const vector<double> &targets, long double a, long double b, long double c, long double d, int left_err, int right_err)
{
      cout << "- Performing Reg_Interpolation Search (cubic regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = a * target * target * target + b * target * target + c * target + d;
            Reg_Interpolation(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_exp_inter(const vector<double> &data, const vector<double> &targets, long double a, long double b, int left_err, int right_err)
{
      cout << "- Performing Reg_Interpolation Search (exponential regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            long double pos = a * expl(b * target) - 1.0;
            Reg_Interpolation(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

void time_pow_inter(const vector<double> &data, const vector<double> &targets, long double a, long double b, int left_err, int right_err)
{
      cout << "- Performing Reg_Interpolation Search (power regression params)..." << endl;
      auto start = high_resolution_clock::now();
      for (double target : targets)
      {
            int pos = a * powl(target, b) - 1.0;
            Reg_Interpolation(data, target, pos, pos - left_err, pos + right_err);
      }
      auto end = high_resolution_clock::now();
      duration<double> elapsed = end - start;
      cout << "- Search Time: " << elapsed.count() << " seconds" << endl;
      cout << "-------------------------------------------------------------------" << endl;
}

int main(int argc, char *argv[])
{
      const int N = 1'000'000;
      const int SEARCH_COUNT = N * 10; // Number of searches to perform for timing
      cout << fixed << setprecision(3) << endl;
      cout << "+ Total number of elements: " << N << endl
           << endl;

      random_device rd;
      mt19937 gen(rd());

      double mean = N / 2;
      double std_dev = N / 6;
      cout << "Test 1: Normal Distribution (Mean: " << mean << ", Std Dev:" << std_dev << ")" << endl;
      cout << "**************************************************************************" << endl;

      cout << "+ Generating normally distributed numbers..." << endl;
      normal_distribution<> normal_dist(mean, std_dev);
      vector<double> data(N);
      for (int i = 0; i < N; i++)
      {
            data[i] = abs(normal_dist(gen));
      }

      cout << "+ Sorting the original data..." << endl;
      sort(data.begin(), data.end());
      cout << "+ Original Data range: [" << data.front() << ", " << data.back() << "]" << endl;

      //! Warning: The Classic MSSKCA is too slow for large N
      if (N <= 100'000)
      {
            cout << "+ Performing Classic MSSKCA for key and d ..." << endl;
            classic_MSSKCA(data);
      }

      cout << "+ Performing Full Game Changer MSSKCA for key and d ..." << endl;
      auto [ms_max_error, ms_inv_key] = game_changer_MSSKCA(data);

      int parts_count = atoi(argv[1]);
      if (parts_count > 1)
            cout << "+ Performing Parts Game Changer MSSKCA for " << parts_count << " parts count to calculate key and d ..." << endl;
      auto [parts_high_list, parts_max_err_list, parts_inv_key_list] = parts_game_changer_MSSKCA(data, parts_count); // if parts_count <= 1 it returns 0,0,0

      cout << "+ Performing Linear Regression for m and intercept ..." << endl;
      auto [m, b, lr_left_err, lr_right_err] = linearRegression(data);

      cout << "+ Performing Quadratic Regression for a, b, c ..." << endl;
      auto [qr_a, qr_b, qr_c, qr_left_err, qr_right_err] = quadraticRegression(data);

      cout << "+ Performing Cubic Regression for a, b, c, d ..." << endl;
      auto [cr_a, cr_b, cr_c, cr_d, cr_left_err, cr_right_err] = cubicRegression(data);

      cout << "+ Performing Exponential Regression for a, b ..." << endl;
      auto [er_a, er_b, er_left_err, er_right_err] = exponentialRegression(data);

      cout << "+ Performing Power Regression for a, b ..." << endl;
      auto [pr_a, pr_b, pr_left_err, pr_right_err] = powerRegression(data);

      cout << "+ Generating " << SEARCH_COUNT << " Search Targets..." << endl;
      vector<double> targets(SEARCH_COUNT);
      for (int i = 0; i < SEARCH_COUNT; i++)
      {
            targets[i] = abs(normal_dist(gen));
      }
      cout << "+ Sorting the test data..." << endl;
      sort(targets.begin(), targets.end());
      cout << "+ Test Data range: [" << targets.front() << ", " << targets.back() << "]" << endl;

      // cout << "+ Using the same data as Search Targets..." << endl;
      // vector<double> targets = data;

      cout << "-------------------------------------------------------------------" << endl;
      cout << "Performing Execution Time Tests for Normal Distribution" << endl;
      cout << "-------------------------------------------------------------------" << endl;

      time_bin(data, targets);

      time_ms_bin(data, targets, ms_max_error, ms_inv_key);

      if (parts_count > 1)
            time_parts_ms_bin(data, targets, parts_high_list, parts_max_err_list, parts_inv_key_list);

      time_lin_bin(data, targets, m, b, lr_left_err, lr_right_err);

      time_quad_bin(data, targets, qr_a, qr_b, qr_c, qr_left_err, qr_right_err);

      time_cub_bin(data, targets, cr_a, cr_b, cr_c, cr_d, cr_left_err, cr_right_err);

      time_exp_bin(data, targets, er_a, er_b, er_left_err, er_right_err);

      time_pow_bin(data, targets, pr_a, pr_b, pr_left_err, pr_right_err);

      cout << endl
           << endl;

      int max = N * 10;
      cout << "Test 2: Uniform Distribution (Min: 0, Max: " << max << ")" << endl;
      cout << "**************************************************************************" << endl;

      cout << "+ Generating uniformly distributed numbers..." << endl;
      uniform_real_distribution<> uniform_dist(0, max);
      for (int i = 0; i < N; i++)
      {
            data[i] = uniform_dist(gen);
      }

      cout << "+ Sorting the original data..." << endl;
      sort(data.begin(), data.end());
      cout << "+ Original Data range: [" << data.front() << ", " << data.back() << "]" << endl;

      //! Warning: The Classic MSSKCA is too slow for large N
      if (N <= 100'000)
      {
            cout << "+ Performing Classic MSSKCA for key and d ..." << endl;
            classic_MSSKCA(data);
      }

      cout << "+ Performing Full Game Changer MSSKCA for key and d ..." << endl;
      tie(ms_max_error, ms_inv_key) = game_changer_MSSKCA(data);

      if (parts_count > 1)
            cout << "+ Performing Parts Game Changer MSSKCA for " << parts_count << " parts count to calculate key and d ..." << endl;
      tie(parts_high_list, parts_max_err_list, parts_inv_key_list) = parts_game_changer_MSSKCA(data, parts_count); // if parts_count <= 1 it returns 0,0,0

      cout << "+ Performing Linear Regression for m and intercept ..." << endl;
      tie(m, b, lr_left_err, lr_right_err) = linearRegression(data);

      cout << "+ Performing Quadratic Regression for a, b, c ..." << endl;
      tie(qr_a, qr_b, qr_c, qr_left_err, qr_right_err) = quadraticRegression(data);

      cout << "+ Performing Cubic Regression for a, b, c, d ..." << endl;
      tie(cr_a, cr_b, cr_c, cr_d, cr_left_err, cr_right_err) = cubicRegression(data);

      cout << "+ Performing Exponential Regression for a, b ..." << endl;
      tie(er_a, er_b, er_left_err, er_right_err) = exponentialRegression(data);

      cout << "+ Performing Power Regression for a, b ..." << endl;
      tie(er_a, er_b, pr_left_err, pr_right_err) = powerRegression(data);

      cout << "+ Generating " << SEARCH_COUNT << " Search Targets..." << endl;
      for (int i = 0; i < SEARCH_COUNT; i++)
      {
            targets[i] = uniform_dist(gen);
      }

      cout << "+ Sorting the test data..." << endl;
      sort(targets.begin(), targets.end());
      cout << "+ Test Data range: [" << targets.front() << ", " << targets.back() << "]" << endl;

      // cout << "+ Using the same data as Search Targets..." << endl;
      // vector<double> targets = data;

      cout << "-------------------------------------------------------------------" << endl;

      cout << "Performing Execution Time Tests for Uniform Distribution" << endl;
      cout << "-------------------------------------------------------------------" << endl;

      time_inter(data, targets);
      time_ms_inter(data, targets, ms_max_error, ms_inv_key);

      if (parts_count > 1)
            time_parts_ms_inter(data, targets, parts_high_list, parts_max_err_list, parts_inv_key_list);

      time_lin_inter(data, targets, m, b, lr_left_err, lr_right_err);

      time_quad_inter(data, targets, qr_a, qr_b, qr_c, qr_left_err, qr_right_err);

      time_cub_inter(data, targets, cr_a, cr_b, cr_c, cr_d, cr_left_err, cr_right_err);

      time_exp_inter(data, targets, er_a, er_b, er_left_err, er_right_err);

      time_pow_inter(data, targets, pr_a, pr_b, pr_left_err, pr_right_err);

      cout << endl
           << endl;

      cout << "Test 3: Exponential Distribution" << endl;
      cout << "**************************************************************************" << endl;

      cout << "+ Generating exponentially distributed numbers..." << endl;
      exponential_distribution<> exp_dist(1.0 / (N / 5.0));
      for (int i = 0; i < N; i++)
      {
            data[i] = abs(exp_dist(gen));
      }

      cout << "+ Sorting the original data..." << endl;
      sort(data.begin(), data.end());
      cout << "+ Original Data range: [" << data.front() << ", " << data.back() << "]" << endl;

      //! Warning: The Classic MSSKCA is too slow for large N
      if (N <= 100'000)
      {
            cout << "+ Performing Classic MSSKCA for key and d ..." << endl;
            classic_MSSKCA(data);
      }

      cout << "+ Performing Full Game Changer MSSKCA for key and d ..." << endl;
      tie(ms_max_error, ms_inv_key) = game_changer_MSSKCA(data);

      if (parts_count > 1)
            cout << "+ Performing Parts Game Changer MSSKCA for " << parts_count << " parts count to calculate key and d ..." << endl;
      tie(parts_high_list, parts_max_err_list, parts_inv_key_list) = parts_game_changer_MSSKCA(data, parts_count); // if parts_count <= 1 it returns 0,0,0

      cout << "+ Performing Linear Regression for m and intercept ..." << endl;
      tie(m, b, lr_left_err, lr_right_err) = linearRegression(data);

      cout << "+ Performing Quadratic Regression for a, b, c ..." << endl;
      tie(qr_a, qr_b, qr_c, qr_left_err, qr_right_err) = quadraticRegression(data);

      cout << "+ Performing Cubic Regression for a, b, c, d ..." << endl;
      tie(cr_a, cr_b, cr_c, cr_d, cr_left_err, cr_right_err) = cubicRegression(data);

      cout << "+ Performing Exponential Regression for a, b ..." << endl;
      tie(er_a, er_b, er_left_err, er_right_err) = exponentialRegression(data);

      cout << "+ Performing Power Regression for a, b ..." << endl;
      tie(pr_a, pr_b, pr_left_err, pr_right_err) = powerRegression(data);

      cout << "+ Generating " << SEARCH_COUNT << " Search Targets..." << endl;
      for (int i = 0; i < SEARCH_COUNT; i++)
      {
            targets[i] = abs(exp_dist(gen));
      }

      cout << "+ Sorting the test data..." << endl;
      sort(targets.begin(), targets.end());
      cout << "+ Test Data range: [" << targets.front() << ", " << targets.back() << "]" << endl;

      cout << "-------------------------------------------------------------------" << endl;

      cout << "Performing Execution Time Tests for Exponential Distribution" << endl;
      cout << "-------------------------------------------------------------------" << endl;

      time_bin(data, targets);

      time_ms_bin(data, targets, ms_max_error, ms_inv_key);

      if (parts_count > 1)
            time_parts_ms_bin(data, targets, parts_high_list, parts_max_err_list, parts_inv_key_list);

      time_lin_bin(data, targets, m, b, lr_left_err, lr_right_err);

      time_quad_bin(data, targets, qr_a, qr_b, qr_c, qr_left_err, qr_right_err);

      time_cub_bin(data, targets, cr_a, cr_b, cr_c, cr_d, cr_left_err, cr_right_err);

      time_exp_bin(data, targets, er_a, er_b, er_left_err, er_right_err);

      time_pow_bin(data, targets, pr_a, pr_b, pr_left_err, pr_right_err);

      cout << endl
           << endl;
}