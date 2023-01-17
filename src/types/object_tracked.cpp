/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 14:11:30
 * @FilePath: /yolox_deploy/src/types/object_tracked.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "object_tracked.h"


bool TrackedObject::IsValid()
{
    float ratio = rect.height / rect.width;
    bool is_shape_valid = rect.width  > BOX_MIN_PIXEL 
                       && rect.height > BOX_MIN_PIXEL 
                       && ratio < BOX_RATIO_THRESHOLD 
                       && ratio > 1.0 / BOX_RATIO_THRESHOLD;

    bool is_boundry_valid = rect.x > MIN_SIDE_PIXEL_THRESH 
                         && rect.y > MIN_SIDE_PIXEL_THRESH 
                         && rect.x + rect.width  < DEFAULT_IMAGE_WIDTH
                         && rect.y + rect.height < DEFAULT_IMAGE_HEIGHT;

    return is_shape_valid && is_boundry_valid;
}

