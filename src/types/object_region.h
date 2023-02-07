/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 14:17:28
 * @FilePath: /yolox_sort/src/types/object_region.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

#define REGION_OBJ_NUMS 5
#define TROLLY_MAX_OBJ_NUMS 5


struct ACCS_Point
{
    int x;
    int y;

    ACCS_Point() : x(0), y(0) {}
    ACCS_Point(int _x, int _y) : x(_x), y(_y) {}
};


struct ACCS_Polygon
{
    ACCS_Point point[30];
    int num_point = 0;
};


struct RegionObject
{
    // std::vector<cv::Point> pts;
    ACCS_Polygon poly;
    cv::Scalar color;
    cv::Point center;
    std::string name;
    bool valid = false;

    void GetCenter()
    {
        center.x = 0;
        center.y = 0;
        if(poly.num_point <= 0)
            return;

        for(int i=0; i < poly.num_point; i++)
        {
            center.x += poly.point[i].x;
            center.y += poly.point[i].y;
        }
        center.x /= poly.num_point;
        center.y /= poly.num_point;
    }
};


struct AccsRegions
{
    // human, guider region.
    RegionObject region_left_d;
    RegionObject region_left_s;
    RegionObject region_right_d;
    RegionObject region_right_s;
    RegionObject region_trolly;

    // sign1 region.
    RegionObject region_signl;
    RegionObject region_signr;
};


struct AccsResult
{
    // int channel;
    std::vector<int> nums_human;             // ld, ls, rd, rs, tr
    std::vector<int> nums_guider;            // ld, ls, rd, rs, tr
    std::vector<int> trolly_ys_human;        // tr
    std::vector<int> trolly_ys_guider;       // tr
    
    int sign1_num_left;
    int sign1_num_right;

    void Reset()
    {
        nums_human.clear();
        nums_guider.clear();
        trolly_ys_human.clear();
        trolly_ys_guider.clear();
        sign1_num_left = 0;
        sign1_num_right = 0;

        for(int i=0; i<REGION_OBJ_NUMS; i++)
        {
            nums_human.push_back(0);
            nums_guider.push_back(0);
        }
        
        // for(int i=0; i<TROLLY_MAX_OBJ_NUMS; i++)
        // {
        //     trolly_ys_human.push_back(0);
        //     trolly_ys_guider.push_back(0);
        // }
    }

    void GetAlarmString(std::string& obj_nums, std::string& obj_axis)
    {
        char obj_nums_char[30] = "";
        // 报文顺序: LD-LS-TR-RS-RD-SgL-SgR
        // 即 左减 human-guider, 左停 human-guider, 小车 human-guider, 右停 human-guider, 右减 human-guider, sign左, sign右
        sprintf(obj_nums_char, "%02d%02d%02d%02d%02d%02d%02d%02d%02d%02d %02d%02d", 
                    nums_human[0], nums_guider[0], 
                    nums_human[1], nums_guider[1], 
                    nums_human[4], nums_guider[4], 
                    nums_human[3], nums_guider[3], 
                    nums_human[2], nums_guider[2],
                    sign1_num_left, sign1_num_right
                );
        obj_nums = obj_nums_char;

        char obj_axis_char[30] = "";
        for(int i=0; i<TROLLY_MAX_OBJ_NUMS; i++)
        {
            if(trolly_ys_human.size() > 0)
            {
                int tmp_y = trolly_ys_human.back();
                trolly_ys_human.pop_back();
                sprintf(obj_axis_char, "%s%02d", obj_axis_char, tmp_y);
            }
            else
                sprintf(obj_axis_char, "%s-1", obj_axis_char);
        }
        for(int i=0; i<TROLLY_MAX_OBJ_NUMS; i++)
        {
            if(trolly_ys_guider.size() > 0)
            {
                int tmp_y = trolly_ys_guider.back();
                trolly_ys_guider.pop_back();
                sprintf(obj_axis_char, "%s%02d", obj_axis_char, tmp_y);
            }
            else
                sprintf(obj_axis_char, "%s-1", obj_axis_char);
        }
        obj_axis = obj_axis_char;
    }
};


inline int cvMat2Polygon(const cv::Mat& mat, ACCS_Polygon& poly)
{
	// std::cout << "mat.rows = " << mat.rows << ", mat.cols = " << mat.cols << std::endl;
    if(mat.rows < 2 || mat.rows % 2 != 0)
	{
		return -1;
	}
	
	cv::Mat dst = mat.reshape(2, 0);
    for(int i = 0; i < dst.rows; i++)
    {
        poly.point[i].x = dst.at<int>(i, 0);
        poly.point[i].y = dst.at<int>(i, 1);
		poly.num_point++;
    }

	return 0;
}

