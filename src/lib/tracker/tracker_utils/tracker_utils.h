/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-06 14:16:11
 * @FilePath: /yolox_sort/src/lib/tracker/tracker_utils/tracker_utils.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>
#include "types/object_detected.h"
#include "types/object_tracked.h"
#include "types/base_macros.h"


static inline Eigen::VectorXd ConvertDetToObservation(DetectObject& obj)
{
    Eigen::VectorXd observation = Eigen::VectorXd::Zero(4);
    observation << obj.rect.x, obj.rect.y, obj.rect.area(), obj.rect.width / obj.rect.height;
    return observation;
}


static inline cv_rect ConvertObservationToRect(Eigen::VectorXd& x)
{
    cv_rect rect;
    rect.x = x[0];
    rect.y = x[1];
    rect.width = sqrt(x[2] * x[3]);
    rect.height = x[2] / rect.width;

    if(isnan(rect.width) || isnan(rect.height))
    {
        rect.width = 0.0;
        rect.height = 0.0;
    }    
    return rect;
}

static inline float iou_rect(const cv_rect& a, const cv_rect& b)
{
    cv_rect inter = a & b;
    float inter_area = inter.area();
    float union_area = a.area() + b.area() - inter_area;
    float iou = inter_area / union_area;

    return iou;
}

static inline float giou_rect(const cv_rect& a, const cv_rect& b)
{
    cv_rect inter = a & b;
    float inter_area = inter.area();
    float union_area = a.area() + b.area() - inter_area;
    float iou = inter_area / union_area;

    cv_rect convex = a | b;
    float convex_area = convex.area();
    float giou = iou - (convex_area - union_area) / convex_area;

    return giou;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//// plot image
/////////////////////////////////////////////////////////////////////////////////////////////////////

static void draw_trk_objects(cv::Mat& image, const TrkObjectList& objects)
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        const TrkObject& obj = objects[i];

        cv::Scalar color;
        switch (obj.cls)
        {
        case ACCS_HUMAN:
            color = cv::Scalar(165,79,255);     // red
            break;
        case ACCS_GUIDER:
            color = cv::Scalar(0,238,238);      // yellow
            break;
        case ACCS_SIGN1:
            color = cv::Scalar(147,20,255);     // pink
            break;
        default:
            color = cv::Scalar(205,205,180);    // gray
            break;
        }

        cv::Scalar txt_color = cv::Scalar(255,255,255);
        cv::rectangle(image, obj.rect, color, 3);

#ifdef DRAW_TRACK_TEXT_INFOS
        char text[256];
        sprintf(text, "%s_%d", obj.GetClassname().c_str(), obj.track_id);
        int baseLine = 0;
        float txt_size = 0.4;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, txt_size, 1, &baseLine);
        cv::Scalar txt_bk_color = color * 0.7;

        int x = obj.rect.x;
        int y = obj.rect.y + 1;
        //int y = obj.rect.y - label_size.height - baseLine;
        if (y > image.rows)
            y = image.rows;
        //if (x + label_size.width > image.cols)
            //x = image.cols - label_size.width;

        cv::rectangle(image, cv::Rect(cv::Point(x-1, y-label_size.height), cv::Size(label_size.width, label_size.height + baseLine)),
                      txt_bk_color, -1);

        cv::putText(image, text, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, txt_size, txt_color, 1);
#endif  //  DRAW_TRACK_TEXT_INFOS
    }
}
