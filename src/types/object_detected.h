/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 15:20:21
 * @FilePath: /yolox_deploy/src/types/object_detected.h
 * @LastEditors: zhanghao
 * @Description: 
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
// #include "box.h"
#include "types/base_macros.h"


// detect object class
enum ACCS_CLASS
{
    ACCS_HUMAN      = 0,
    ACCS_GUIDER     = 1,
    ACCS_SIGN1      = 2,
    ACCS_UNKNOWN    = -1
};


// detect object
typedef struct DetectObject
{
    // BBox bbox; 
    ACCS_CLASS cls;                   
    cv_rect rect;
    float det_score;

    bool CheckWidthValid();
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

} DetObject;


typedef std::shared_ptr<DetectObject> DetObjectPtr;
typedef std::shared_ptr<const DetectObject> DetObjectConstPtr;

typedef std::vector<DetectObject> DetObjectList;

typedef std::vector<DetObjectPtr> DetObjectPtrList;
typedef std::vector<DetObjectConstPtr> DetObjectConstPtrList;