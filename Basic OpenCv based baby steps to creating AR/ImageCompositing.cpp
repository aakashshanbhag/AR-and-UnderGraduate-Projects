/* Basic baby Steps for AR OpenCv 2.4 for Image Compositing and Resizing with trackbars.
Authors: Anusha Relwani, Kesha Bodawala, Aakash Shanbhag
*/


#include<D:\Software\opencv\opencv\build\include\opencv\cv.h>
#include<D:\Software\opencv\opencv\build\include\opencv\highgui.h>
#include<stdio.h>
#include<math.h>
#include<conio.h>

using namespace cv;
void changesize(int u,int v,int a,int b);
void temporaryheight(int v, void*);
void temporarywidth(int u, void*);
void on_trackbara(int a, void*);
void on_trackbarb(int b, void*);
void translate(int a, int b);
int l, v, i,u, j, itop, jtop, ileft, jleft, ibottom, jbottom, iright, jright, a, b, flag,h,w;
float vfloat,ufloat;
IplImage* img;
IplImage* img_minion;
IplImage* img_m;
IplImage* img_m1;

int main()
{

	IplImage* img = cvLoadImage("D:\\my photos\\DasVidaniya_2k15\\1.jpg");
	h = img->height;
	w = img->width;
	v = 50;
	u = 50;
	cvNamedWindow("Slider1", CV_WINDOW_AUTOSIZE);
	createTrackbar("trackbar_height", "Slider1", &v, 100, temporaryheight);
	createTrackbar("trackbar_width", "Slider1", &u, 100, temporarywidth);
	createTrackbar("trackbarx", "Slider1", &a, h, on_trackbara);
	createTrackbar("trackbary", "Slider1", &b, w, on_trackbarb);

	while (1)
	{
	
		cvWaitKey(1);

	}

	cvReleaseImage(&img_m);
	cvReleaseImage(&img_minion);
	cvReleaseImage(&img_m1);
	cvReleaseImage(&img);
	return 0;

}

void on_trackbara(int a, void*)
{
	changesize(u,v,a, b);
}
void on_trackbarb(int b, void*)
{
	changesize(u,v,a, b);
}
void temporarywidth(int u, void*)
{
	changesize(u, v,a,b);

}

void temporaryheight(int v, void*)
{

	changesize(u,v,a,b);
}

void changesize(int u, int v,int a,int b)
{
	if (v==0)
	v = v + 1;

	if (u == 0)
		u = u + 1;

	vfloat = v / 50.0;
	ufloat = u / 50.0;
	flag = 0;
	img = cvLoadImage("D:\\my photos\\DasVidaniya_2k15\\1.jpg");
	int h_das = img->height;
	int w_das = img->width;
	int k_das = img->widthStep;

	img_m1 = cvLoadImage("C:\\Users\\Kesha\\Desktop\\greenscreenimage.jpg");
	img_m = cvCreateImage(cvSize(w_das, h_das), IPL_DEPTH_8U, 3);
	cvResize(img_m1, img_m);
	int h_m = img_m->height;
	int w_m = img_m->width;
	int k_m = img_m->widthStep;
	
	int hnew = h_m*vfloat;
	int wnew = w_m*ufloat;



	 img_minion = cvCreateImage(cvSize(wnew,hnew), IPL_DEPTH_8U, 3);
	
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
					if ((h_das - (a)) <= (i - itop))
					{

						
						break;
					}


					for (j = jleft; j <= jright; j++)
					{

						

						if ((w_das - (b)) <= (j - jleft))
						{
							
							break;
						}

						uchar* green = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j) + 1);
						uchar* blue = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j));
						uchar* red = (uchar*)((img_minion->imageData) + (k_rs*i) + (3 * j) + 2);

						uchar* green_o = (uchar*)((img->imageData) + (k_das*(i - itop + a)) + (3 * (j - jleft + b)) + 1);
						uchar* blue_o = (uchar*)((img->imageData) + (k_das*(i - itop + a)) + (3 * (j - jleft + b)));
						uchar* red_o = (uchar*)((img->imageData) + (k_das*(i - itop + a)) + (3 * (j - jleft + b)) + 2);

						
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
	
	cvNamedWindow("final", CV_WINDOW_AUTOSIZE);
	cvShowImage("final", img);

}
