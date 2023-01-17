/* 
 * @Author: zhanghao
 * @LastEditTime: 2022-11-10 19:04:36
 * @FilePath: /camera_traffic_light_pipeline/src/lib/tracker/sortx.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once

#include "base_tracker.h"
#include "kalman_box_trakcker.h"
#include "tracker_utils/munkres.h"
#include "tracker_utils/tracker_utils.h"


class SortX : public BaseTracker
{
    public:
        SortX() = default;
        virtual ~SortX() = default;

        /* 
         * @description: Init the tracker by options.
         * @param {TrackerInitOptions} &options
         * @return {status}
         * @author: zhanghao
         */
        virtual bool Init(const TrackerInitOptions &options);


        /* 
         * @description: Do track.
         * @param {CameraFrame*} camera_frame
         * @return {status}
         * @author: zhanghao
         */
        virtual bool Track(CameraFrame* camera_frame);
        

        /* 
         * @description: Name of this tracker
         * @return {Tracker name}
         * @author: zhanghao
         */
        virtual std::string Name() const{
            return "sortx";
        };


    private:
        void _HungarianMatching(const std::vector<std::vector<float>>& iou_matrix,
                                size_t nrows, 
                                size_t ncols,
                                std::vector<std::vector<float>>& association);

        void _Associate(const DetObjectList& det_objs);


    private:
        TrackerInitOptions m_options_;

        bool m_use_giou_;
        double m_timestamp_;
        float m_iou_threshold_;
        unsigned int m_frame_count_;

        std::vector<KalmanBoxTrackerCV> m_kf_trackers_;

        std::map<int, int> m_matched_indexes_;
        std::vector<int> m_unmatched_det_indexes_;
        std::vector<int> m_unmatched_trk_indexes_;
};
