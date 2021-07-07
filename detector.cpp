#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  

using namespace std;
using namespace cv;

CascadeClassifier peopele;
CascadeClassifier cars;
CascadeClassifier half_peopele;
using namespace cv;
/////////////////////
Point getCenterPoint(Rect rect)
{
	Point cpt;
	cpt.x = rect.x + cvRound(rect.width / 2.0);
	cpt.y = rect.y + cvRound(rect.height / 2.0);
	return cpt;
}
///////////////////////

int main()
{
	////////////////////////////////////////////
	peopele.load("haarcascade_fullbody.xml");   //���ط�������ע���ļ�·��
	cars.load("haarcascade_cars2.xml");
	half_peopele.load("haarcascade_upperbody.xml");
	//��1��������Ƶ
	VideoCapture cap;
	//cap.open(0);   //������ͷ  
	VideoCapture capture("road6.avi");//road3.avi road4.mp4
	
	
	Point root_points[1][6];                                  //�����������ݲ�ͬ�ķֱ���ѡ���С
	root_points[0][0] = Point(0, 0);
	root_points[0][1] = Point(0, 280);
	root_points[0][2] = Point(213, 220);
	root_points[0][3] = Point(426, 220);
	root_points[0][4] = Point(640, 280);
	root_points[0][5] = Point(640, 0);

	Mat dstImage, edge, grayImage;                           //������srcͬ���ͺʹ�С�ľ���(dst)

	////////////////////////////////////////////
	std::vector<Rect> peopele1;                              //��������Ȧ���˵ķ���
	std::vector<Rect> cars1;
	std::vector<Rect> half_peopele1;
	/////////////////////////////////
	const Point* ppt[1] = { root_points[0] };                //ָ�����ε�����ָ��
	int npt[] = { 6 };                                       //����ζ������������

	vector<Vec4i> lines;                                     //����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������
	Vec4f l = NULL;                                          //�������ֱ����Ϣ����� line �У�Ϊ cv::Vec4f ���͡�
	Vec4f line1 = NULL;
	Vec4f line2 = NULL;

	while (1)                                                //ѭ����ʾÿһ֡
	{
		Mat frame,edge1;                                     //����һ��Mat���������ڴ洢ÿһ֡��ͼ��;edge1�������Haar
	    capture >> frame;                                    //��ȡ��ǰ֡
	//	cap >> frame;	  
		dstImage.create(frame.size(), frame.type());         //������srcͬ���ͺʹ�С�ľ���(dst)
		
		cvtColor(frame, grayImage, CV_BGR2GRAY);             //��ԭͼ��ת��Ϊ�Ҷ�ͼ��
	
		GaussianBlur(grayImage, edge, Size(5, 5), 0, 0);     //����ʹ�� 5x5�ں�������
	  //blur(grayImage, edge, Size(5, 5));				     //blur�������Ƕ������ͼ��src���о�ֵ�˲�����dst�����

//		Canny(grayImage, edge1, 50, 150, 3);                 //Haar����õ�
	 	Canny(edge, edge, 30, 180, 3);                       //����Canny����;���Ըĸ���ֵ����Ч��һ��
	//////////////////////////////////////
	    cars.detectMultiScale(grayImage, cars1, 1.2, 6, 0, Size(0, 0));                //��⳵
		peopele.detectMultiScale(grayImage, peopele1, 1.2, 6, 0, Size(0, 0));           //�����
	//	eyerCascade.detectMultiScale(grayImage, eyer, 1.2, 6, 0, Size(0, 0));           //������ϰ���
    //////////////////////////////////////////

		//��5���������
		polylines(edge, ppt, npt, 1, 1, Scalar(255, 0, 0), 1, 8, 0);                    //���ƶ����polylines
		fillPoly(edge, ppt, npt, 1, Scalar(0));                                         //��������fillPoly
		
    	//Rect ROI(0, 100, width / 2, height / 2);                                      //��ȡROI

		HoughLinesP(edge, lines, 1, CV_PI / 180, 5, 40, 20);                           //���ʻ���任�����ֱ�ߡ���HoughLinesP�����������ֵĬ��15

		vector<Point2f> points;                                                         //???????????
		vector<Point2f> points2;

		float avr1 = 0.0;                                                               //????????????
		float avr2 = 0.0;
		
		for (size_t i = 0; i < lines.size(); i++)                                       //������ͼ�л��Ƴ�ÿ���߶�,linesΪ�������
		{
			l = lines[i];                                                               // Vec4f l = NULL;     
			printf("б�ʣ� %f \r\n", (l[3] - l[1]) / (l[2] - l[0]));
			
			if ((l[3] - l[1]) / (l[2] - l[0])>0.5 && (l[3] - l[1]) / (l[2] - l[0])<1)   //��ֱ��б�ʣ�l[3] - l[1]) / (l[2] - l[0])>0.5����(l[3] - l[1]) / (l[2] - l[0])<1
			{
				avr1 += (l[3] - l[1]) / (l[2] - l[0]);                                  //avrl=avrl+б��
				printf("--(%f,%f),(%f,%f)\r\n", l[0], l[1], l[2], l[3]);                //
				points.push_back(Point2f(l[0], l[1]));                                  //push_back��������˼����vector��ĩβ����һ��Ԫ�ء�
				points.push_back(Point2f(l[2], l[3]));
			}
			else if ((l[3] - l[1]) / (l[2] - l[0])<-0.5 && (l[3] - l[1]) / (l[2] - l[0])>-1)
			{                                                                           //���ұ�б��
				avr2 += (l[3] - l[1]) / (l[2] - l[0]);
				printf("**(%f,%f),(%f,%f)\r\n", l[0], l[1], l[2], l[3]);
				points2.push_back(Point2f(l[0], l[1]));
				points2.push_back(Point2f(l[2], l[3]));
			}
			for (int i = 0; i < points.size(); i++)                                     // auto�Զ������ƶ�
				circle(frame, cvPoint(points.at(i).x, points.at(i).y), 3, Scalar(0, 255, 0));//cvPoint(points.at(i).x, points.at(i).y)ȡ��Բ������
			for (int i = 0; i < points2.size(); i++)
				circle(frame, cvPoint(points2.at(i).x, points2.at(i).y), 3, Scalar(255, 255, 0));//�뾶Ϊ3
			//cv::line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
		}
		printf("%f,%f \r\n", avr1, avr2);                                                //���б��
//////////////////////////////////////////��ϵ�һ��ֱ�ߺͲ�������
		fitLine(Mat(points), line1, CV_DIST_L1, 0, 0.01, 0.01);                          //ֱ�����
		double k = line1[1] / line1[0];                                                  //�õ�б�ʣ�
		double step = 350;                                                               //�ؾࣿ
		cvtColor(edge, dstImage, CV_GRAY2BGR);                                           //CV_RGB2GRAY��<��ɫͼ��---�Ҷ�ͼ��>
		cv::line(dstImage, cvPoint(line1[2] - step, line1[3] - k*step), cvPoint(line1[2] + step, k*step + line1[3]), Scalar(0, 0, 255), 5);//
		//cvPoint(line1[2] - step, line1[3] - k*step)ֱ����㣻cvPoint(line1[2] + step, k*step + line1[3])   ֱ���յ㣻
		//cv::line(frame, Point(line1[0], line1[1]), Point(line1[2], line1[3]), Scalar(0, 0, 255), 1, CV_AA);
//////////////////////////////////////////��ϵڶ���
		fitLine(Mat(points2), line2, CV_DIST_L1, 0, 0.01, 0.01);
		k = line2[1] / line2[0];
		cv::line(dstImage, cvPoint(line2[2] - step, line2[3] - k*step), cvPoint(line2[2] + step, k*step + line2[3]), Scalar(0, 0, 255), 5);
		//cv::line(frame, Point(line2[0], line2[1]), Point(line2[2], line2[3]), Scalar(0, 0, 255), 1, CV_AA);
//////////////////////////////////////////
    	fillPoly(dstImage, ppt, npt, 1, Scalar(0));//���ƶ����polylines(ppt);  Scalar(0)  �Ҷ�ֵ0Ϊ��1Ϊ�ף�
		
		//cvtColor(edge, dstImage, CV_GRAY2BGR);
		addWeighted(frame, 0.8, dstImage, 1, 0.0, dstImage);//ͼ����ӣ��Ѷ���ε�����ԭͼ����
////////////////////////////////////////�������Ŀ��
	//	char *strID; //��ʾ����
	//	CvFont font;
	//	strID = "Car";
		if (cars1.size()>0)
		{
			for (int i = 0; i<cars1.size(); i++)
			{
				rectangle(dstImage, Point(cars1[i].x, cars1[i].y), Point(cars1[i].x + cars1[i].width, cars1[i].y + cars1[i].height),//dstImage or frame?
					Scalar(0, 255, 0), 1, 8);    //�������λ��  
				//cvPutText(dstImage, strID, cvPoint(cars1[i].x, cars1[i].y - 10), &font, CV_RGB(255, 0, 0));//��ɫ����ע�� ��������Mat
			//printf("����λ�ã�%Point,%Point \r\n", cars1[i].x+0.5*cars1[i].width, cars1[i].y+0.5*cars1[i].height);
				cout << "����λ�ã�" << cars1[i].x + 0.5*cars1[i].width << "  "<< cars1[i].y + 0.5*cars1[i].height<< endl;
			}
		}
		if (peopele1.size()>0)
		{
			for (int i = 0; i<peopele1.size(); i++)
			{
				rectangle(dstImage, Point(peopele1[i].x, peopele1[i].y), Point(peopele1[i].x + peopele1[i].width, peopele1[i].y + peopele1[i].height),//dstImage or frame?
					Scalar(0, 0, 255), 1, 8);    //�����λ��  
				cout << "����λ�ã�" << peopele1[i].x + 0.5*peopele1[i].width << "  " << peopele1[i].y + 0.5*peopele1[i].height << endl;
			}
		}
////////////////////////////////////////
		imshow("��ȡ��Ƶ", dstImage);  //��ʾ��Ƶ
    //  imshow("�Ҷ�", grayImage);
	//	imshow("��Ե", edge);
	//	imshow("��Ե1", edge1);
		printf("********%d,%d*******\r\n", points.size(), points2.size());
		waitKey(1);  //��ʱ1ms
	}
	return 0;
}