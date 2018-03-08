//
// Created by zzhfeng on 2018/3/7.
//

#include "PWLH.h"
#include <math.h>


/**
 * 构造函数初始化
 */
PWLH_Data::PWLH_Data() {
    counter = 0;
    a = 0.0;
    b = 0.0;
}

/**
 * 重置结构
 */
void PWLH_Data::reset() {
    counter = 0;
    a = 0.0;
    b = 0.0;
}

/**
 * 求回归分析的a,b值
 * @param points
 * @param sum
 * @return
 */
float *get_a_b(std::vector<float> &points, double sum) {
    float result[2];
    int len = points.size();
    float avg_x = sum / len;
    float avg_y = (len % 2 == 0) ? ((float) len / 2 + 0.5) : (len / 2);
    double sum_xy = 0.0;
    double square_x = 0.0;
    int pos = 1;
    for (auto it = points.begin(); it != points.end(); it++) {
        sum_xy += *it * pos;
        square_x += *it * *it;
        pos++;
    }
    float b = (sum_xy - len * avg_x * avg_y) / (square_x - len * avg_x * avg_x);
    float a = avg_y - b * avg_x;
    result[0] = a;
    result[1] = b;
    return result;
}

double compute_e(float *para, std::vector<float> &points) {
    double variance = 0.0;
    int pos = 1;
    for (auto it = points.begin(); it != points.end(); it++) {
        float new_x = para[0] + pos * para[1];
        variance += (new_x - *it) * (new_x - *it);
        pos++;
    }
    return sqrt(variance / points.size());
}

/**
 * 构造函数初始化
 * @param f
 */
PWLH::PWLH(double f) {
    variance_threshold = f;
}

/**
 * 重置PLWH
 */
void PWLH::reset() {
    pwlh_data.reset();
    points.clear();
    sum = 0.0;
}

/**
 * 读取被压缩的原始数据
 * @return
 */
std::vector<float> PWLH::get_points() {
    return points;
}

/**
 * 开始线性拟合,并写入开始数据点
 * @param f
 */
void PWLH::start(float f) {
    points.push_back(f);
    pwlh_data.counter += 1;
}

/**
 * 压缩数据点f
 * @param f
 * @return
 */
void PWLH::push_point(float f) {
    points.push_back(f);
    sum += f;
    pwlh_data.counter += 1;
}

/**
 * 新加入一个数据点后，拟合误差超过预设阈值，此时去除加入的数据点
 */
void PWLH::pop_point(float f) {
    points.pop_back();
    sum -= f;
    pwlh_data.counter -= 1;
}

/**
 * 根据新的数据点更新拟合器
 * @param f
 */
bool PWLH::update(float f) {
    push_point(f);
    float *para = get_a_b(points, sum);
    float avg_variance = compute_e(para, points);
    if (avg_variance <= variance_threshold) return true;
    pop_point(f);
    para = get_a_b(points, sum);
    pwlh_data.a = para[0];
    pwlh_data.b = para[1];
    reset();
    start(f);
    return false;
}
