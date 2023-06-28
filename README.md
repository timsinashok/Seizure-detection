# Seizure-detection
This program implements a system to detect nocturnal seizure on M5 Core 2, which is an ESP32 board. The M5 will be a wearable watch that  displays time and daily schedule. The device is designed for people who experience epileptic seiure during the night. It is designed to work as a normal watch during the day and a seizure detector when the host is asleep at night. The program leverages the accelerometer data that is received from the IMU sensor of the M5 and uses that to make the detection.

## Update: Implementation of Machine Learning 

Data was collected by simulating seizure and non-seizure cases using the [data acquisition script](https://github.com/timsinashok/Seizure-detection/blob/main/dataaquision.ino). The script collects data for 4 seconds where data is collected in all 3 axes 4 times every second. So we get 48 values of acceleration and then a label is provided for the data. 

The collected data is analyzed and then the dimension of the data is reduced by applying Principal Component Analysis(PCA). After the data is reduced to 2 or 3 dimensions, we can see two different clusters of data points which could easily be classified by a linear decision boundary. Different models are trained on the data and the SVM model with 10 estimators gives the maximum accuracy of 99%. 

## Relevant Links

[M5 Core 2 Documentation](https://docs.m5stack.com/en/core/core2) 

[Esp32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)

## Future Work

It would be more practical to train a time-series-based classification model because we get a continuous stream of data from the M5 board. The detection using a continuous stream of data will be more accurate and also the algorithm will be more efficient in detecting the episodes of seizure. 
