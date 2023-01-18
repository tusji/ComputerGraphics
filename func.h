#include"algebra3.h"
#include"ppm.h"
#ifndef SHAP
#define SHAP
struct M{
	float material[10]={};
};

struct triangle{
    vec3 v1,v2,v3,n;
    vec3 v12,v13;
    M m;
};
struct triangles{
	triangle *tri=NULL;
	int size=0;
	vec3 max,min;
};

struct sphere{
    vec3 center;
    float radius=0;
    M m;
};
struct part{
	triangles tris;
	sphere s;
};
struct parts{
	part p[8]={};
	vec3 max,min,center;
};
float interacts(vec3 ray,sphere s,vec3 e);
float interactt(vec3 ray,triangle t,vec3 e);
void Ia(Pixel *p,M m);
void Id(float t,vec3 l,vec3 ray,vec3 n,vec3 newray,vec3 point,sphere s,Pixel *p,M m,parts ps,triangles ts);
void Is(vec3 ray,vec3 newray,vec3 n,Pixel *p,M m);
void reflect(int level,vec3 l,vec3 point,vec3 ray,vec3 n,Pixel *p,M m,sphere s,parts ps,triangles ts);
void addpart(parts *ps,triangle t);
void creatpart(triangles ts,parts* ps);
#endif
