#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    cv::Mat src = cv::imread("light_test.jpg");
    if (src.empty()) { std::cerr << "读图失败\n"; return -1; }
    std::cout << "图像尺寸: " << src.cols << " x " << src.rows << "\n";

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask1, mask2, mask;
    cv::inRange(hsv, cv::Scalar(0,   100, 100), cv::Scalar(10,  255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(170, 100, 100), cv::Scalar(180, 255, 255), mask2);
    cv::bitwise_or(mask1, mask2, mask);

    int n1 = cv::countNonZero(mask1);
    int n2 = cv::countNonZero(mask2);
    int n  = cv::countNonZero(mask);
    double total = (double)mask.total();

    std::cout << "\n===== 阈值分割结果 =====\n";
    std::cout << "红色低段 (H 0-10):    " << n1 << " 像素\n";
    std::cout << "红色高段 (H 170-180): " << n2 << " 像素\n";
    std::cout << "合并后目标区域:       " << n  << " 像素 (" << 100.0 * n / total << "%)\n";

    cv::imwrite("mask.jpg", mask);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::cout << "轮廓总数: " << contours.size() << "\n";

    cv::Mat result = src.clone();
    int count = 0;
    for (const auto& c : contours) {
        double area = cv::contourArea(c);
        if (area < 60) continue;

        cv::Rect box = cv::boundingRect(c);
        cv::rectangle(result, box, cv::Scalar(0, 255, 0), 2);

        std::cout << "目标 " << ++count
                  << "  面积=" << area
                  << "  中心=(" << box.x + box.width / 2 << "," << box.y + box.height / 2 << ")"
                  << std::endl;
    }
    std::cout << "最终检出: " << count << " 个\n";

    cv::imwrite("result.jpg", result);
    cv::imshow("mask", mask);
    cv::imshow("result", result);
    cv::waitKey(0);
    return 0;
}