uniform float _xc;
uniform float _yc;
uniform float _huefreq;
uniform float _sz;
uniform float _escape;
uniform float _maxiter;

vec4 HSVtoRGB( float h, float s, float v ){
   int i;
   float f, p, q, t;
   vec4 RGB;
   
   h = 360*h;
   /*if( s == 0 ) {
      // achromatic (grey)
      RGB = vec3(v,v,v);
      return RGB
   }*/
   
   h /= 60;         // sector 0 to 5
   i = int(floor( h ));
   f = h - i;         // factorial part of h
   p = v * ( 1 - s );
   q = v * ( 1 - s * f );
   t = v * ( 1 - s * ( 1 - f ) );
   
   switch( i ) {
      case 0:
         RGB.x = v;
         RGB.y = t;
         RGB.z = p;
         break;
      case 1:
         RGB.x = q;
         RGB.y = v;
         RGB.z = p;
         break;
      case 2:
         RGB.x = p;
         RGB.y = v;
         RGB.z = t;
         break;
      case 3:
         RGB.x = p;
         RGB.y = q;
         RGB.z = v;
         break;
      case 4:
         RGB.x = t;
         RGB.y = p;
         RGB.z = v;
         break;
      default:      // case 5:
         RGB.x = v;
         RGB.y = p;
         RGB.z = q;
         break;
   }
   
   RGB.w = 1.0;
   return RGB;
   
}

vec4 mandel(float xc, float yc, float sz, float escape, float maxiter, 
			float huefreq, float ka, float kd, float ks, float roughness){

	float xmin,ymin,x,y,a,b,n,aa,bb,twoab,h;
	vec4 cMandel;

	xmin = xc - 0.5*sz;
	ymin = yc - 0.5*sz;
	x = xmin + sz*gl_TexCoord[0].s;
	y = ymin + sz*gl_TexCoord[0].t;

	n = 0; a = x; b = y;

	while (n <maxiter){
		aa = a*a;
		bb = b*b;
		twoab = 2*a*b;
		if((aa+bb)>escape){
			break;
		}
		n += 1;
		a = aa-bb+x;
		b = twoab+y;

	}

	h = 0.5*(1+sin(huefreq*n/maxiter));

	cMandel = HSVtoRGB(h,1,1);
	return cMandel;
}


void main(void) {


	gl_FragColor = mandel(_xc,_yc,_sz,_escape,_maxiter,_huefreq,0,1,0,0.1);
}