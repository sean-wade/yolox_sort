/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 14:26:31
 * @FilePath: /yolox_deploy/src/types/object_tracked.h
 * @LastEditors: zhanghao
 * @Description: 
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "base_macros.h"
#include "object_detected.h"
#include "types/base_macros.h"


/* 
 * @description: tracked object
 * @author: zhanghao
 */
typedef struct TrackedObject
{
    // BBox bbox;
    ACCS_CLASS cls;
    cv_rect rect;
    float det_score;

    int track_id = -1;
    int track_age = 0;

    /* 
     * @description: Check box valid status.
     * @return {if bbox is valid}
     * @author: zhanghao
     */
    bool IsValid();

    float GetPixelCX()
    {
        return rect.x + rect.width / 2.0;
    }

    float GetPixelCY()
    {
        return rect.y + rect.height / 2.0;
    }
    
    std::string GetClassname() const{
        switch (cls)
        {
            case ACCS_HUMAN:
                return "human";
            case ACCS_GUIDER:
                return "guider";
            case ACCS_SIGN1:
                return "sign1";
            default:
                return "unknown";
        }
    }
} TrkObject;


typedef std::vector<TrackedObject> TrkObjectList;
typedef std::vector<TrackedObject*> TrkObjectPtrList;
