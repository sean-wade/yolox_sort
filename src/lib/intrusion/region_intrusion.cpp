/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 13:48:41
 * @FilePath: /yolox_sort/src/lib/intrusion/region_intrusion.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "region_intrusion.h"


bool IsPointInPolygon(const ACCS_Polygon &polygon, const ACCS_Point &point)
{
    // Ref: https://www.cnblogs.com/anningwang/p/7581545.html
    bool is_in = false;
    for (int i = 0, j = polygon.num_point - 1; i < polygon.num_point; j = i++) 
    {
        if (((polygon.point[i].y > point.y) != (polygon.point[j].y > point.y)) &&       // y is in [y_i, y_j) or [y_j, y_i)
            (point.x < (polygon.point[j].x - polygon.point[i].x) * (point.y - polygon.point[i].y) /
            (polygon.point[j].y - polygon.point[i].y) + polygon.point[i].x))            // (x,y) is below the (x_i, y_i) and (x_j, y_j) line
        {        
            is_in = !is_in;
        }
    }
    return is_in;
}


void drawPolyOnMask(cv::Mat& mask, const RegionObject region_obj)
{
    if(region_obj.valid)
    {
        std::vector<cv::Point> pts;
        for(int i=0; i<region_obj.poly.num_point; i++)
        {
            cv::Point tmp;
            tmp.x = region_obj.poly.point[i].x;
            tmp.y = region_obj.poly.point[i].y;
            pts.push_back(tmp);
        }
        
        std::vector<std::vector<cv::Point>> ppts_;
        ppts_.push_back(pts);

        cv::fillPoly(mask, ppts_, region_obj.color);
        cv::polylines(mask, pts, true, region_obj.color, 4); 

        cv::putText(mask, region_obj.name, region_obj.center, cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255,255,255), 2);
    }
}


bool RegionIntrusion::Init(const RegionIntrusionOptions &options)
{
    cvMat2Polygon(options.matA, m_accs_regions_.region_left_d.poly );
    cvMat2Polygon(options.matB, m_accs_regions_.region_left_s.poly );
    cvMat2Polygon(options.matC, m_accs_regions_.region_right_d.poly);
    cvMat2Polygon(options.matD, m_accs_regions_.region_right_s.poly);
    cvMat2Polygon(options.matE, m_accs_regions_.region_trolly.poly );
    cvMat2Polygon(options.matX, m_accs_regions_.region_signl.poly  );
    cvMat2Polygon(options.matY, m_accs_regions_.region_signr.poly  );

    m_accs_regions_.region_left_d.valid  = options.valid_A;
    m_accs_regions_.region_left_s.valid  = options.valid_B;
    m_accs_regions_.region_right_d.valid = options.valid_C;
    m_accs_regions_.region_right_s.valid = options.valid_D;
    m_accs_regions_.region_trolly.valid  = options.valid_E;
    m_accs_regions_.region_signl.valid   = options.valid_X;
    m_accs_regions_.region_signr.valid   = options.valid_Y;

    m_accs_regions_.region_left_d.GetCenter();
    m_accs_regions_.region_left_s.GetCenter();
    m_accs_regions_.region_right_d.GetCenter();
    m_accs_regions_.region_right_s.GetCenter();
    m_accs_regions_.region_trolly.GetCenter();
    m_accs_regions_.region_signl.GetCenter();
    m_accs_regions_.region_signr.GetCenter();

    m_accs_regions_.region_left_d.color  = cv::Scalar(128, 0, 0);
    m_accs_regions_.region_left_s.color  = cv::Scalar(0, 128, 0);
    m_accs_regions_.region_right_d.color = cv::Scalar(0, 0, 128);
    m_accs_regions_.region_right_s.color = cv::Scalar(128, 128, 0);
    m_accs_regions_.region_trolly.color  = cv::Scalar(128, 0, 128);
    m_accs_regions_.region_signl.color   = cv::Scalar(0, 56, 216);
    m_accs_regions_.region_signr.color   = cv::Scalar(216, 56, 0);

    m_accs_regions_.region_left_d.name  = "LD";
    m_accs_regions_.region_left_s.name  = "LS";
    m_accs_regions_.region_right_d.name = "RD";
    m_accs_regions_.region_right_s.name = "RS";
    m_accs_regions_.region_trolly.name  = "TR";
    m_accs_regions_.region_signl.name   = "SgL";
    m_accs_regions_.region_signr.name   = "SgR";

    m_options_ = options;
    get_mask_img();

    return true;
}


bool RegionIntrusion::Process(CameraFrame* camera_frame)
{
    camera_frame->accs_result.Reset();

    for(int i=0; i<camera_frame->trk_objects.size(); i++)
    {
        ACCS_Point foot_cur;
        foot_cur.x = camera_frame->trk_objects[i].GetPixelCX();
        foot_cur.y = camera_frame->trk_objects[i].rect.br().y;
        ACCS_CLASS cls_cur = camera_frame->trk_objects[i].cls;

        if(ACCS_HUMAN == cls_cur)
        {
            if(m_accs_regions_.region_left_d.valid && IsPointInPolygon(m_accs_regions_.region_left_d.poly, foot_cur))
            {
                camera_frame->accs_result.nums_human[0] += 1;
                continue;
            }
            if(m_accs_regions_.region_left_s.valid && IsPointInPolygon(m_accs_regions_.region_left_s.poly, foot_cur))
            {
                camera_frame->accs_result.nums_human[1] += 1;
                continue;
            }
            if(m_accs_regions_.region_right_d.valid && IsPointInPolygon(m_accs_regions_.region_right_d.poly, foot_cur))
            {
                camera_frame->accs_result.nums_human[2] += 1;
                continue;
            }
            if(m_accs_regions_.region_right_s.valid && IsPointInPolygon(m_accs_regions_.region_right_s.poly, foot_cur))
            {
                camera_frame->accs_result.nums_human[3] += 1;
                continue;
            }
            if(m_accs_regions_.region_trolly.valid && IsPointInPolygon(m_accs_regions_.region_trolly.poly, foot_cur))
            {
                camera_frame->accs_result.nums_human[4] += 1;
                int y_rate = (int)(foot_cur.y * 100 / m_options_.img_height);
                y_rate = (y_rate < 1) ? 1 : y_rate;
                camera_frame->accs_result.trolly_ys_human.push_back(y_rate);
                continue;
            }
        }
        else if(ACCS_GUIDER == cls_cur)
        {
            if(m_accs_regions_.region_left_d.valid && IsPointInPolygon(m_accs_regions_.region_left_d.poly, foot_cur))
            {
                camera_frame->accs_result.nums_guider[0] += 1;
                continue;
            }
            if(m_accs_regions_.region_left_s.valid && IsPointInPolygon(m_accs_regions_.region_left_s.poly, foot_cur))
            {
                camera_frame->accs_result.nums_guider[1] += 1;
                continue;
            }
            if(m_accs_regions_.region_right_d.valid && IsPointInPolygon(m_accs_regions_.region_right_d.poly, foot_cur))
            {
                camera_frame->accs_result.nums_guider[2] += 1;
                continue;
            }
            if(m_accs_regions_.region_right_s.valid && IsPointInPolygon(m_accs_regions_.region_right_s.poly, foot_cur))
            {
                camera_frame->accs_result.nums_guider[3] += 1;
                continue;
            }
            if(m_accs_regions_.region_trolly.valid && IsPointInPolygon(m_accs_regions_.region_trolly.poly, foot_cur))
            {
                camera_frame->accs_result.nums_guider[4] += 1;
                int y_rate = (int)(foot_cur.y * 100 / m_options_.img_height);
                y_rate = (y_rate < 1) ? 1 : y_rate;
                camera_frame->accs_result.trolly_ys_guider.push_back(y_rate);
                continue;
            }
        }
        else if(ACCS_SIGN1 == cls_cur)
        {
            if(m_accs_regions_.region_signl.valid && IsPointInPolygon(m_accs_regions_.region_signl.poly, foot_cur))
            {
                camera_frame->accs_result.sign1_num_left += 1;
                continue;
            }
            if(m_accs_regions_.region_signr.valid && IsPointInPolygon(m_accs_regions_.region_signr.poly, foot_cur))
            {
                camera_frame->accs_result.sign1_num_right += 1;
                continue;
            }
        }
    }

    if(m_options_.plot_region)
    {
        cv::addWeighted(m_region_mask_, 0.4, *camera_frame->image_ptr, 1, 0, *camera_frame->image_ptr);
    }

    return true;
}


void RegionIntrusion::get_mask_img()
{
    m_region_mask_ = cv::Mat(m_options_.img_height, m_options_.img_width, CV_8UC3, cv::Scalar(0,0,0));

    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_left_d);
    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_left_s);
    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_right_d);
    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_right_s);
    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_trolly);
    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_signl);
    drawPolyOnMask(m_region_mask_, m_accs_regions_.region_signr);
}