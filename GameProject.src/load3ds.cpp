/*	load 3ds file
 *
 *			written by Alexander Zaprjagaev
 *			frustum@public.tsu.ru
 *
 *	usage:
 *	float *Load3DS(char *name,int *num_vertex);
 */
#include "StdAfx.h"
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "load3ds.h"



#define	KEYFRAME						0xB000	// level 1
#define	  KEYFRAME_MESH_INFO			0xB002
#define   KEYFRAME_START_AND_END		0xB008
#define	  KEYFRAME_HEADER				0xb00a



#define  	POS_TRACK_TAG	0xb020 
#define  	ROT_TRACK_TAG	0xb021 
#define  	SCL_TRACK_TAG	0xb022 
#define		FOV_TRACK_TAG	0xb023
#define		ROLL_TRACK_TAG	0xb024	
#define		COL_TRACK_TAG  	0xb025
#define		MORPH_TRACK_TAG 0xb026
#define		HOT_TRACK_TAG  	0xb027
#define		FALL_TRACK_TAG  0xb028
#define		HIDE_TRACK_TAG  0xb029
 	


#define  	PIVOT			0xb013 

#define  	NODE_HDR		0xb010 
#define  	NODE_ID			0xb030 
#define  	KFCURTIME		0xb009 


#define     TRACK_OBJ_NAME  0xB010









typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef signed char  sbyte;
typedef signed short sword;
typedef signed long  sdword;


int num_obj;
int num_trackobj;
int cur_trackobj;
static short unknown;

int cur_postrack;
int cur_rottrack;

typedef struct {
    word    id;
    dword   len;
} TChunkHeader, *PChunkHeader;


enum {
    CHUNK_RGBF      = 0x0010,
    CHUNK_RGBB      = 0x0011,
//    CHUNK_RBGB2     = 0x0012,       // ?? NOT HLS.

    CHUNK_PRJ       = 0xC23D,
    CHUNK_MLI       = 0x3DAA,

    CHUNK_MAIN      = 0x4D4D,
        CHUNK_OBJMESH   = 0x3D3D,
            CHUNK_BKGCOLOR  = 0x1200,
            CHUNK_AMBCOLOR  = 0x2100,
            CHUNK_OBJBLOCK  = 0x4000,
                CHUNK_TRIMESH   = 0x4100,
                    CHUNK_VERTLIST  = 0x4110,
                    CHUNK_VERTFLAGS = 0x4111,
                    CHUNK_FACELIST  = 0x4120,
                    CHUNK_FACEMAT   = 0x4130,
                    CHUNK_MAPLIST   = 0x4140,
                    CHUNK_SMOOLIST  = 0x4150,
                    CHUNK_TRMATRIX  = 0x4160,
                    CHUNK_MESHCOLOR = 0x4165,
                    CHUNK_TXTINFO   = 0x4170,
                CHUNK_LIGHT     = 0x4600,
                    CHUNK_SPOTLIGHT = 0x4610,
                CHUNK_CAMERA    = 0x4700,
                CHUNK_HIERARCHY = 0x4F00,
        CHUNK_VIEWPORT  = 0x7001,
        CHUNK_MATERIAL  = 0xAFFF,
            CHUNK_MATNAME   = 0xA000,
            CHUNK_AMBIENT   = 0xA010,
            CHUNK_DIFFUSE   = 0xA020,
            CHUNK_SPECULAR  = 0xA030,
            CHUNK_TEXTURE   = 0xA200,
            CHUNK_BUMPMAP   = 0xA230,
            CHUNK_MAPFILE   = 0xA300,
        CHUNK_KEYFRAMER = 0xB000,
            CHUNK_AMBIENTKEY    = 0xB001,
            CHUNK_TRACKINFO = 0xB002,
                CHUNK_TRACKOBJNAME  = 0xB010,
                CHUNK_TRACKPIVOT    = 0xB013,
                CHUNK_TRACKPOS      = 0xB020,
                CHUNK_TRACKROTATE   = 0xB021,
                CHUNK_TRACKSCALE    = 0xB022,
                CHUNK_TRACKMORPH    = 0xB026,
                CHUNK_TRACKHIDE     = 0xB029,
                CHUNK_OBJNUMBER     = 0xB030,
            CHUNK_TRACKCAMERA = 0xB003,
                CHUNK_TRACKFOV  = 0xB023,
                CHUNK_TRACKROLL = 0xB024,
            CHUNK_TRACKCAMTGT = 0xB004,
            CHUNK_TRACKLIGHT  = 0xB005,
            CHUNK_TRACKLIGTGT = 0xB006,
            CHUNK_TRACKSPOTL  = 0xB007,
            CHUNK_FRAMES    = 0xB008,



};



//-------------------------------------------------------


/*
// KEYFRAME - PROCESS ANIMATION DATA ROUTINES

void ProcessPivots(FILE *fp)
{
	float x,y,z;
	
	fread(&x, sizeof(float), 1, fp);
	fread(&y,  sizeof(float), 1, fp);
	fread(&z,  sizeof(float), 1, fp);

	pivot.x = x;
	pivot.y = y;
	pivot.z = z;

	if(bEnable3dsLogfile)
		fprintf(logfile, "PIVOT: %f %f %f\n", x, y, z);	

	
}
*/
void ProcessRotationTrack(FILE *fp, mesh_t* mesh)
{		
	int i;
	short framenum;
	long lunknown; 
	float rotation_rad;
	float axis_x;
	float axis_y;
	float axis_z;
	short pt_flags;


	fread(&pt_flags, sizeof(short), 1, fp);

	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);

	fread(&mesh->trimesh[cur_trackobj].num_rotkeys,	 sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	
	for(i = 0; i < mesh->trimesh[cur_trackobj].num_rotkeys; i++)
	{
		fread(&framenum,  sizeof(short), 1, fp);
		fread(&lunknown,  sizeof(long), 1, fp);
		fread(&rotation_rad,  sizeof(float), 1, fp);
		fread(&axis_x,  sizeof(float), 1, fp);
		fread(&axis_y,  sizeof(float), 1, fp);
		fread(&axis_z,  sizeof(float), 1, fp);

		mesh->trimesh[cur_rottrack].rot_track[i].framenum = framenum;
		mesh->trimesh[cur_rottrack].rot_track[i].lunknown = lunknown;
		mesh->trimesh[cur_rottrack].rot_track[i].rotation_rad = rotation_rad;
		mesh->trimesh[cur_rottrack].rot_track[i].axis_x = axis_x;
		mesh->trimesh[cur_rottrack].rot_track[i].axis_y = axis_y;
		mesh->trimesh[cur_rottrack].rot_track[i].axis_z = axis_z;
		if(i)
		{
/*			TVector V1 = TVector(mesh->trimesh[cur_trackobj].rot_track[i].axis_x,
								 mesh->trimesh[cur_trackobj].rot_track[i].axis_y,
								 mesh->trimesh[cur_trackobj].rot_track[i].axis_z);

			TVector V2 = TVector(mesh->trimesh[cur_trackobj].rot_track[i-1].axis_x,
								 mesh->trimesh[cur_trackobj].rot_track[i-1].axis_y,
								 mesh->trimesh[cur_trackobj].rot_track[i-1].axis_z);

			float S1 = mesh->trimesh[cur_trackobj].rot_track[i].rotation_rad;

			float S2 = mesh->trimesh[cur_trackobj].rot_track[i].rotation_rad;

			mesh->trimesh[cur_trackobj].rot_track[i].rotation_rad = S1*S2 - (V1*V2);

			TVector V = V2*S1 + V1*S1 + V1^V2;

			mesh->trimesh[cur_trackobj].rot_track[i].axis_x = V.Radius.X;
			mesh->trimesh[cur_trackobj].rot_track[i].axis_y = V.Radius.Y;
			mesh->trimesh[cur_trackobj].rot_track[i].axis_z = V.Radius.Z;
*/
		}

	}

	cur_rottrack++;
}

void ProcessPositionTrack(FILE *fp, mesh_t* mesh)
{	
	
	int i;
	short framenum;
	long  lunknown;
	float pos_x;
	float pos_y;
	float pos_z;
	short pt_flags;


	
	fread(&pt_flags, sizeof(short), 1, fp);

	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);

	fread(&mesh->trimesh[cur_trackobj].num_poskeys,	 sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	
	for(i = 0; i < mesh->trimesh[cur_trackobj].num_poskeys; i++)
	{
		fread(&framenum,  sizeof(short), 1, fp);
		fread(&lunknown,  sizeof(long), 1, fp);
		fread(&pos_x,  sizeof(float), 1, fp);
		fread(&pos_y,  sizeof(float), 1, fp);
		fread(&pos_z,  sizeof(float), 1, fp);

		mesh->trimesh[cur_postrack].pos_track[i].framenum = framenum;
		mesh->trimesh[cur_postrack].pos_track[i].lunknown = lunknown;
		mesh->trimesh[cur_postrack].pos_track[i].pos_x = pos_x;
		mesh->trimesh[cur_postrack].pos_track[i].pos_y = pos_y;
		mesh->trimesh[cur_postrack].pos_track[i].pos_z = pos_z;
		if(i)
		{
//			mesh->trimesh[cur_trackobj].pos_track[i].pos_x += mesh->trimesh[cur_trackobj].pos_track[i-1].pos_x;
//			mesh->trimesh[cur_trackobj].pos_track[i].pos_y += mesh->trimesh[cur_trackobj].pos_track[i-1].pos_y; 
//			mesh->trimesh[cur_trackobj].pos_track[i].pos_z += mesh->trimesh[cur_trackobj].pos_track[i-1].pos_z; 
		}

	}
	cur_postrack++;
}
	
void ProcessScaleTrack(FILE *fp, mesh_t* mesh)
{	
	
	int i;
	short framenum;
	long lunknown;		
	float scale_x;
	float scale_y;
	float scale_z;
	short pt_flags;


	fread(&pt_flags, sizeof(short), 1, fp);

	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);

	fread(&mesh->trimesh[cur_trackobj].num_scalekeys,	 sizeof(short), 1, fp);
	fread(&unknown,  sizeof(short), 1, fp);
	
	for(i = 0; i < mesh->trimesh[cur_trackobj].num_scalekeys; i++)
	{
		fread(&framenum,  sizeof(short), 1, fp);
		fread(&lunknown,  sizeof(long), 1, fp);
		
		fread(&scale_x,  sizeof(float), 1, fp);
		fread(&scale_y,  sizeof(float), 1, fp);
		fread(&scale_z,  sizeof(float), 1, fp);

		mesh->trimesh[cur_trackobj].scale_track[i].framenum = framenum;
		mesh->trimesh[cur_trackobj].scale_track[i].lunknown = lunknown;
		mesh->trimesh[cur_trackobj].scale_track[i].scale_x = scale_x;
		mesh->trimesh[cur_trackobj].scale_track[i].scale_y = scale_y;
		mesh->trimesh[cur_trackobj].scale_track[i].scale_z = scale_z;


	}
}
/*
void ProcessNodeId(FILE *fp)
{
	short node_id;


	fread(&node_id, sizeof(short), 1, fp);
	if(bEnable3dsLogfile)
		fprintf(logfile, "NODE_ID  %d\n", node_id); 
}

void ProcessNodeHeader(FILE *fp)
{
	int i;
	short flags1, flags2, heirarchy;
	char node_name[256];


	// read in node name from file

	for (i = 0; i < 256; i++)
	{
		fread(&node_name[i], sizeof(char), 1, fp);
		if(node_name[i] == 0)
			break;
	}

	fread(&flags1, sizeof(short), 1, fp);
	fread(&flags2, sizeof(short), 1, fp);
	fread(&heirarchy, sizeof(short), 1, fp);

	if(bEnable3dsLogfile)
	{
		fprintf(logfile, "NODE_HDR %s\n", node_name);
		fprintf(logfile, " flags1 %d\n", flags1);
		fprintf(logfile, " flags2 %d\n", flags2);
		fprintf(logfile, " heirarchy %d\n", heirarchy);
	}

}

void Process3DSVersion(FILE *fp)
{
	short version;

	fread(&version, sizeof(short), 1, fp);
	if(bEnable3dsLogfile)
		fprintf(logfile, "3DS VERSION %d\n",version);
	fread(&version, sizeof(short), 1, fp);
}

void ProcessMasterScale(FILE *fp)
{
	float master_scale;

	fread(&master_scale, sizeof(float), 1, fp);
	if(bEnable3dsLogfile)
		fprintf(logfile, "MASTER_SCALE %f\n",master_scale);	
}


*/

void ProcessPivots(FILE* file, mesh_t* mesh)
{

	float x,y,z;
	
	fread(&x, sizeof(float), 1, file);
	fread(&y,  sizeof(float), 1, file);
	fread(&z,  sizeof(float), 1, file);

	mesh->trimesh[cur_trackobj].pivot_x = x;
	mesh->trimesh[cur_trackobj].pivot_y = y;
	mesh->trimesh[cur_trackobj].pivot_z = z;


}








static int read_trackobjname(FILE *file,void *data) {
	int i = 0;
	mesh_t *mesh = (mesh_t*)data;

	while((mesh->trackobj_names[num_trackobj][i] = fgetc(file)) != '\0') i++;
	num_trackobj++;
	return ++i;
}





void ReadFrames(FILE* file,void* data)
{

	mesh_t *mesh = (mesh_t*)data;

	int done = 0;
	int i,j,cnt;
	int length;
	int	data_length;
	int frame_num;
	int num_frames;
	int datfile_vert_cnt;
	int quad_cnt=0;
	unsigned short	command;
	char temp;
	float tx, ty, tz;
	float angle;
	float x,y,z;
	BOOL process_data_flag;
	char datfilename[1024];
	int file_ex_start = 0;



	int kf_count = 0;
	int kfstart;
	int kfend;
	int kfcurframe;

	bool f;

	while (!done)
	{

		fread(&command, sizeof(command), 1, file);
		
		// Test for end of file

		if (feof(file))
		{
			done = 1;
			break;
		}
		
		fread (&length, sizeof(length), 1, file);
		data_length = length - 6;

		// Process 3DS file commands


		switch (command)
		{

			case TRACK_OBJ_NAME:
				read_trackobjname(file,data);

				f = true;

				cur_trackobj++;
/*				for(i = 0; (i < num_obj)&&(f); i++)
					if(strstr(mesh->obj_names[i],mesh->trackobj_names[num_trackobj - 1]))
					{
						cur_trackobj = i + 1;
						f = false;
					}
*/
			break;	


			case KEYFRAME_MESH_INFO:
				kf_count++;
			break;

			case KEYFRAME_START_AND_END:
				fread(&kfstart, sizeof(unsigned long), 1, file);
				fread(&kfend,	sizeof(unsigned long), 1, file);
			break;
				

			case KFCURTIME:
				fread(&kfcurframe, sizeof(long), 1, file);
			break;


			case POS_TRACK_TAG:  
				ProcessPositionTrack(file, mesh);
			break;

			case ROT_TRACK_TAG:  
				ProcessRotationTrack(file, mesh);
			break;
		
			case SCL_TRACK_TAG:
				ProcessScaleTrack(file, mesh);
			break;
			
			case PIVOT:
				ProcessPivots(file, mesh);
			break;

				
		} 
	}

}





//--------------------------------------------------------

typedef int (*process_chunk)(FILE *file,
		unsigned short type,int size,void *data);

/*
 *
 */

static void vector_copy(vector_t *a,vector_t *b) {
	b->x = a->x;
	b->y = a->y;
	b->z = a->z;
}

static void vector_add(vector_t *a,vector_t *b,vector_t *c) {
	c->x = a->x + b->x;
	c->y = a->y + b->y;
	c->z = a->z + b->z;
}

static void vector_sub(vector_t *a,vector_t *b,vector_t *c) {
	c->x = a->x - b->x;
	c->y = a->y - b->y;
	c->z = a->z - b->z;
}

static void vector_cross(vector_t *a,vector_t *b,vector_t *c) {
	c->x = a->y * b->z - a->z * b->y;
	c->y = a->z * b->x - a->x * b->z;
	c->z = a->x * b->y - a->y * b->x;
}

static float vector_normalize(vector_t *a,vector_t *b) {
	float length = sqrt( (double)a->x * a->x + a->y * a->y + a->z * a->z );
	if(length) {
		float ilength = 1.0 / length;
		b->x = a->x * ilength;
		b->y = a->y * ilength;
		b->z = a->z * ilength;
		return length;
	}
	b->x = 0.0;
	b->y = 0.0;
	b->z = 0.0;
	return 0.0;
}

/*
 *
 */

static void skeep_bytes(FILE *file,int bytes) {
	fseek(file,bytes,SEEK_CUR);
}


static int read_objname(FILE *file,void *data) {
	int i = 0;
	mesh_t *mesh = (mesh_t*)data;

	while((mesh->obj_names[num_obj][i] = fgetc(file)) != '\0') i++;
	num_obj++;
	return ++i;
}


static int skeep_string(FILE *file) {
	int i = 0;
	while(fgetc(file) != '\0') i++;
	return ++i;
}

static unsigned short read_ushort(FILE *file) {
	unsigned short ret;
	fread(&ret,1,sizeof(unsigned short),file);
	return ret;
}

static int read_int(FILE *file) {
	int ret;
	fread(&ret,1,sizeof(int),file);
	return ret;
}

static float read_float(FILE *file) {
	float ret;
	fread(&ret,1,sizeof(float),file);
	return ret;
}

static int read_chunk(FILE *file,process_chunk chunk_func,void *data) {
	unsigned short chunk_type;
	int chunk_size,content_size;
	chunk_type = read_ushort(file);
	chunk_size = read_int(file);
	content_size = chunk_size - 6;
	if(!chunk_func(file,chunk_type,content_size,data))
		skeep_bytes(file,content_size);
	return chunk_size;
}

static int read_chunks(FILE *file,int bytes,
	process_chunk chunk_func,void *data) {
	int bytes_read = 0;
	while(bytes_read < bytes)
		bytes_read += read_chunk(file,chunk_func,data);	
	if(bytes_read != bytes)
		fprintf(stderr,"expected %d bytes but read %d\n",bytes_read,bytes);
	return bytes_read;
}

/*
 *
 */

static int process_smoolist(FILE *file,unsigned short type,
	int size,void *data) {
	int i;
	trimesh_t *trimesh = (trimesh_t*)data;
	if(type == CHUNK_SMOOLIST) {
		trimesh->smoothgroup = (int*)malloc(sizeof(int) * trimesh->num_face);
		for(i = 0; i < trimesh->num_face; i++) {
			trimesh->smoothgroup[i] = read_int(file);
		}
		return 1;
	}
	return 0;
}

static int process_trimesh(FILE *file,unsigned short type,
	int size,void *data) {
	int i;
	trimesh_t *trimesh = (trimesh_t*)data;
	if(type == CHUNK_VERTLIST) { /* vertlist */
		trimesh->num_vertex = read_ushort(file);
		trimesh->vertex = (vector_t*)malloc(sizeof(vector_t) *
			trimesh->num_vertex);
		for(i = 0; i < trimesh->num_vertex; i++) {
			trimesh->vertex[i].x = read_float(file);
			trimesh->vertex[i].y = read_float(file);
			trimesh->vertex[i].z = read_float(file);
		}
		return 1;
	} else if(type == CHUNK_MAPLIST) { /* maplist */
		trimesh->num_uvmap = read_ushort(file);
		trimesh->uvmap = (uvmap_t*)malloc(sizeof(uvmap_t) * trimesh->num_uvmap);
		for(i = 0; i < trimesh->num_uvmap; i++) {
			trimesh->uvmap[i].u = read_float(file);
			trimesh->uvmap[i].v = 1.0 - read_float(file);
		}
		return 1;

	} else if(type == CHUNK_TRMATRIX)	{ 
		trimesh->local_centre_x = read_float(file);
		trimesh->local_centre_y = read_float(file);
		trimesh->local_centre_z = read_float(file);
		fseek(file, -sizeof(float)*3,SEEK_CUR);

	} else if(type == CHUNK_FACELIST) { /* facelist */
		int bytes_left;
		trimesh->num_face = read_ushort(file);
		trimesh->face = (int*)malloc(sizeof(int) * trimesh->num_face * 3);
		for(i = 0; i < trimesh->num_face * 3; i += 3) {
			trimesh->face[i + 0] = read_ushort(file);
			trimesh->face[i + 1] = read_ushort(file);
			trimesh->face[i + 2] = read_ushort(file);
			read_ushort(file);
		}
		bytes_left = size - trimesh->num_face * sizeof(unsigned short) * 4 - 2;
		if(bytes_left > 0)
			read_chunks(file,bytes_left,process_smoolist,(void*)trimesh);
		return 1;
	}

	return 0;
}

static int process_objblock(FILE *file,unsigned short type,
	int size,void *data) {
	mesh_t *mesh = (mesh_t*)data;
	if(type == CHUNK_TRIMESH) {
		mesh->num_trimesh++;
		if(mesh->trimesh == NULL) mesh->trimesh =
			(trimesh_t*)malloc(sizeof(trimesh_t));
		else mesh->trimesh = (trimesh_t*)realloc(mesh->trimesh,
			sizeof(trimesh_t) * mesh->num_trimesh);
		memset(&mesh->trimesh[mesh->num_trimesh - 1],0,sizeof(trimesh_t));
		read_chunks(file,size,process_trimesh,
			(void*)&mesh->trimesh[mesh->num_trimesh - 1]);
		return 1;
	}
	return 0;
}

static int process_objmesh(FILE *file,unsigned short type,
	int size,void *data) {
	if(type == CHUNK_OBJBLOCK) {
		size -= read_objname(file,data);
		read_chunks(file,size,process_objblock,data);
		return 1;
	}
	return 0;
}

static int process_main(FILE *file,unsigned short type,
	int size,void *data) {
	if(type == CHUNK_OBJMESH) {
		read_chunks(file,size,process_objmesh,data);
		return 1;
	}

	if(type == CHUNK_KEYFRAMER)
	{
		ReadFrames(file, data);
	}

	return 0;
}


/*
 *
 */

static void create_trimesh(trimesh_t *trimesh) {
	int i;
	uvmap_t *uvmap;
	vector_t *vertex,*normal_face,*normal_vertex;
	int *face,*vertex_count,**vertex_face,*smoothgroup;
	/* calc normals */
	normal_face = (vector_t*)malloc(sizeof(vector_t) * trimesh->num_face);
	memset(normal_face,0,sizeof(vector_t) * trimesh->num_face);
	normal_vertex = (vector_t*)malloc(sizeof(vector_t) * trimesh->num_face * 3);
	memset(normal_vertex,0,sizeof(vector_t) * trimesh->num_face * 3);
	vertex_count = (int*)malloc(sizeof(int) * trimesh->num_face * 3);
	memset(vertex_count,0,sizeof(int) * trimesh->num_face * 3);
	vertex_face = (int**)malloc(sizeof(int*) * trimesh->num_face * 3);
	memset(vertex_face,0,sizeof(int) * trimesh->num_face * 3);
	trimesh->normal = normal_vertex;
	vertex = trimesh->vertex;
	face = trimesh->face;
	smoothgroup = trimesh->smoothgroup;
	for(i = 0; i < trimesh->num_face; i++) {
		int j = i * 3;
		vector_t a,b;
		int v0 = face[j + 0];
		int v1 = face[j + 1];
		int v2 = face[j + 2];
		vertex_count[v0]++;
		vertex_count[v1]++;
		vertex_count[v2]++;
		vector_sub(&vertex[v1],&vertex[v0],&a);
		vector_sub(&vertex[v2],&vertex[v0],&b);
		vector_cross(&a,&b,&normal_face[i]);
		vector_normalize(&normal_face[i],&normal_face[i]);
	}
	for(i = 0; i < trimesh->num_face * 3; i++) {
		vertex_face[i] = (int*)malloc(sizeof(int) * (vertex_count[i] + 1));
		vertex_face[i][0] = vertex_count[i];
	}
	for(i = 0; i < trimesh->num_face; i++) {
		int j = i * 3;
		int v0 = face[j + 0];
		int v1 = face[j + 1];
		int v2 = face[j + 2];
		vertex_face[v0][vertex_count[v0]--] = i;
		vertex_face[v1][vertex_count[v1]--] = i;
		vertex_face[v2][vertex_count[v2]--] = i;
	}
	for(i = 0; i < trimesh->num_face; i++) {
		int j = i * 3,k;
		int v0 = face[j + 0];
		int v1 = face[j + 1];
		int v2 = face[j + 2];
		for(k = 1; k <= vertex_face[v0][0]; k++) {
			int l = vertex_face[v0][k];
			if(i == l || (smoothgroup && smoothgroup[i] & smoothgroup[l]))
				vector_add(&normal_vertex[j + 0],&normal_face[l],
					&normal_vertex[j + 0]);
		}
		for(k = 1; k <= vertex_face[v1][0]; k++) {
			int l = vertex_face[v1][k];
			if(i == l || (smoothgroup && smoothgroup[i] & smoothgroup[l]))
				vector_add(&normal_vertex[j + 1],&normal_face[l],
					&normal_vertex[j + 1]);
		}
		for(k = 1; k <= vertex_face[v2][0]; k++) {
			int l = vertex_face[v2][k];
			if(i == l || (smoothgroup && smoothgroup[i] & smoothgroup[l]))
				vector_add(&normal_vertex[j + 2],&normal_face[l],
					&normal_vertex[j + 2]);
		}
	}
	for(i = 0; i < trimesh->num_face * 3; i++) {
		vector_normalize(&normal_vertex[i],&normal_vertex[i]);
		free(vertex_face[i]);
	}
	free(normal_face);
	free(vertex_count);
	free(vertex_face);
	/* create linear arrays for vertex and uvmap */
	vertex = (vector_t*)malloc(sizeof(vector_t) * trimesh->num_face * 3);
	memset(vertex,0,sizeof(vector_t) * trimesh->num_face * 3);
	uvmap = (uvmap_t*)malloc(sizeof(uvmap_t) * trimesh->num_face * 3);
	memset(uvmap,0,sizeof(uvmap_t) * trimesh->num_face * 3);
	for(i = 0; i < trimesh->num_face * 3; i += 3) {
		vector_copy(&trimesh->vertex[trimesh->face[i + 0]],&vertex[i + 0]);
		vector_copy(&trimesh->vertex[trimesh->face[i + 1]],&vertex[i + 1]);
		vector_copy(&trimesh->vertex[trimesh->face[i + 2]],&vertex[i + 2]);
	}
	free(trimesh->vertex);
	trimesh->vertex = vertex;
	trimesh->num_vertex = trimesh->num_face * 3;
	if(trimesh->uvmap) {
		for(i = 0; i < trimesh->num_face * 3; i += 3) {
			uvmap[i + 0].u = trimesh->uvmap[trimesh->face[i + 0]].u;
			uvmap[i + 0].v = trimesh->uvmap[trimesh->face[i + 0]].v;
			uvmap[i + 1].u = trimesh->uvmap[trimesh->face[i + 1]].u;
			uvmap[i + 1].v = trimesh->uvmap[trimesh->face[i + 1]].v;
			uvmap[i + 2].u = trimesh->uvmap[trimesh->face[i + 2]].u;
			uvmap[i + 2].v = trimesh->uvmap[trimesh->face[i + 2]].v;
		}
		free(trimesh->uvmap);
	}
	trimesh->uvmap = uvmap;
	trimesh->num_uvmap = trimesh->num_face * 3;
	free(trimesh->face);
	trimesh->face = NULL;
}

void create_mesh(mesh_t *mesh,int *num_vertex) {
	int i,j;
	float *vertex;
	for(i = 0, *num_vertex = 0; i < mesh->num_trimesh; i++)
	{
		create_trimesh(&mesh->trimesh[i]);
		*num_vertex += mesh->trimesh[i].num_face;
	}
	*num_vertex *= 3;
}


//---------------------------

mesh_t *Load3DS(char *name,int *num_vertex) {

	cur_trackobj = 0;
    num_obj = 0;
	num_trackobj = 0;

	cur_rottrack = 0;
	cur_postrack = 0;

	FILE *file;
	mesh_t *mesh;
	float *vertex;
	unsigned short chunk_type;
	int chunk_size,content_size;
	file = fopen(name,"rb");
	if(!file) {
		fprintf(stderr,"error open %s file\n",name);
		return NULL;
	}
	chunk_type = read_ushort(file);
	chunk_size = read_int(file);
	content_size = chunk_size - 6;
	if(chunk_type != CHUNK_MAIN) {
		fprintf(stderr,"wrong main chunk\n");
		fclose(file);
		return NULL;
	}
	mesh = (mesh_t*)malloc(sizeof(mesh_t));
	memset(mesh,0,sizeof(mesh_t));
	read_chunks(file,content_size,process_main,(void*)mesh);
	fclose(file);
	create_mesh(mesh,num_vertex);
	return mesh;
}

//--------------------------------------------------------------



