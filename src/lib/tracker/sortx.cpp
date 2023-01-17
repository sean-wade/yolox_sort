/* 
 * @Author: zhanghao
 * @LastEditTime: 2022-11-10 19:07:36
 * @FilePath: /camera_traffic_light_pipeline/src/lib/tracker/sortx.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "sortx.h"


bool SortX::Init(const TrackerInitOptions &options)
{
    m_options_ = options;
    m_iou_threshold_ = m_options_.iou_threshold;
    m_use_giou_ = m_options_.use_giou;

    m_kf_trackers_.clear();
    m_matched_indexes_.clear();
    m_unmatched_det_indexes_.clear();
    m_unmatched_trk_indexes_.clear();

    m_frame_count_ = 0;
    m_timestamp_ = 0.0;
    
    return true;
}


bool SortX::Track(CameraFrame* camera_frame)
{
    if(camera_frame->timestamp - m_timestamp_ > 120 || camera_frame->timestamp - m_timestamp_ < 0)
    {
        Init(m_options_);
    }
    m_timestamp_ = camera_frame->timestamp;

    m_frame_count_++;
    camera_frame->trk_objects.clear();
    m_matched_indexes_.clear();
    m_unmatched_det_indexes_.clear();
    m_unmatched_trk_indexes_.clear();

    // std::cout << "Track1." << std::endl;
    for (auto &kf_tracker : m_kf_trackers_) 
    {
        kf_tracker.Predict();
    }

    // std::cout << "Track2." << std::endl;
    if(camera_frame->det_objects.size() != 0)
    {
        _Associate(camera_frame->det_objects);
    }

    // std::cout << "Track3." << std::endl;
    for (const auto &match_idx : m_matched_indexes_) 
    {
        m_kf_trackers_[match_idx.first].Update(camera_frame->det_objects[match_idx.second], camera_frame->timestamp);
    }

    // std::cout << "Track4." << std::endl;
    for (const auto &det_idx : m_unmatched_det_indexes_) 
    {
        KalmanBoxTrackerCV new_tracker(camera_frame->det_objects[det_idx], camera_frame->timestamp, m_options_.max_predict, m_options_.min_hits, m_options_.max_age);
        m_kf_trackers_.push_back(new_tracker);
    }

    // std::cout << "Track5." << std::endl;
    for (auto it = m_kf_trackers_.begin(); it != m_kf_trackers_.end();) 
    {
        TrackedObject trk_obj = it->GetState();
        if(it->CheckDelete()) 
        {
            it = m_kf_trackers_.erase(it);
        } 
        else 
        {
            if(it->CheckValid())
            {
                camera_frame->trk_objects.push_back(trk_obj);
            }
            it++;
        }
    }
    return true;
}


void SortX::_HungarianMatching(const std::vector<std::vector<float>>& iou_matrix,
                                size_t nrows, 
                                size_t ncols,
                                std::vector<std::vector<float>>& association) 
{
    Matrix<float> matrix(nrows, ncols);
    for (size_t i = 0 ; i < nrows ; i++) 
    {
        for (size_t j = 0 ; j < ncols ; j++) 
        {
            if (iou_matrix[i][j] != 0) 
            {
                matrix(i, j) = -iou_matrix[i][j];
            }
            else 
            {
                matrix(i, j) = 1.0f;
            }
        }
    }

    Munkres<float> m;
    m.solve(matrix);

    for (size_t i = 0 ; i < nrows ; i++) 
    {
        for (size_t j = 0 ; j < ncols ; j++) 
        {
            association[i][j] = matrix(i, j);
        }
    }
}


void SortX::_Associate(const DetObjectList& det_objs)
{
    if(m_kf_trackers_.empty())
    {
        for (size_t dd=0; dd < det_objs.size(); dd++) 
        {
            m_unmatched_det_indexes_.push_back(dd);
        }
        return;
    }

    std::vector<std::vector<float>> iou_matrix;
    iou_matrix.resize(det_objs.size(), std::vector<float>(m_kf_trackers_.size()));
    std::vector<std::vector<float>> association;
    association.resize(det_objs.size(), std::vector<float>(m_kf_trackers_.size()));

    for (size_t i = 0; i < det_objs.size(); i++) 
    {
        size_t j = 0;
        for (const auto& trk : m_kf_trackers_) 
        {
            if(m_use_giou_)
            {
                iou_matrix[i][j] = giou_rect(det_objs[i].rect, trk.GetPredict());
            }
            else
            {
                iou_matrix[i][j] = iou_rect(det_objs[i].rect, trk.GetPredict());
            }
            
            j++;
        }
    }

    _HungarianMatching(iou_matrix, det_objs.size(), m_kf_trackers_.size(), association);
    
    for (size_t i = 0; i < det_objs.size(); i++) 
    {
        bool matched_flag = false;
        for (size_t j = 0; j < m_kf_trackers_.size(); j++) 
        {
            if (0 == association[i][j]) 
            {
                if (iou_matrix[i][j] >= m_iou_threshold_) 
                {
                    m_matched_indexes_[j] = i;  // first : trk, second : det
                    matched_flag = true;
                }
                break;
            }
        }
        if (!matched_flag) 
        {
            m_unmatched_det_indexes_.push_back(i);
        }
    }

    for (size_t k = 0; k < m_kf_trackers_.size(); k++)
    {
        if(m_matched_indexes_.find(k) == m_matched_indexes_.end())
        {
            m_unmatched_trk_indexes_.push_back(k);
        }
    }

}