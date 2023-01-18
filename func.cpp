#include"ppm.h"
#include"func.h"
#include"algebra3.h"
void creatpart(triangles ts,parts* ps){
	ps->max=ts.max;
	ps->min=ts.min;
	ps->center=(ps->max+ps->min)/2;
	
	
	for(int i=0;i<8;i++){
		switch(i){
			case 0:{
				ps->p[i].tris.max=ps->max;
				ps->p[i].tris.min=ps->center;
				
				break;
			}
			case 1:{
				ps->p[i].tris.max=ps->max;
				ps->p[i].tris.max[1]=ps->center[1];
				ps->p[i].tris.min=ps->center;
				ps->p[i].tris.min[1]=ps->min[1];
				
				break;
			}
			case 2:{
				ps->p[i].tris.max=ps->center;
				ps->p[i].tris.max[2]=ps->max[2];
				ps->p[i].tris.min=ps->min;
				ps->p[i].tris.min[2]=ps->center[2];
				
				break;
			}
			case 3:{
				ps->p[i].tris.max=ps->max;
				ps->p[i].tris.max[0]=ps->center[0];
				ps->p[i].tris.min=ps->center;
				ps->p[i].tris.min[0]=ps->min[0];
				
				break;
			}
			case 4:{
				ps->p[i].tris.max=ps->max;
				ps->p[i].tris.max[2]=ps->center[2];
				ps->p[i].tris.min=ps->center;
				ps->p[i].tris.min[2]=ps->min[2];
				
				break;
			}
			case 5:{//
				ps->p[i].tris.max=ps->center;
				ps->p[i].tris.max[0]=ps->max[0];
				ps->p[i].tris.min=ps->min;
				ps->p[i].tris.min[0]=ps->center[0];
				
				break;
			}
			case 6:{
				ps->p[i].tris.max=ps->center;
				ps->p[i].tris.min=ps->min;
				break;
			}
			case 7:{
				ps->p[i].tris.max=ps->center;
				ps->p[i].tris.max[1]=ps->max[1];
				ps->p[i].tris.min=ps->min;
				ps->p[i].tris.min[1]=ps->center[1];
				break;
			}
		}
		ps->p[i].s.center=(ps->p[i].tris.max+ps->p[i].tris.min)/2;
		ps->p[i].s.radius=(ps->p[i].tris.min-ps->p[i].tris.max).length()/2;
		ps->p[i].tris.tri=(triangle *)malloc(ts.size*sizeof(triangle));
	}
}

void addpart(parts *ps,triangle t){
	for(int i=0;i<8;i++){
		bool inside=false;
		for(int j=0;j<3;j++){
			if(t.v1[j]>ps->p[i].tris.max[j])goto again;
			if(t.v1[j]<ps->p[i].tris.min[j])goto again;
		}
		inside=true;
		for(int j=0;j<3;j++){
			if(t.v2[j]>ps->p[i].tris.max[j])goto again;
			if(t.v2[j]<ps->p[i].tris.min[j])goto again;
		}
		inside=true;
		for(int j=0;j<3;j++){
			if(t.v3[j]>ps->p[i].tris.max[j])goto again;
			if(t.v3[j]<ps->p[i].tris.min[j])goto again;
		}
		inside=true;
		again:;
		if(inside){
			ps->p[i].tris.tri[ps->p[i].tris.size]=t;
			ps->p[i].tris.size++;
		}
	}
}

float interacts(vec3 ray,sphere s,vec3 e){
	float a,b,c,Sroot,delta;
	a=ray.length2();
    b=2*(ray*(e-s.center));
    c=(e-s.center).length2()-pow(s.radius,2);
    delta=pow(b,2)-4*a*c;
    if(delta>=0){
        Sroot=(0-b-sqrt(delta))/(2*a);
        if(Sroot>((0-b+sqrt(delta))/(2*a)) && ((0-b+sqrt(delta))/(2*a))>0)Sroot=(0-b+sqrt(delta));
        if(Sroot>0){
       		return Sroot;
		}
	}
	return -1;
}
float interactt(vec3 ray,triangle t,vec3 e){
	if(ray*t.n==0)return -1;
	vec3 TriCon;
	mat3 mat,mat_trans;
	mat[0]=t.v12;
    mat[1]=t.v13;
    mat[2]=-ray;
    mat_trans=mat.transpose();
    TriCon=mat_trans.inverse()*(e-t.v1);
    if(TriCon[0]>=0 && TriCon[1]>=0 && TriCon[2]>0.01 && TriCon[0]+TriCon[1]<=1){
        return TriCon[2];
	}
	return -1;
}
void Ia(Pixel *p,M m){
	p->R=m.material[0]*255*m.material[3];
	p->G=m.material[1]*255*m.material[3];
	p->B=m.material[2]*255*m.material[3];
}
void Is(vec3 ray,vec3 newray,vec3 n,Pixel *p,M m){
	vec3 h;
	h=ray-newray;
	h.normalize();
	float is=n*h;
	if(is<0)is*=-1;
	is=pow(is,m.material[6]);
	if((255-p->R)<=(255*m.material[0]*is*m.material[5]))p->R=255;
	else p->R+=(255*m.material[0]*is*m.material[5]);
	if((255-p->G)<=(255*m.material[1]*is*m.material[5]))p->G=255;
	else p->G+=(255*m.material[1]*is*m.material[5]);
	if((255-p->B)<=(255*m.material[2]*is*m.material[5]))p->B=255;
	else p->B+=(255*m.material[2]*is*m.material[5]);
}
void Id(float t,vec3 l,vec3 ray,vec3 n,vec3 newray,vec3 point,sphere s,Pixel *p,M m,parts ps,triangles ts){

	float diffuse;
	if(interacts(newray,s,point)!=-1)return;
	for(int i=0;i<8;i++){
		if(interacts(newray,ps.p[i].s,point)!=-1){
			for(int k=0;k<ps.p[i].tris.size;k++){
				if(interactt(newray,ps.p[i].tris.tri[k],point)!=-1)return;
			}
		}
	}
	for(int k=0;k<ts.size;k++){
		if(interactt(newray,ts.tri[k],point)!=-1)return;
	}
	
	
	diffuse=newray*n;
	if(diffuse<0)return;
	if((255-p->R)<=(255*m.material[0]*diffuse*m.material[4]))p->R=255;
	else p->R+=(255*m.material[0]*diffuse*m.material[4]);
	if((255-p->G)<=(255*m.material[1]*diffuse*m.material[4]))p->G=255;
	else p->G+=(255*m.material[1]*diffuse*m.material[4]);
	if((255-p->B)<=(255*m.material[2]*diffuse*m.material[4]))p->B=255;
	else p->B+=(255*m.material[2]*diffuse*m.material[4]);
	Is(ray,newray,n,p,m);
}

void reflect(int level,vec3 l,vec3 point,vec3 ray,vec3 n,Pixel *p,M m,sphere s,parts ps,triangles ts){
	//§PÂ_
	if(level==0)return;
	if(m.material[7]==0)return;
	//newray
	vec3 newray;
	float proj=ray*n;
	if(proj<0)proj*=-1;
	newray=n*proj;
	newray*=2;
	newray=newray+ray;
	newray.normalize();
	//interate
	float t=20000,st;
	st=interacts(newray,s,point);
	if(st!=-1){
		t=st;
		vec3 newpoint,newnewray,newn;
		Pixel newp;
		newpoint=(t*newray)+point;
		newnewray=l-newpoint;
		newnewray.normalize();
		newn=newpoint-s.center;
		newn.normalize();

		Ia(&newp,s.m);
		Id(t,l,newray,newn,newnewray,newpoint,s,&newp,s.m,ps,ts);
		level--;
		reflect(level,l,newpoint,newray,newn,&newp,s.m,s,ps,ts);
		p->R+=newp.R*m.material[7];
		p->G+=newp.G*m.material[7];
		p->B+=newp.B*m.material[7];
	}
	for(int i=0;i<8;i++){
		if(interacts(newray,ps.p[i].s,point)!=-1){
			for(int k=0;k<ps.p[i].tris.size;k++){
        		float t0=interactt(newray,ps.p[i].tris.tri[k],point);
				if(t0<t && t0>0){
					t=t0;
					vec3 newpoint,newnewray,newn;
					Pixel newp;
					float diffuse;
					newpoint=(t*newray)+point;
					newnewray=l-newpoint;
					newnewray.normalize();
					newn=ps.p[i].tris.tri[k].n;
					newn.normalize();

					Ia(&newp,ps.p[i].tris.tri[k].m);
					Id(t,l,newray,newn,newnewray,newpoint,s,&newp,ps.p[i].tris.tri[k].m,ps,ts);
					level--;
					reflect(level,l,newpoint,newray,newn,&newp,ps.p[i].tris.tri[k].m,s,ps,ts);
					p->R+=newp.R*m.material[7];
					p->G+=newp.G*m.material[7];
					p->B+=newp.B*m.material[7];
				}
    		}
		}
	}
	for(int k=0;k<ts.size;k++){
        float t0=interactt(newray,ts.tri[k],point);
		if(t0<t && t0>0){
			t=t0;
			vec3 newpoint,newnewray,newn;
			Pixel newp;
			float diffuse;
			newpoint=(t*newray)+point;
			newnewray=l-newpoint;
			newnewray.normalize();
			newn=ts.tri[k].n;
			newn.normalize();
			Ia(&newp,ts.tri[k].m);
			Id(t,l,newray,newn,newnewray,newpoint,s,&newp,ts.tri[k].m,ps,ts);
			level--;
			reflect(level,l,newpoint,newray,newn,&newp,ts.tri[k].m,s,ps,ts);
			p->R+=newp.R*m.material[7];
			p->G+=newp.G*m.material[7];
			p->B+=newp.B*m.material[7];
		}
    }
	

}



