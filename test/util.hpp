#pragma once

#include <boost/test/unit_test.hpp>

#include <boost/format.hpp>
#include <chrono>
#include <random>
#include <sstream>
#include <unordered_set>

/**
 * @brief 随机数生成工具
 */
namespace genrand {

extern thread_local std::default_random_engine gRand;

/**
 * @brief 生成随机的布尔值
 */
inline bool
tf()
{
  return gRand() & 1;
}

/**
 * @brief 生成0~1之间的均匀随机变量。
 */
inline double
norm()
{
  return std::uniform_real_distribution<>(0, 1)(gRand);
}

/**
 * @brief 生成[0,end)间的随机整数。
 */
inline std::size_t
index(std::size_t end)
{
  return std::uniform_int_distribution<std::size_t>(0, end - 1)(gRand);
}

/**
 * @brief 生成期望将0~1均匀分成n+1份的随机分割点数列，返回的数列已按升序排序。
 */
std::vector<double>
split(std::size_t n);

}
