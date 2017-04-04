#ifndef CURVE_H
#define CURVE_H

#define CONTROL_MAX 200
#define CURVE_MAX 100

typedef struct{
	double x,y;
}Control;

typedef struct{
	int control_count;
	double shift_x;
	double shift_y;
	Control controls[CONTROL_MAX];
}Curve;

typedef struct{
	int curve_count;
	Curve curves[CURVE_MAX];
}Curve_list;

typedef struct{
	Curve_list curve_list;
	int current_curve;
	int current_control;
}Curve_infos;

void init_curve_infos (Curve_infos * ci);

int add_curve (Curve_infos * ci);
int add_control (Curve_infos * ci, double x, double y);
int find_control(Curve_infos * ci, double x, double y);
int move_control(Curve_infos * ci, double x, double y);
int move_curve(Curve_infos * ci, double dx, double dy);
int remove_curve(Curve_infos * ci);
int remove_control(Curve_infos * ci);
void convert_bsp3_to_bezier(double p[4], double b[4]);
void compute_bezier_points_open(Curve * curve, int i, Control bez_points[4]);
void compute_bezier_points_close(Curve * curve, int i, Control bez_points[4]);
void compute_bezier_points_prolong(Curve * curve, int i, Control bez_points[4]);
double compute_bezier_cubic (double d[4],double t);
int move_shift(Curve_infos *ci, double dx, double dy);
int reset_shift(Curve_infos *ci, double dx, double dy);
void store_sample (double x, double y,double sx[], double sy[], int *ind, int ind_max);
int interpolate_samples (double sx[], double sy[], double t, int tmax,double *x, double *y);
void sample_bezier_curve (Control bez_points[4], double theta,  double sx[], double sy[], int *ind, int ind_max, int is_first);
double compute_distant_point_forward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB);
double compute_distant_point_backward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB);

#endif