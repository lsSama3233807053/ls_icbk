#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

struct BarInfo{
    	cv::RotatedRect rect;
    	float angle;
    	float height;
    	cv::Point2f center;
    	float y_top,y_bottom;
    	float width;
    	};
int main() {
    cv::Mat src = cv::imread("test04.jpg");
    if (src.empty()) { std::cerr << "读图失败\n"; return -1; }
    std::cout << "图像尺寸: " << src.cols << " x " << src.rows << "\n";

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask1, mask2, mask;
    cv::inRange(hsv, cv::Scalar(0,   180, 0), cv::Scalar(60,  255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(90, 120, 150), cv::Scalar(90, 120, 150), mask2);
    cv::bitwise_or(mask1, mask2, mask);

    int n1 = cv::countNonZero(mask1);
    int n2 = cv::countNonZero(mask2);
    int n  = cv::countNonZero(mask);
    double total = (double)mask.total();

    std::cout << "\n===== 阈值分割结果 =====\n";
    std::cout << "红色阈值 (H 0-60):    " << n1 << " 像素\n";
    std::cout << "占全图: " << 100*n1/total<< " %\n";
   

    cv::imwrite("mask04.jpg", mask);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::cout << "轮廓总数: " << contours.size() << "\n";

    cv::Mat result = src.clone();
    
    std::vector<BarInfo>bars;
    int count = 0;
    for (const auto& c : contours) {
        double area = cv::contourArea(c);
        if (area < 1000) continue;
        if(area>30000)continue;
                cv::Rect box = cv::boundingRect(c);
        float ratio = (float)std::max(box.width, box.height) /
                      std::min(box.width, box.height);
        if (ratio < 1.2f) continue;
        cv::RotatedRect rr = cv::minAreaRect(c);
                // 角度筛选：灯条应接近竖直，剔除明显横向的干扰
        float w = rr.size.width, h = rr.size.height;
        float a = std::min(rr.size.width, rr.size.height);
	float b = std::max(rr.size.width, rr.size.height);
	if (b / a < 1.5f) continue;
        
        cv::Point2f pts[4];
        rr.points(pts);
        for (int i = 0; i < 4; ++i)
            cv::line(result, pts[i], pts[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);

        std::cout << "目标 " << ++count
                  << "  面积=" << area
                  << "  中心=(" << box.x + box.width / 2 << "," << box.y + box.height / 2 << ")"
                  << std::endl;
        float yTop=pts[0].y,yBottom=pts[0].y;
        for(int i=1;i<4;i++){
        	yTop=std::min(yTop,pts[i].y);
        	yBottom=std::max(yBottom,pts[i].y);
        }
        //存入列表
        BarInfo bi;
        bi.rect=rr;
        bi.angle=rr.angle;
        bi.height=b;
        bi.width=a;
        bi.center=rr.center;
        bi.y_top=yTop;
        bi.y_bottom=yBottom;
        bars.push_back(bi);
        
        std::cout<<"灯条"<<++count
		 <<"中心=("<<bi.center.x<<","<<bi.center.y<<")"
		 <<"长="<<b<<"宽"<<a
		 <<"角度="<<bi.angle
		 <<"top="<<yTop<<"bottom="<<yBottom<<"\n";       	
    }
    std::cout << "最终检出: " << count << " 个\n";
    //灯条配对
    std::vector<std::pair<int,int>>pairs;
    
    for(size_t i=0;i<bars.size();++i){
    	for(size_t j=i+1;j<bars.size();++j){
    		const BarInfo& A=bars[i];
    		const BarInfo& B=bars[j];
    		//先是需要接近平行
    		if(std::abs(A.angle-B.angle)>15.0f)continue;
    		//然后高度要相近，min和max函数能直接引用
    		float hMin=std::min(A.height,B.height);
    		float hMax=std::max(A.height,B.height);
    		if(hMax/hMin>1.5f)continue;
    		//最后是水平间距要比较合理
    		float dx=std::abs(A.center.x-B.center.x);
    		float dy=std::abs(A.center.y-B.center.y);
    		float hAvg=(A.height+B.height)/2.0f;
    		if(dx<hAvg*0.8f)continue;
    		if(dx>hAvg*5.0f)continue;
    		//连线还要接近水平
    		if (dy > dx * 0.4f) continue;

            pairs.push_back({(int)i, (int)j});
            std::cout << "配对: 灯条" << (i + 1) << " + 灯条" << (j + 1)
                      << "  dx=" << dx << "  dy=" << dy << "\n";
        }
    }
    std::cout << "共配出 " << pairs.size() << " 对\n";
    //装甲板ROI
    int plateId = 0;

    for (auto& p : pairs) {
        const BarInfo& A = bars[p.first];
        const BarInfo& B = bars[p.second];

        float margin = 15.0f;

        float x1 = std::min(A.center.x, B.center.x);
        float x2 = std::max(A.center.x, B.center.x);
        float y1 = std::min(A.y_top,    B.y_top)    - margin;
        float y2 = std::max(A.y_bottom, B.y_bottom) + margin;

        // 防越界
        x1 = std::max(x1, 0.0f);                         x2 = std::min(x2, (float)src.cols - 1);
        y1 = std::max(y1, 0.0f);                         y2 = std::min(y2, (float)src.rows - 1);
        if (x2 <= x1 || y2 <= y1) continue;

        cv::Rect plate(cv::Point((int)x1, (int)y1), cv::Point((int)x2, (int)y2));

        // 画装甲板框（黄色粗线）
        cv::rectangle(result, plate, cv::Scalar(0, 255, 255), 3);

        // 标编号
        char label[32];
        snprintf(label, sizeof(label), "armor_%d", ++plateId);
        cv::putText(result, label, cv::Point(plate.x, std::max(plate.y - 8, 15)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);

        // 导出 ROI
        cv::Mat roi = src(plate).clone();
        char fname[64];
        snprintf(fname, sizeof(fname), "armor_roi_%d.jpg", plateId);
        cv::imwrite(fname, roi);

        std::cout << "装甲板 " << plateId
                  << "  ROI=(" << plate.x << "," << plate.y
                  << "," << plate.width << "x" << plate.height << ")"
                  << "  已保存 " << fname << "\n";
    }
    		

    cv::imwrite("result04.jpg", result);
    cv::imshow("mask04", mask);
    cv::imshow("result04", result);
    cv::waitKey(0);
    return 0;
}
