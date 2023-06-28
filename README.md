# Seizure-detection
This program implements a system to detect nocturnal seizure on M5 Core 2, which is an ESP32 board. The M5 will be a wearable watch that  displays time and daily schedule. The device is basically designed for people who expereince epileptic seiure during the night. It is designed to work as a normal watch during the day and seizure detector when the host is asleep at night. The program leverages the accelerometer data that is received from the IMU sensor of the M5 and uses that to make the detection.

## Update : Implementation of Machine Learning 

Data was collected by simulating seizure and non-seizure cases using the [data aquisition script](). The script collects data for the duration of 4 seconds where data is collected in all 3 axes 4 times every second. So we get 48 values of acceleration and then a label is provided for the data. 

The collcted data is analyzed and then the dimension of the data is reduced by applying Princiapal Component Analysis(PCA). After the data is reduced to 2 or 3 dimensions, we can see a two different clusters of data points which could easily be classified by a linear decisioon boundary. Different models are trained on the data and SVM model with 10 estimators give the maximum accuracy of 99%. 

## Relevant Links

[M5 Core 2 Documentation]()
[Esp32 Documentation]()

## Future Work

It would be more practical to train a time-series based classification model because we get continuous stream of data from the M5 board. The detection using continuous stream of data will be more accurate and also the algoritm will be more effecient in detecting the episodes of seizure. 