/* Basic baby Steps for AR OpenCv 2.4 for Marker based tracking with image on green ball for AR.
Authors: Anusha Relwani, Kesha Bodawala, Aakash Shanbhag
*/
#include<D:\Software\opencv\opencv\build\include\opencv\cv.h>
#include<D:\Software\opencv\opencv\build\include\opencv\highgui.h>
#include<stdio.h>
#include<math.h>
#include<conio.h>

using namespace cv;
void changesize(float u, float v, int a, int b);
	
int differ(IplImage* image_initial);

int l, i, j, itop, jtop, ileft, jleft, ibottom, jbottom, iright, jright, a, b, flag;

int rstd, rnew, rmain;
float s, u, v;
IplImage* img_minion;
IplImage* img_m;
IplImage* img_m1;
IplImage* img = cvLoadImage("C:\\Users\\Kesha\\Desktop\\ball.jpg");
int h = img->height;
int w = img->width;
int k = img->widthStep;
int hn = 50;

int main()
{
	rmain = differ(img);
	printf("rmain=%d",rmain);
	cvNamedWindow("final", CV_WINDOW_AUTOSIZE);
	CvCapture* cap = cvCreateCameraCapture(0);
	while (1){
		img = cvQueryFrame(cap);
		if (!img)
			break;
		 h = img->height;
		 w = img->width;
		 k = img->widthStep;

		 hn = 50;

		rnew = differ(img);

		printf("rnew is %d\n", rnew);
		
		s = (float)rnew/20;
		changesize(s, s, a, b);

		cvShowImage("final", img);
		cvWaitKey(10);
	}
	
	cvReleaseCapture(&cap);
	
	cvDestroyWindow("final");
	return 0;

}


int differ(IplImage* image_initial)
{
	IplImage* imgf = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	IplImage* imgf_bw = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			uchar* green = (uchar*)((image_initial->imageData) + (k*i) + (3 * j) + 1);
			uchar* blue = (uchar*)((image_initial->imageData) + (k*i) + (3 * j));
			uchar* red = (uchar*)((image_initial->imageData) + (k*i) + (3 * j) + 2);
			
			uchar* bluef = (uchar*)((imgf->imageData) + (k*i) + (3 * j));
			uchar* greenf = (uchar*)((imgf->imageData) + (k*i) + (3 * j) + 1);
			uchar* redf = (uchar*)((imgf->imageData) + (k*i) + (3 * j) + 2);

			if ((*green >= 220) && (*blue >= 90) && (*blue <= 120) && (*red >= 170) && (*red <= 255))
			{
				*bluef = 255;
				*greenf = 255;
				*redf = 255;
			}
			else
			{
				*bluef = 0;
				*greenf = 0;
				*redf = 0;
			}
		}
	}


	cvCvtColor(imgf, imgf_bw, CV_BGR2GRAY);
	cvNamedWindow("differentiated image", CV_WINDOW_AUTOSIZE);
	cvShowImage("differentiated image", imgf_bw);
	
	cvDilate(imgf_bw, imgf_bw, NULL, 5);
	cvNamedWindow("erode", CV_WINDOW_AUTOSIZE);
	cvShowImage("erode", imgf_bw);


	CvMemStorage *cvmem = cvCreateMemStorage(0);
	CvSeq *cvs = NULL;
	CvRect rect;
	int hn, wn;
	int area = 0;
	int max = 0;
	Mat imgx;
	
	int g = cvFindContours(imgf_bw, cvmem, &cvs);
	
	if (g > 0)
	{

		for (j = 0; j < g; j++)
		{
			rect = cvBoundingRect(cvs);
			area = cvContourArea(cvs);

			if (max < area)
			{

				hn = rect.height;
				wn = rect.width;
				max = area;

				b = rect.x + (hn / 2);
				a = rect.y + (wn / 2);
			}

			cvs = cvs->h_next;
		}


		rstd = hn;
		printf("\n%d,%d\n",a, b);
		return(rstd);
	}
	else{
		a = -50;
		b = -50;
		g = 0;
		return(50);
	}
}

void changesize(float u, float v, int a, int b)
{

	flag = 0;
	
	int h_das = img->height;
	int w_das = img->width;
	int k_das = img->widthStep;

	img_m1 = cvLoadImage("C:\\Users\\Kesha\\Desktop\\smallgreenscreenimage.jpg");
	img_m = cvCreateImage(cvSize(w_das, h_das), IPL_DEPTH_8U, 3);
	cvResize(img_m1, img_m);
	int h_m = img_m->height;
	int w_m = img_m->width;
	int k_m = img_m->widthStep;

	int hnew = h_m*u;
	int wnew = w_m*v;



	img_minion = cvCreateImage(cvSize(wnew, hnew), IPL_DEPTH_8U, 3);

	cvResize(img_m, img_minion);
	int h_rs = img_minion->height;
	int w_rs = img_minion->width;
	int k_rs = img_minion->widthStep;

	
	for (i = 0; i < h_rs; i++)
	{



		for (j = 0; j < w_rs; j++)
		{
			uchar* green_minion = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j) + 1);
			uchar* blue_minion = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j));
			uchar* red_minion = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j) + 2);

			uchar* green_minion_colnext = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * (j + 1)) + 1);
			uchar* blue_minion_colnext = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * (j + 1)));
			uchar* red_minion_colnext = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * (j + 1)) + 2);

			uchar* green_minion_rownext = (uchar*)((img_minion->imageData) + (k_rs*(i + 1)) + (3 * j) + 1);
			uchar* blue_minion_rownext = (uchar*)((img_minion->imageData) + (k_rs*(i + 1)) + (3 * j));
			uchar* red_minion_rownext = (uchar*)((img_minion->imageData) + (k_rs*(i + 1)) + (3 * j) + 2);

			if (*green_minion > 230 && *blue_minion < 20 && *red_minion < 10)
			{
			}

			else
			{

				if (flag == 0)
				{
					itop = i;
					jtop = j;


					jleft = j;
					ibottom = i;
					jright = j;

					flag = 1;


				}

				else{

					if (j < jleft)
					{
						ileft = i;
						jleft = j;
					}


					if (*green_minion_colnext == 255 && j>jright && *blue_minion_colnext < 20 && *red_minion_colnext<10)
					{
						jright = j;
						iright = i;
					}

					if (*green_minion_rownext == 255 && i>ibottom && *blue_minion_colnext < 50 && *red_minion_colnext < 50)
					{
						ibottom = i;
						jbottom = j;
					}

				}
			}
		}
	}


	for (int m = 0; m < h_das; m++)
	{

		for (int n = 0; n < w_das; n++)
		{
			if (m == a  && n == b)
			{

				for (i = itop; i <= ibottom; i++)
				{

					if ((a - (ibottom - itop) / 2) + i - itop <= 0)
					{
						continue;
					}

					if ((h_das) <= ((a - (ibottom - itop) / 2) + (i - itop)))
					{
						break;
					}

					for (j = jleft; j <= jright; j++)
					{
						if ((b - (jright - jleft) / 2) + j - jleft <= 0)
						{
							continue;
						}

						if ((w_das) <= ((b - (jright - jleft) / 2) + (j - jleft)))
						{
							break;
						}




						uchar* green = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j) + 1);
						uchar* blue = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j));
						uchar* red = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j) + 2);

						uchar* green_o = (uchar*)((img->imageData) + (k_das*(i - itop + (a - (ibottom - itop) / 2))) + (3 * (j - jleft + (b - (jright - jleft) / 2))) + 1);
						uchar* blue_o = (uchar*)((img->imageData) + (k_das*(i - itop + (a - (ibottom - itop) / 2))) + (3 * (j - jleft + (b - (jright - jleft) / 2))));
						uchar* red_o = (uchar*)((img->imageData) + (k_das*(i - itop + (a - (ibottom - itop) / 2))) + (3 * (j - jleft + (b - (jright - jleft) / 2))) + 2);

						

						uchar t1, t2, t3;
						if ((*green > 1.48*(*red)) && (*green > 1.48*(*blue)))
						{
							
						}
						else{
							t1 = *green;
							*green = *green_o;
							*green_o = t1;

							t2 = *red;
							*red = *red_o;
							*red_o = t2;

							t3 = *blue;
							*blue = *blue_o;
							*blue_o = t3;
						}
					}
				}
				break;
			}

		}
	}
}
