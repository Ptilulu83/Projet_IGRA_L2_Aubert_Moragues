#ifndef DRAWINGS_H
#define DRAWINGS_H

void apply_image_transforms (Mydata *data);
void update_area_with_transforms (Mydata *data);
void draw_control_labels(cairo_t * cr, PangoLayout * layout, Curve_infos * ci);
void draw_control_polygons_open (cairo_t * cr,Curve_infos * ci);
void draw_control_polygons_close(cairo_t * cr,Curve_infos * ci);
void draw_bezier_polygons_open(cairo_t * cr, Curve_infos * ci);
void draw_bezier_polygons_close(cairo_t * cr, Curve_infos * ci);
void draw_bezier_polygons_prolong(cairo_t * cr, Curve_infos * ci);
void draw_bezier_curve(cairo_t *cr, Control bez_points[4], double theta);
void draw_bezier_curves_open (cairo_t *cr , Curve_infos *ci , double theta);
void draw_bezier_curves_close(cairo_t *cr , Curve_infos *ci , double theta);
void draw_bezier_curves_prolong(cairo_t *cr , Curve_infos *ci , double theta);
void draw_tracks(cairo_t *cr , Track_list *track_list);
//void draw_bezier_curves_prolong_game(cairo_t *cr , Curve_infos *ci , double theta);
void generate_bezier_path(cairo_t *cr, Control bez_pts[4], double theta, int is_first);
void draw_bezier_curves_fill(cairo_t *cr , Curve_infos *ci , double theta);
void draw_bezier_curves_clip(cairo_t *cr , Curve_infos *ci , double theta, gpointer data);
void draw_cannon(cairo_t * cr, gpointer data);
void draw_munition(cairo_t*, gpointer);
void draw_next_munition(cairo_t*, gpointer);
void draw_shots(cairo_t*, gpointer);

gboolean on_timeout_1(gpointer data);

void area_init(gpointer data);
#endif

