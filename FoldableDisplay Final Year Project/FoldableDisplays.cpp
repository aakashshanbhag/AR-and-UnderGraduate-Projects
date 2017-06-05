/* Final Submission for Undergraduate Project for Foldable Displays using Homography using OpenCv 2.4.
Authors: Anusha Relwani, Kesha Bodawala, Aakash Shanbhag
*/
#include<D:\Software\opencv\build\include\opencv\cv.h>
#include<D:\Software\opencv\build\include\opencv\highgui.h>
#include<stdio.h>
#include<math.h>
#include<conio.h>

using namespace cv;
IplImage* find_marker(IplImage*);
IplImage* detect_marker(CvPoint*, CvPoint*, CvPoint*, CvPoint*, IplImage*);
bool detect_code(IplImage*);
IplImage* super_impose(int, int, IplImage*);
IplImage* display(CvPoint*, CvPoint*, CvPoint*, CvPoint*, IplImage*, IplImage*);
int marker1,marker2, marker3,marker4;

int main()
{

	CvCapture* cap = cvCreateCameraCapture(0);
	//IplImage* img;
	while (1)
	{
		IplImage* img = cvQueryFrame(cap);
		if (!img)
			break;
		img = find_marker(img); //function to find marker
		cvShowImage("finaloutput", img);
	
		cvWaitKey(1);
	}
		//cvWaitKey(0);
		cvReleaseCapture(&cap);
		//cvReleaseImage(&img);
		//cvReleaseImage(&img);
	//cvReleaseImage(&img_bw);
	//cvReleaseImage(&edge);
	//cvReleaseImage(&img_marker);
		cvDestroyWindow("finaloutput");
	return 0;
}



IplImage* find_marker(IplImage* img)
{
	int flag1 = 0;
	IplImage* img_bw = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	IplImage* final_img = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	cvCvtColor(img, img_bw, CV_BGR2GRAY);   //BGR to gray for canny edge detector
	Mat mat_bw = img_bw;						//IplImage* to Mat for Canny Edge detector
	Canny(mat_bw, mat_bw, 100, 150);		//Canny Edge Detector, detects edges of areas having pixel value > 250  
	
	IplImage* edge = cvCloneImage(&(IplImage)mat_bw);	//cv Mat to IplImage Converter
	cvShowImage("contour", edge);
	//Contouring
	CvMemStorage *cvmem = cvCreateMemStorage(0);
	CvSeq *cvs, *result = NULL;
	CvRect rect;
	int area = 0;
	CvPoint *pt[4];
	int g = cvFindContours(edge, cvmem, &cvs, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	IplImage* marker;
	if (g > 0)
	{
		for (int j = 0; j < g; j++)
		{
			rect = cvBoundingRect(cvs);
			area = cvContourArea(cvs);
			if ((rect.height > 40 || rect.width > 40) && area >1000)
			{
				result = cvApproxPoly(cvs, sizeof(CvContour), cvmem, CV_POLY_APPROX_DP, cvContourPerimeter(cvs)*0.02, 0); //approximate each conout with polygon and store information regarding contour in resu;t

				if (result->total == 4)  //result->total gives total number of vertices in the contour
				{
					for (int i = 0; i<4; i++)
					{
						pt[i] = (CvPoint*)cvGetSeqElem(result, i);                         //store vertices
						printf("x%d: %d		y%d: %d\n\n", i, pt[i]->x, i, pt[i]->y);
						printf("\n");
					}
					 
					marker = detect_marker(pt[0], pt[3], pt[2], pt[1], img);  //detect whether given rectangle is a marker or not
					if (detect_code(marker))
					{
						flag1 = 1;
						marker = super_impose(img->width, img->height, marker);
						final_img = display(pt[0], pt[3], pt[2], pt[1], marker, img); // move to next if
						
						// if marker then superimpose object and exit 				
					}
					

				}
			}
			cvs = cvs->h_next;															 //if not then try for next contour
		}
	}
	if (flag1 == 1)
		return final_img;
	else
		return img;
}


IplImage* detect_marker(CvPoint* a, CvPoint* b, CvPoint* c, CvPoint* d, IplImage* img)
{
	//warp perspective transform
	CvPoint2D32f inputQuad[4], outputQuad[4];
	CvMat* lambda = cvCreateMat(3, 3, CV_32FC1);
	Mat img_final;
	IplImage* img_marker = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	outputQuad[0].x = 0;
	outputQuad[0].y = 0;
	outputQuad[1].x = img->width - 1;
	outputQuad[1].y = 0;
	outputQuad[2].x = img->width - 1;
	outputQuad[2].y = img->height - 1;
	outputQuad[3].x = 0;
	outputQuad[3].y = img->height - 1;

	inputQuad[0].x = a->x;
	inputQuad[0].y = a->y;
	inputQuad[1].x = b->x;
	inputQuad[1].y = b->y;
	inputQuad[2].x = c->x;
	inputQuad[2].y = c->y;
	inputQuad[3].x = d->x;
	inputQuad[3].y = d->y;

	cvGetPerspectiveTransform(inputQuad, outputQuad, lambda);
	cvWarpPerspective(img, img_marker, lambda);
	//cvShowImage("detect", img_marker);
	return img_marker;
}


bool detect_code(IplImage* img_bw)
{
	marker1 = 1, marker2 = 1, marker3 = 1, marker4 = 1;
	//convert geray to binary
	//find points on middle of the upper part where there is change from black to white or white to black
	//same for lower middle part
	//decide whether marker or not
	for (int i = 50; i < (img_bw->height-50); i++)
	{
		for (int j = 50; j < (img_bw->width-50); j++)
		{
			uchar* green = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 1);
			uchar* blue = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j));
			uchar* red = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 2);
			
			if ((*green<250) || (*red>20) || (*blue>20))
			{
				marker1 = 0;
			}			
		}
	}

	for (int i = 50; i < (img_bw->height-50); i++)
	{
		for (int j = 50; j < (img_bw->width-50); j++)
		{
			uchar* green = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 1);
			uchar* blue = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j));
			uchar* red = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 2);

			if ((*blue<250) || (*red>20) || (*green>20))
			{
				marker2 = 0;
			}
		}
	}

	for (int i = 50; i < (img_bw->height-50); i++)
	{
		for (int j = 50; j < (img_bw->width-50); j++)
		{
			uchar* green = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 1);
			uchar* blue = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j));
			uchar* red = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 2);

			if ((*red<250) || (*green>20) || (*blue>20))
			{
				marker3 = 0;
			}
		}
	}

	for (int i = 50; i < (img_bw->height - 50); i++)
	{
		for (int j = 50; j < (img_bw->width - 50); j++)
		{
			uchar* green = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 1);
			uchar* blue = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j));
			uchar* red = (uchar*)((img_bw->imageData) + (img_bw->widthStep*i) + (3 * j) + 2);

			if ((*red>250) && (*green>250) && (*blue>250))
			{
				marker4 = 0;
			}
		}
	}

	printf("done");

	//if (!marker1 && !marker2 && !marker3)
		//return false;
	//else
		return true;
}

IplImage* super_impose(int x, int y, IplImage* img)
{
	IplImage* temp = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	IplImage* obj = cvLoadImage("C:\\Users\\Kesha\\Desktop\\paint\\sample.JPG");
	cvResize(obj, temp);
	return temp;
}



IplImage* display(CvPoint* a, CvPoint* b, CvPoint* c, CvPoint* d, IplImage* marker, IplImage* img)
{
	CvPoint2D32f inputQuad[4], outputQuad[4];
	CvMat* lambda = cvCreateMat(3, 3, CV_32FC1);
	Mat img_final;
	IplImage* final_img = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	inputQuad[0].x = 0;
	inputQuad[0].y = 0;
	inputQuad[1].x = img->width - 1;
	inputQuad[1].y = 0;
	inputQuad[2].x = img->width - 1;
	inputQuad[2].y = img->height - 1;
	inputQuad[3].x = 0;
	inputQuad[3].y = img->height - 1;

	outputQuad[0].x = a->x;
	outputQuad[0].y = a->y;
	outputQuad[1].x = b->x;
	outputQuad[1].y = b->y;
	outputQuad[2].x = c->x;
	outputQuad[2].y = c->y;
	outputQuad[3].x = d->x;
	outputQuad[3].y = d->y;

	cvGetPerspectiveTransform(inputQuad, outputQuad, lambda);
	cvWarpPerspective(marker, final_img, lambda);
	//cvShowImage("warp", final_img);

	for (int i = 0; i < img->height; i++)
	{
		for (int j = 0; j < img->width; j++)
		{
			uchar* green = (uchar*)((img->imageData) + (img->widthStep*i) + (3 * j) + 1);
			uchar* blue = (uchar*)((img->imageData) + (img->widthStep*i) + (3 * j));
			uchar* red = (uchar*)((img->imageData) + (img->widthStep*i) + (3 * j) + 2);

			uchar* green_marker = (uchar*)((final_img->imageData) + (final_img->widthStep*i) + (3 * j) + 1);
			uchar* blue_marker = (uchar*)((final_img->imageData) + (final_img->widthStep*i) + (3 * j));
			uchar* red_marker = (uchar*)((final_img->imageData) + (final_img->widthStep*i) + (3 * j) + 2);

			if (*green_marker == 0 && *blue_marker == 0 && *red_marker == 0)
			{
				*green_marker = *green;
				*blue_marker = *blue;
				*red_marker = *red;
			}
		}
	}


	return final_img;
}



