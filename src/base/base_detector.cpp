/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-06 16:11:44
 * @FilePath: /yolox_sort/src/base/base_detector.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "base_detector.h"
// #include <sglog/sglog.h>


bool BaseDetector::_ParseOnnxToEngine()
{
    auto _builder = nvinfer1::createInferBuilder(m_logger_);
    if (!_builder)
    { 
        ERROR << "Builder not created !";
        return false;
    }

    const auto explicitBatch = 1U << static_cast<uint32_t>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    auto _network = _builder->createNetworkV2(explicitBatch);
    if (!_network)
    {
        ERROR << ("Network not created ! ");
        return false;
    }

    auto _config = _builder->createBuilderConfig();
    if (!_config)
    {
        ERROR << ("Config not created ! ");
        return false;
    }

    auto _parser = nvonnxparser::createParser(*_network, m_logger_);
    if (!_parser)
    {
        ERROR << ("Parser not created ! ");
        return false;
    }
    DEBUG << ("BaseDetector::ConstructNetwork start!");

    auto parsed = _parser->parseFromFile(m_options_.onnx_path.c_str(), static_cast<int>(nvinfer1::ILogger::Severity::kWARNING));
    if (!parsed)
    {
        ERROR << ("ox model cannot be parsed ! ");
        return false;
    }

    _builder->setMaxBatchSize(m_options_.batch_size);
    _config->setMaxWorkspaceSize(10 * (1 << 20));     // 8GB
    if (m_options_.ues_fp16)
    {
        _config->setFlag(nvinfer1::BuilderFlag::kFP16);
        WARN << ("Using FP16 mode, this may take several minutes... Please wait......");
    }
    else
    {
        WARN << ("Using FP32 mode.");
    }

    // printf("BaseDetector::buildEngineWithConfig start!");

    m_engine_ = _builder->buildEngineWithConfig(*_network, *_config);
    // m_engine_ = _builder->buildSerializedNetwork(*_network, *_config);

    if (!m_engine_)
    {
        ERROR << ("Engine cannot be built ! ");
        return false;
    }
    // printf("BaseDetector::Create Engine done!");

    // _parser->destroy();
    // _network->destroy();
    // _config->destroy();
    // _builder->destroy();
    // delete _parser;
    // delete _network;
    // delete _config;
    // delete _builder;

    _SerializeEngineToFile();
    // printf("BaseDetector::SerializeEngineToFile done!");

    // reload model, there's a bug if not reload.
    // m_engine_->destroy();
    // delete m_engine_;
    if(!_DeserializeEngineFromFile())
    {
        ERROR << ("Engine rebuild failed!");
        return false;
    }
    // printf("BaseDetector::DeserializeEngineFromFile done!");

    return true;
}


bool BaseDetector::_SerializeEngineToFile()
{
    if(m_options_.engine_path == "") 
    {
        WARN << ("Empty engine file name, skip save");
        return false;
    }
    
    if(m_engine_ != nullptr) 
    {
        DEBUG << "Saving engine to " << m_options_.engine_path;
        nvinfer1::IHostMemory* modelStream = m_engine_->serialize();
        std::ofstream file;
        file.open(m_options_.engine_path, std::ios::binary | std::ios::out);
        if(!file.is_open()) 
        {
            ERROR << ("Cannot write to engine file {}!");
            return false;
        }
        file.write((const char*)modelStream->data(), modelStream->size());
        file.close();
        // modelStream->destroy();
        // delete modelStream;
    } 
    else 
    {
        ERROR << ("Engine is empty, save engine failed");
        return false;
    }

    DEBUG << ("Saving engine succeed.");
    return true;
}


bool BaseDetector::_DeserializeEngineFromFile()
{
    cudaSetDevice(m_options_.gpu_id);
    char *_trtModelStream{nullptr};
    size_t size{0};

    std::ifstream file(m_options_.engine_path, std::ios::binary);
    if (file.good()) 
    {
        file.seekg(0, file.end);
        size = file.tellg();
        file.seekg(0, file.beg);
        _trtModelStream = new char[size];
        assert(_trtModelStream);
        file.read(_trtModelStream, size);
        file.close();
    }
    else
    {
        ERROR << "Cannot open engine path : " << m_options_.engine_path;
        return false;
    }

    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(m_logger_);
    if(!runtime) 
    {
        return false;
    }

    m_engine_ = runtime->deserializeCudaEngine(_trtModelStream, size);
    if(!m_engine_) 
    {
        return false;
    }

    m_context_ = m_engine_->createExecutionContext();
    if(!m_context_) 
    {
        return false;
    }
    
    delete[] _trtModelStream;

    // runtime->destroy();
    // delete runtime;
    return true;
}