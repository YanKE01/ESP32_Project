#include "tools.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

vector<float> dataList;

/**
 * @brief 数据写入vector
 *
 * @param data
 */
void PushVector(float data)
{
    dataList.push_back(data); //向后追加数据
}

/**
 * @brief 查询vector长度
 *
 * @return int
 */
int QueryVectorLength()
{
    return dataList.size();
}

/**
 * @brief 清空vector
 *
 */
void ClearVector()
{
    dataList.clear();
}

/**
 * @brief 获取最大值
 *
 */
float GetMaxValue()
{
    return *max_element(dataList.begin(), dataList.end());
}

/**
 * @brief 获取最小值
 *
 */
float GetAvgValue()
{
    float sum = 0;
    for (int i = 0; i < dataList.size(); i++)
    {
        sum += dataList[i];
    }

    return sum / dataList.size();
}