/* 
 * @Author: zhanghao
 * @LastEditTime: 2022-08-31 10:45:26
 * @FilePath: /camera_traffic_light_pipeline/src/include/types/box.h
 * @LastEditors: zhanghao
 * @Description: 
 */

#pragma once


typedef struct BoundingBox
{
    float x1, y1, x2, y2;
    BoundingBox()
    {
        x1, y1, x2, y2 = 0.f, 0.f, 0.f, 0.f;
    }
} BBox;




