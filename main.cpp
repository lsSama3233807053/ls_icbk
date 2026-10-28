#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    // ========== 基础部分 ==========
    Mat src = imread("/mnt/hgfs/opencv_share/test.jpg");
    if(src.empty()){
        cout << "图片读取失败！检查路径、文件名" << endl;
        return -1;
    }
    cout << "====原始图像信息====" << endl;
    cout << "宽：" << src.cols << " 高：" << src.rows << " 通道数：" << src.channels() << endl;

    // 转灰度图
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    imwrite("/mnt/hgfs/opencv_share/gray.jpg", gray);
    int gray_nonzero = countNonZero(gray);
    cout << "灰度图非零像素数量：" << gray_nonzero << endl;

    // 阈值二值化
    Mat binary;
    threshold(gray, binary, 127, 255, THRESH_BINARY);
    imwrite("/mnt/hgfs/opencv_share/binary.jpg", binary);
    int binary_nonzero = countNonZero(binary);
    cout << "二值图非零像素（白色）数量：" << binary_nonzero << endl;

    // ========== 进阶① ROI裁剪 ==========
    Rect roi_rect(50, 50, 200, 200);
    Mat roi_img = src(roi_rect);
    imwrite("/mnt/hgfs/opencv_share/roi.jpg", roi_img);
    cout << "【ROI裁剪】裁剪图像宽：" << roi_img.cols << "，高：" << roi_img.rows << endl;
    imshow("ROI裁剪图", roi_img);

    // ========== 进阶② 图像缩放 ==========
    Mat resize_img;
    resize(src, resize_img, Size(200, 200));
    imwrite("/mnt/hgfs/opencv_share/resize.jpg", resize_img);
    cout << "【缩放】缩放后图像宽：" << resize_img.cols << "，高：" << resize_img.rows << endl;
    imshow("缩放图", resize_img);

    // ========== 进阶③ HSV 筛选黄色 ==========
    Mat hsv, yellow_mask;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    Scalar lower_yellow = Scalar(20, 100, 100);
    Scalar upper_yellow = Scalar(30, 255, 255);
    inRange(hsv, lower_yellow, upper_yellow, yellow_mask);
    imwrite("/mnt/hgfs/opencv_share/hsv_yellow_mask.jpg", yellow_mask);
    int yellow_nonzero = countNonZero(yellow_mask);
    cout << "【HSV黄色筛选】黄色区域非零像素：" << yellow_nonzero << endl;
    imshow("黄色掩码图", yellow_mask);

    // ========== 进阶④ 高斯模糊 + 二值化对比 ==========
    Mat blur_gray, blur_binary;
    GaussianBlur(gray, blur_gray, Size(5,5), 0);
    threshold(blur_gray, blur_binary, 127, 255, THRESH_BINARY);
    imwrite("/mnt/hgfs/opencv_share/blur_binary.jpg", blur_binary);
    int blur_bin_nonzero = countNonZero(blur_binary);
    cout << "【高斯模糊后二值】非零像素：" << blur_bin_nonzero << endl;
    imshow("模糊后二值图", blur_binary);

    // ========== 进阶⑤ Canny边缘检测 ==========
    Mat edge;
    Canny(gray, edge, 80, 160);
    imwrite("/mnt/hgfs/opencv_share/canny_edge.jpg", edge);
    int edge_nonzero = countNonZero(edge);
    cout << "【Canny边缘】边缘非零像素：" << edge_nonzero << endl;
    imshow("Canny边缘图", edge);

    // 基础窗口
    imshow("原图", src);
    imshow("灰度图", gray);
    imshow("二值图", binary);

    waitKey(0);
    destroyAllWindows();
    return 0;
}