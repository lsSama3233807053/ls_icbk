#include <opencv2/opencv.hpp>
#include <iostream>

static cv::Mat g_hsv;

int main() {
    cv::Mat src = cv::imread("test04.jpg");
    if (src.empty()) {
        std::cerr << "读图失败:test04.jpg\n";
        return -1;
    }
    std::cout << "图像: " << src.cols << " x " << src.rows << "\n";

    cv::cvtColor(src, g_hsv, cv::COLOR_BGR2HSV);

    int hmin = 90, smin = 180, vmin = 200;
    int hmax = 140, smax = 255, vmax = 255;

    const char* W = "HSV Tool (ESC = 输出参数)";
    cv::namedWindow(W, cv::WINDOW_NORMAL);
    cv::resizeWindow(W, 600, 300);

    cv::createTrackbar("H min (0-179)", W, &hmin, 179);
    cv::createTrackbar("S min", W, &smin, 255);
    cv::createTrackbar("V min", W, &vmin, 255);
    cv::createTrackbar("H max (0-179)", W, &hmax, 179);
    cv::createTrackbar("S max", W, &smax, 255);
    cv::createTrackbar("V max", W, &vmax, 255);

    // 图片太大就缩小显示
    double scale = std::min(1.0, 900.0 / std::max(src.cols, src.rows));
    cv::Mat src_small, mask_small;

    double total = (double)(src.cols * src.rows);

    while (true) {
        cv::Mat mask;
        cv::inRange(g_hsv,
                    cv::Scalar(hmin, smin, vmin),
                    cv::Scalar(hmax, smax, vmax),
                    mask);

        int nz = cv::countNonZero(mask);

        std::cout << "\r下限(" << hmin << "," << smin << "," << vmin << ")"
                  << "  上限(" << hmax << "," << smax << "," << vmax << ")"
                  << "  选中 " << nz << " 像素 (" << 100.0 * nz / total << "%)   "
                  << std::flush;

        if (scale < 1.0) {
            cv::resize(src,  src_small,  cv::Size(), scale, scale);
            cv::resize(mask, mask_small, cv::Size(), scale, scale);
            cv::imshow("src",  src_small);
            cv::imshow("mask", mask_small);
        } else {
            cv::imshow("src",  src);
            cv::imshow("mask", mask);
        }

        int key = cv::waitKey(30);
        if (key == 27) break;      // ESC 退出
    }

    std::cout << "\n\n";
    std::cout << "=====================================\n";
    std::cout << "  最终参数（直接粘进 autoaim.cpp）\n";
    std::cout << "=====================================\n";
    std::cout << "cv::Scalar(" << hmin << ", " << smin << ", " << vmin << ")\n";
    std::cout << "cv::Scalar(" << hmax << ", " << smax << ", " << vmax << ")\n";
    std::cout << "\n完整写法:\n";
    std::cout << "cv::inRange(hsv, cv::Scalar(" << hmin << ", " << smin << ", " << vmin
              << "), cv::Scalar(" << hmax << ", " << smax << ", " << vmax << "), mask);\n";

    return 0;
}
