import cv2
import argparse

from ultralytics import YOLO
import supervision as sv

def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="YOLOv8 Live")
    parser.add_argument(
        "--WebCam-resolution",
        default=[1200,720],
        nargs=2,
        type=int
    )
    args = parser.parse_args()
    return args


def main():
    args = parse_arguments()
    frame_width, frame_height = args.WebCam_resolution

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, frame_width) 
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)

    model = YOLO("yolov8n.pt")

    #box_annotator = sv.BoundingBoxAnnotator(
     #   thickness=2
    #)


    while True:
        ret, frame = cap.read()
        #cv2.imshow("yolov8", frame)

        result = model(frame)[0]
        #detections = sv.Detections.from_yolov8(result)
        #frame = box_annotator.annotate(scene=frame, detections=detections)

        cv2.imshow("yolov8", frame)

        if(cv2.waitKey(30) == 27):
            break


if __name__ == "__main__":
    main()
