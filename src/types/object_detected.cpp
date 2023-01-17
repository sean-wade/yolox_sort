/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 14:09:15
 * @FilePath: /yolox_deploy/src/types/object_detected.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "object_detected.h"


bool DetectObject::CheckWidthValid()
{
    // if(rect.x < MIN_SIDE_PIXEL_THRESH || (rect.x + rect.width) > DEFAULT_IMAGE_WIDTH - MIN_SIDE_PIXEL_THRESH){
    if(rect.width < MIN_LIGHT_PIXEL_WIDTH){
        return false;
    }
    return true;
}


