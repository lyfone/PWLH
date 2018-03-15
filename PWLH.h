//
// Created by zzhfeng on 2018/3/7.
//

#ifndef PWLH_PWLH_H
#define PWLH_PWLH_H

#pragma once;

#include <vector>

/**
 * PWLH数据结构
 */
struct PWLH_Data{
    int counter;    //数据点数量
    float a;    //拟合线的参数a
    float b;      //拟合线的参数b

    PWLH_Data();
    void reset();
};

/**
 * PWLH类
 */
class PWLH{
private:
    //压缩的数据点集合
    std::vector<float> points;
    //PLWH数据结构
    PWLH_Data pwlh_data;
    //拟合方差阈值
    double variance_threshold;
    //数据点和
    double sum;

public:
    //构造函数初始化
    PWLH(double f);
    //读取数据点集合
    std::vector<float> get_points();
    //开始线性拟合
    void start(float f);
    //压缩数据点f
    void push_point(float f);
    //去除数据点
    void pop_point(float f);
    //重置
    void reset();
    //根据新的数据点更新拟合器
    bool update(float f);
};

#endif //PWLH_PWLH_H
