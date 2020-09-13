#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <iostream>
#include <string>


int main(int argc, char *argv[]) {
    cv::VideoCapture cap(0); //Создание потока ввода видеоданных с первой камеры

    struct timespec start, end;

    size_t count = 0;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    while (1) {
        cv::Mat frame; //Создание матрицы для кадра

        cap >> frame; //Считывание

        namedWindow("Original image", cv::WINDOW_AUTOSIZE); /*Создание окна для оригинального
        изображения*/

        imshow("Original image", frame);//Отображение кадра в этом окне

        cv::Mat gray, edge, draw; /*Создание матрицы серого цвета, матрицы границ и матрицы
        итогового изображения*/
        cvtColor(frame, gray, cv::COLOR_BGR2GRAY);/*Преобразование кадра из исходного
        цветового пространства в градации серого */

        Canny(gray, edge, 50, 150, 3);//Получение границ

        edge.convertTo(draw, CV_8U);

        namedWindow("Canny", cv::WINDOW_AUTOSIZE); //Создание окна

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        int result = static_cast<double>(count) / (end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec));

        cv::putText(draw, "About " + std::to_string(result) + " frames per sec.", cv::Point(300, 100), cv::FONT_HERSHEY_SIMPLEX,0.8, cv::Scalar(255, 255, 255) );

        imshow("Canny", draw);

        char c = cv::waitKey(33);//Ожидание нажатия клавиши в течение 33 миллисекунд
        ++count;
        if (c == 27) break;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    std::cout <<
    static_cast<int>(static_cast<double>(count) / (end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec)))
    << " frames per sec." << std::endl;

    std::cout << "Processed: " << count << " frames." << std::endl;

    return 0;
}