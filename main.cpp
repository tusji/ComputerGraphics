#include"algebra3.h"
#include"ppm.h"
#include"func.h"
#include<stdio.h>
#include<math.h>
#define dis 2

int main(){
	int arrlen=100000;
	sphere s;
	triangle t;
	triangles ts;
	parts ps;
	M m;
	ts.tri=(triangle *)malloc(arrlen*sizeof(triangle));
	if(ts.tri==NULL){
		printf("fail");
		return 0;
	}

	vec3 e(0,0,0),v(0,0,0),top(0,1,0),l(0,0,0),
		 frame1,frame2,ray,TriCon;
	double f;
	int pixw,pixh;
	double picw,pich;
	char input=' ';

	FILE *fptr;
    fptr = fopen("Input_Bunny.txt","r");
    if(fptr==0){
    	printf("fail");
    	return 0;
	}

    while(!feof(fptr)){
        fscanf(fptr," %c",&input);
        switch(input){
        	case 'E':{
         	   fscanf(fptr,"%f %f %f", &e[0], &e[1], &e[2]);
        	    break;
        	}
        	case 'V':{
        	    fscanf(fptr,"%f %f %f", &v[0], &v[1], &v[2]);
        	    break;
        	}
        	case 'F':{
       	    	fscanf(fptr,"%lf", &f);
       	     	break;
        	}
        	case 'R':{
        	    fscanf(fptr,"%d %d", &pixw, &pixh);
        	    break;
        	}
        	case 'M':{
        		for(int i=0;i<10;i++){
        			fscanf(fptr,"%f",&m.material[i]);
				}
				break;
			}
        	case 'S':{
        		vec3 temp;
        	    fscanf(fptr,"%f %f %f %f", &temp[0], &temp[1], &temp[2], &s.radius);
        	    s.center=temp;
				s.m=m;
        	    break;
        	}
        	case 'T':{
        	    vec3 temp1,temp2,temp3,temp4;
        	    fscanf(fptr,"%f %f %f", &temp1[0], &temp1[1], &temp1[2]);
        	    fscanf(fptr,"%f %f %f", &temp2[0], &temp2[1], &temp2[2]);
        	    fscanf(fptr,"%f %f %f", &temp3[0], &temp3[1], &temp3[2]);
        	    fscanf(fptr,"%f %f %f", &temp4[0], &temp4[1], &temp4[2]);
				if(ts.size==0){
					for(int i=0;i<3;i++){
						ts.max[i]=temp1[i];
						ts.min[i]=temp1[i];
						if(ts.max[i]<temp2[i])ts.max[i]=temp2[i];
						if(ts.min[i]>temp2[i])ts.min[i]=temp2[i];
						if(ts.max[i]<temp3[i])ts.max[i]=temp3[i];
						if(ts.min[i]>temp3[i])ts.min[i]=temp3[i];
					}//=====================================¥¼§¹===========================================//	
				}
        	    if(ts.size<100000){
        	    	ts.tri[ts.size].v1=temp1;
        	    	ts.tri[ts.size].v2=temp2;
        	    	ts.tri[ts.size].v3=temp3;
        	    	ts.tri[ts.size].n=temp4;
        	    	ts.tri[ts.size].v12=ts.tri[ts.size].v2-ts.tri[ts.size].v1;
					ts.tri[ts.size].v13=ts.tri[ts.size].v3-ts.tri[ts.size].v1;
					for(int i=0;i<3;i++){
						if(ts.max[i]<temp1[i])ts.max[i]=temp1[i];
						if(ts.min[i]>temp1[i])ts.min[i]=temp1[i];
						if(ts.max[i]<temp2[i])ts.max[i]=temp2[i];
						if(ts.min[i]>temp2[i])ts.min[i]=temp2[i];
						if(ts.max[i]<temp3[i])ts.max[i]=temp3[i];
						if(ts.min[i]>temp3[i])ts.min[i]=temp3[i];
					}
					
					
					ts.tri[ts.size].m=m;
					ts.size++;
				}
				else{
					printf("fail");
					return 0;
				}
        	    break;
        	}
        	case 'L':{
        		fscanf(fptr,"%f %f %f",&l[0],&l[1],&l[2]);
				break;
			}
        }
    }
    fclose(fptr);
    
    
    creatpart(ts,&ps);
    for(int i=0;i<ts.size;i++){
    	addpart(&ps,ts.tri[i]);
	}
	free(ts.tri);
	ts.size=0;
	
    picw=2*dis*tan((f*M_PI)/(180*2));
    pich=(picw/pixw)*pixh;
    v.normalize();
	frame1=top^v;
	frame2=v^frame1;
	frame1.normalize();
	frame2.normalize();



	ColorImage image;
	image.init(pixw,pixh);
	Pixel p;


	for(int i=0;i<pixw;i++){
        for(int j=0;j<pixh;j++){
        	float t=20000,st;
            ray =dis*v+(picw/2-(i+0.5)*picw/pixw)*frame1+(pich/2-(j+0.5)*pich/pixh)*frame2;
            ray.normalize();
            /*shpere*/
			st=interacts(ray,s,e);
			if(st!=-1){
				t=st;
				int level=2;
				vec3 point,newray,n;
				point=(t*ray)+e;
				newray=l-point;
				newray.normalize();
				n=point-s.center;
				n.normalize();

				Ia(&p,s.m);
				reflect(level,l,point,ray,n,&p,s.m,s,ps,ts);
				image.writePixel(i,j,p);
				Id(t,l,ray,n,newray,point,s,&p,s.m,ps,ts);
				image.writePixel(i,j,p);

			}
			
			

			/*triangle*/

            /*for(int k=0;k<ts.size;k++){
            	if(ts.tri[k].n*ray<=0){
            		float t0=interactt(ray,ts.tri[k],e);
					if(t0<t && t0>0){
						t=t0;
						int level=2;
						vec3 point,newray,n;
						float diffuse;
						point=(t*ray)+e;
						newray=l-point;
						newray.normalize();
						n=ts.tri[k].n;
						n.normalize();

						Ia(&p,ts.tri[k].m);
						reflect(level,l,point,ray,n,&p,ts.tri[k].m,s,ts);
						image.writePixel(i,j,p);
						Id(t,l,ray,n,newray,point,s,ts,&p,ts.tri[k].m);
						image.writePixel(i,j,p);
						
					}
				}
            	
            }*/
            
            for(int k=0;k<8;k++){
            	if(interacts(ray,ps.p[k].s,e)!=-1){
            		for(int w=0;w<ps.p[k].tris.size;w++){
            			if(ps.p[k].tris.tri[w].n*ray<=0){
            			float t0=interactt(ray,ps.p[k].tris.tri[w],e);
							if(t0<t && t0>0){
								t=t0;
								int level=2;
								vec3 point,newray,n;
								float diffuse;
								point=(t*ray)+e;
								newray=l-point;
								newray.normalize();
								n=ps.p[k].tris.tri[w].n;
								n.normalize();

								Ia(&p,ps.p[k].tris.tri[w].m);
								reflect(level,l,point,ray,n,&p,ps.p[k].tris.tri[w].m,s,ps,ps.p[k].tris);
								image.writePixel(i,j,p);
								Id(t,l,ray,n,newray,point,s,&p,ps.p[k].tris.tri[w].m,ps,ps.p[k].tris);
								image.writePixel(i,j,p);
							}
						}
					}
				}
			}
        }
    }
    
    for(int i=0;i<8;i++){
    	free(ps.p[i].tris.tri);
	}
    image.outputPPM("Input_Bunny2.ppm");

}
