
# 模型描述
.
├── zc_nano_480_640_pad.onnx                yolox-nano, 480*640, padding方式训练，best AP is 58.85
├── zc_nano_rep_480_640_nopad.onnx          yolox-nano-rep, 480*640, no-padding(直接resize)方式训练，best AP is 58.86
├── zc_nanox_rep_486_640_nopad.onnx         yolox-nanox-rep, 480*640, no-padding(直接resize)方式训练，best AP is 54.85
└── zc_y5_20230109_3cls.wts                 yolov5-s, wts, padding方式训练, best AP is 61.64
