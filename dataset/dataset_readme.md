# Ambulance Detection Dataset

This dataset contains images used to train the YOLO model for ambulance detection.

## Dataset Structure

dataset
│
├── train
│   ├── images
│   └── labels
│
├── valid
│   ├── images
│   └── labels
│
└── test
    ├── images
    └── labels

## Annotation Format

The dataset uses **YOLO format annotations**.

Example label file:

0 0.52 0.43 0.31 0.25

Where:

0 → ambulance class  
0.52 → x center  
0.43 → y center  
0.31 → width  
0.25 → height  

## Classes

| Class ID | Object |
|--------|--------|
| 0 | Ambulance |

## Dataset Sources

Images were collected from:

- Public traffic datasets
- Open image datasets
- Custom images captured from traffic cameras

## Recommended Dataset Size

For best results:

- Training images: 1000+
- Validation images: 200+
- Test images: 200+

## Training Command

Example YOLOv5 training command:

python train.py --img 640 --batch 16 --epochs 100 --data data.yaml --weights yolov5s.pt