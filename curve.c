#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

#include "curve.h"

void init_curve_infos (Curve_infos * ci){
	ci->current_curve=-1;
	ci->current_control=-1;
	ci->curve_list.curve_count=0;
}

int add_curve (Curve_infos * ci){
	if (ci->curve_list.curve_count >= CURVE_MAX){
		ci->current_curve=-1;
		return -1;
	}
	ci->current_curve=ci->curve_list.curve_count;							//n==current_curve
	ci->curve_list.curve_count++;
	//printf("Curve %d\n",ci->curve_list.curve_count);
	ci->curve_list.curves[ci->current_curve].control_count=0;
	ci->curve_list.curves[ci->current_curve].shift_x=0;
	ci->curve_list.curves[ci->current_curve].shift_y=0;
	ci->current_control=-1;
	return ci->current_curve;
}

int add_control (Curve_infos * ci, double x, double y){
	if (ci->current_curve <0 || ci->current_curve>ci->curve_list.curve_count-1){
		return -1;
	}
	if (ci->curve_list.curves[ci->current_curve].control_count >= CONTROL_MAX){
		ci->current_control=-1;
		return -1;
	}
	//printf("control %d : x=%d, y=%d \n",ci->current_control,(int) x ,(int) y );
	ci->current_control= ci->curve_list.curves[ci->current_curve].control_count;
	ci->curve_list.curves[ci->current_curve].control_count++;
	ci->curve_list.curves[ci->current_curve].controls[ci->current_control].x=x;
	ci->curve_list.curves[ci->current_curve].controls[ci->current_control].y=y;
	return ci->current_control;
}

int find_control(Curve_infos * ci, double x, double y){
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve=&ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){
			int tmpx = curve->controls[j].x;
			int tmpy = curve->controls[j].y;
			double dx = tmpx-x;
			double dy = tmpy-y;
			if ((dx*dx)+(dy*dy) <= (5*5)){
				ci->current_curve = i;
				ci->current_control = j;
				return 0;
			}
		}
	}
	ci->current_curve = -1;
	ci->current_control = -1;
	return -1;
}

int move_control(Curve_infos * ci, double dx, double dy){
	if (ci->current_curve <0 || ci->current_curve > ci->curve_list.curve_count-1){
		return -1;
	}
	Curve * curve = &ci->curve_list.curves[ci->current_curve];
	if (ci->current_control <0 || ci->current_control > curve->control_count-1){
		return -1;
	}
	curve->controls[ci->current_control].x += dx;
	curve->controls[ci->current_control].y += dy;
	return 0;
}

int move_curve(Curve_infos * ci, double dx, double dy){
	if (ci->current_curve <0 || ci->current_curve > ci->curve_list.curve_count-1){
		return -1;
	}
	Curve * curve = &ci->curve_list.curves[ci->current_curve];
	for (int j = 0; j < curve->control_count; ++j)
	{
		curve->controls[j].x += dx;
		curve->controls[j].y += dy;
	}
	return 0;
}

int remove_curve(Curve_infos * ci){
	int n=ci->current_curve;
	if (n <0 || n > ci->curve_list.curve_count-1){
		return -1;
	}
	memmove (ci->curve_list.curves+n,ci->curve_list.curves+n+1, sizeof(Curve)*(ci->curve_list.curve_count-1-n));
	ci->curve_list.curve_count--;
	ci->current_curve= -1;
	return 0;
}

int remove_control(Curve_infos * ci){
	int n=ci->current_control;
	if (ci->current_curve <0 || ci->current_curve > ci->curve_list.curve_count-1){
		return -1;
	}
	Curve * curve = &ci->curve_list.curves[ci->current_curve];
	if (ci->current_control <0 || ci->current_control > curve->control_count-1){
		return -1;
	}
	memmove (curve->controls+n,curve->controls+n+1, sizeof(Control)*(curve->control_count-1-n));
	curve->control_count--;
	if (curve->control_count<= 0){
		remove_curve(ci);
	}

	ci->current_control= -1;
	return 0;
}

void convert_bsp3_to_bezier(double p[4], double b[4]){
	b[0] = (p[0] + 4*p[1] + p[2])/6.;
	b[1] = (4*p[1] + 2*p[2])/6.;
	b[2] = (2*p[1] + 4*p[2])/6.;
	b[3] = (p[1] + 4*p[2] + p[3])/6.;
}

void compute_bezier_points_open(Curve * curve, int i, Control bez_points[4]){
	double px[4] ,py[4];
	double bx[4] ,by[4];

	for (int j = 0; j < 4; ++j){
		px[j]=curve->controls[i+j].x;
		py[j]=curve->controls[i+j].y;
	}
	convert_bsp3_to_bezier(px, bx);
	convert_bsp3_to_bezier(py, by);
	for (int j = 0; j < 4; ++j){
		bez_points[j].x = bx[j];
		bez_points[j].y = by[j];
	}
}

void compute_bezier_points_close(Curve * curve, int i, Control bez_points[4]){
	double px[4] ,py[4];
	double bx[4] ,by[4];

	for (int j = 0; j < 4; ++j){
		px[j]=curve->controls[(i+j)%curve->control_count].x;
		py[j]=curve->controls[(i+j)%curve->control_count].y;
	}
	convert_bsp3_to_bezier(px, bx);
	convert_bsp3_to_bezier(py, by);
	for (int j = 0; j < 4; ++j){
		bez_points[j].x = bx[j];
		bez_points[j].y = by[j];
	}
}

void compute_bezier_points_prolong(Curve * curve, int i, Control bez_points[4]){
	double px[4] ,py[4];
	double bx[4] ,by[4];
	int k=4;
	if (i<2){
		px[0]=curve->controls[0].x;
		py[0]=curve->controls[0].y;
		px[1]=curve->controls[0].x;
		py[1]=curve->controls[0].y;
		k=2;
	}
	if (i>curve->control_count-2){
		px[2]=curve->controls[curve->control_count-1].x;
		py[2]=curve->controls[curve->control_count-1].y;
		px[3]=curve->controls[curve->control_count-1].x;
		py[3]=curve->controls[curve->control_count-1].y;
		i-=2;
		k=0;
	}

	for (int j = 0; j <2; ++j){
		px[k+j]=curve->controls[(i+j)%curve->control_count].x;
		py[k+j]=curve->controls[(i+j)%curve->control_count].y;
	}

	convert_bsp3_to_bezier(px, bx);
	convert_bsp3_to_bezier(py, by);
	for (int j = 0; j < 4; ++j){
		bez_points[j].x = bx[j];
		bez_points[j].y = by[j];
	}
}

double compute_bezier_cubic (double b[4],double t){
	if (t >= 0. && t <= 1.){
		return ((1-t)*(1-t)*(1-t)*b[0]) + (3*(1-t)*(1-t)*t*b[1]) + (3*(1-t)*t*t*b[2]) + (t*t*t*b[3]);
	}
	return (double) -1;
}

int move_shift(Curve_infos *ci, double dx, double dy){

	if (ci->current_curve >=0){
		Curve * curve = &ci->curve_list.curves[ci->current_curve];

		curve->shift_x+=dx;
		curve->shift_y+=dy;

		return 0;
	}
	else {
		return -1;
	}
}

int reset_shift(Curve_infos *ci, double dx, double dy){

	if (ci->current_curve >=0){
		Curve * curve = &ci->curve_list.curves[ci->current_curve];

		curve->shift_x=0;
		curve->shift_y=0;

		return 0;
	}
	else {
		return -1;
	}
}

