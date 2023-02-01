针对 ZC-ACCS,  在 Jetson-NX 上部署 OD+OT 任务,  代码链接见: [https://github.com/sean-wade/yolox_sort](https://github.com/sean-wade/yolox_sort)
其中训练多个模型并部署,  其推理速度如下表格所示:

| **模型名称** | **简介** | **推理速度(平均每帧)**
**未开 jetson_clocks** | **推理速度(平均每帧)**
**打开 jetson_clocks** | **模型 AP 值** |
| --- | --- | --- | --- | --- |
| **_zc_nano_480_640_pad_** |  yolox-nano模型, 图像padding 方式训练 |  21.6ms | 10.74ms | 58.85 |
| **_zc_nano_480_640_nopad_** | yolox-nano模型, 图像直接 resize 训练 |  |  |  |
| **_zc_nano_rep_480_640_nopad_** | yolox-nano, rep-style backbone, 图像直接 resize 训练 | 19.6ms | 9.74ms | 58.86 |
| **_zc_nanox_rep_486_640_nopad_** | yolox-nanox(width=0.125), rep-style backbone, 图像直接 resize 训练 | 17.9ms | 7.69ms | 54.85 |
| **_zc_y5_20230109_3cls_** | yolov5s, 图像 padding 方式训练 | 29.4ms | 23.72 | 61.64 |


