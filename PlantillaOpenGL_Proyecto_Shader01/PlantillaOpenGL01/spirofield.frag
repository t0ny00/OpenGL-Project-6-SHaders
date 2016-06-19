uniform float _r;
uniform float _hoff;
uniform float _freq;
uniform float _calctype;
uniform float _f;

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
   
};

float LCM (float r, float rv){
	float i = 0;
	float cm = 1.0;

	if (mod(r,rv)==0) cm = 1;
	else {
		i = 1;
		while (mod(r*i,rv)!=0) {
			i += 1;
		}
		cm = i;
	}
	return cm;
}

float calcspiro (float r, float rv, float b, float a){
	float rho;
	rho = (sqrt((r - rv)*(r - rv) + b*b + 2*(r-rv)*b*cos((1+r/rv)*a)));
	return rho;
}

vec4 spirofield (float r, float rv, float b, float hoff, float freq, float calctype, float f){
	float i, theta,rho,nrev,a,rsp,ss,tt;
	vec4 ci;
	ss = gl_TexCoord[0].s - 0.5;
	tt = gl_TexCoord[0].t - 0.5;
	theta = atan(tt,ss);
	theta += 3.1415;
	rho = 2*sqrt(ss * ss + tt * tt);
	if ((rho > ((r-rv+b)/r)) || (rho < ((r-rv-b)/r))){
		ci = 0.50;
		return ci;
	} 
	else{
		float deltad;
		vec4 ch, cg;
		nrev = LCM( r,rv);
		if ( 0 == calctype){

			float maxdist;
			maxdist = -2;
			for (i = 0; i < nrev; i += 1 ){
				a = theta + (i)*2*3.1415;
				rsp = calcspiro(r,rv,b,a) / r;
				deltad = abs(rsp - rho);
				if (deltad > maxdist) maxdist = deltad;
			}
			maxdist *= (nrev * freq);
			maxdist = mod((maxdist + hoff),1.0);
			ch = HSVtoRGB(maxdist,1,1);
			cg = vec4 (maxdist, maxdist, maxdist,1);
		}
		else if (1.0 == calctype){
			float mindist;
			mindist = 2;
			for (i = 0; i <nrev; i+=1){
				a = theta + (i)*2*3.1415;
				rsp = calcspiro(r,rv,b,a) / r;
				deltad = abs(rsp - rho);
				if (deltad < mindist)  mindist = deltad;
			}
			mindist *= (nrev*freq);
			mindist = mod (mindist+hoff,1.0);
			ch = HSVtoRGB(mindist,1,1);
			cg = vec4 (mindist, mindist, mindist,1);
		}
		else {
			float avdist = 0;
			for (i = 0; i <nrev; i+=1){
				a = theta + i*2*3.1415;
				rsp = calcspiro(r,rv,b,a) / r;
				avdist += abs(rsp - rho);
			}
			avdist *= freq;
			avdist = mod(avdist + hoff,1.0);
			ch = HSVtoRGB(avdist,1,1);
			cg = vec4 (avdist, avdist, avdist,1);
		}
		ci = mix (cg, ch, f);
		return ci;
	}
};


void main(void) {

	
	gl_FragColor = spirofield(_r/2,2.5,2.5,_hoff,_freq,_calctype,_f);
}