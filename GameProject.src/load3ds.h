/*  load 3ds file
 *
 *      written by Alexander Zaprjagaev
 *      frustum@public.tsu.ru
 *      2:5005/93.15@FidoNet
 */

#include "Vectors.h"

#ifndef __LOAD3DS_H__
#define __LOAD3DS_H__

const int MAX_OBJECTS = 100;
const int MAX_STRLEN = 100;
const int MAX_NUM_3DS_FRAMES = 100;

const float PI                = 3.1415926535897932384626433832795;

typedef struct {
	float x,y,z;
} vector_t;

typedef struct {
	float u,v;
} uvmap_t;

typedef struct {

	vector_t rot_center;
	vector_t rot_axis;
	float    angle;

	vector_t position;

} frame_t;


typedef struct 
{
	short	framenum;
	long	lunknown;	
	float	rotation_rad;
	float	axis_x;	 
	float	axis_y;	 
	float	axis_z;	

} ROT_TRACK3DS;

typedef struct 
{
	short	framenum;
	long	lunknown;	
	float	pos_x;	 
	float	pos_y;	 
	float	pos_z;	

} POS_TRACK3DS;

typedef struct 
{
	short framenum;
	long  lunknown;
	float scale_x; 
	float scale_y; 
	float scale_z;

} SCL_TRACK3DS;


typedef struct {
	vector_t *vertex;
	int num_vertex;
	uvmap_t *uvmap;
	int num_uvmap;
	int *face;
	int *smoothgroup;
	vector_t *normal;
	int num_face;

	ROT_TRACK3DS rot_track[MAX_NUM_3DS_FRAMES];
	POS_TRACK3DS pos_track[MAX_NUM_3DS_FRAMES];
	SCL_TRACK3DS scale_track[MAX_NUM_3DS_FRAMES];

	int num_rotkeys;
	int num_poskeys;
	int num_scalekeys;

	float local_centre_x;
	float local_centre_y;
	float local_centre_z;

	float pivot_x;
	float pivot_y;
	float pivot_z;


} trimesh_t;


typedef struct {
	trimesh_t *trimesh;
	int num_trimesh;

	char obj_names[MAX_OBJECTS][MAX_STRLEN];

	char trackobj_names[MAX_OBJECTS][MAX_STRLEN];

} mesh_t;

// total 8 floats
// 1,2,3 - vertex
// 3,4,5 - normal
// 6,7 - texture coordinate

mesh_t *Load3DS(char *name,int *num_vertex);

#endif /* __LOAD3DS_H__ */

