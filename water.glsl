uniform int initialized;
uniform sampler2D texUnit;

const float pi = 3.14;
uniform float time;

uniform float waveLength1;
uniform float velocity1;
uniform float amplitude1;
uniform vec2 direction1;

uniform float waveLength2;
uniform float velocity2;
uniform float amplitude2;
uniform vec2 direction2;

uniform float waveLength3;
uniform float velocity3;
uniform float amplitude3;
uniform vec2 direction3;

uniform float waveLength4;
uniform float velocity4;
uniform float amplitude4;
uniform vec2 direction4;
uniform int obj;



vec4 waveHeightNormal(float x, float y) {
    float height = 0.0;
    float frequency = 0.0;
    float phase = 0.0;
    float theta = 0.0;
	float dx = 0.0;
	float dy = 0.0;
	vec3 normalvec;

	frequency = (2.0*pi)/waveLength1;
    phase = velocity1 * frequency;
    theta = dot(direction1, vec2(x, y));
    height = height + amplitude1 * sin(theta * frequency + time * phase);
	dx = dx + amplitude1 * direction1.x * frequency * cos(theta * frequency + time * phase);
    dy = dy + amplitude1 * direction1.y * frequency * cos(theta * frequency + time * phase);

		
	frequency = (2.0*pi)/waveLength2;
    phase = velocity2 * frequency;
    theta = dot(direction2, vec2(x, y));
    height = height + amplitude2 * sin(theta * frequency + time * phase);
	dx = dx + amplitude2 * direction2.x * frequency * cos(theta * frequency + time * phase);
    dy = dy + amplitude2 * direction2.y * frequency * cos(theta * frequency + time * phase);
	
	frequency = (2.0*pi)/waveLength3;
    phase = velocity3 * frequency;
    theta = dot(direction3, vec2(x, y));
    height = height + amplitude3 * sin(theta * frequency + time * phase);
	dx = dx + amplitude3 * direction3.x * frequency * cos(theta * frequency + time * phase);
    dy = dy + amplitude3 * direction3.y * frequency * cos(theta * frequency + time * phase);
	
	frequency = (2.0*pi)/waveLength4;
    phase = velocity4 * frequency;
    theta = dot(direction4, vec2(x, y));
    height = height + amplitude4 * sin(theta * frequency + time * phase);
	dx = dx + amplitude4 * direction4.x * frequency * cos(theta * frequency + time * phase);
    dy = dy + amplitude4 * direction4.y * frequency * cos(theta * frequency + time * phase);


	normalvec = vec3(-dx, -dy, -1.0);
    normalvec = normalize(normalvec);
	vec4 r = vec4(normalvec.x,normalvec.y,normalvec.z,height);
    return r;
}

float gaussianHeight(float x,float y)
{
	float A = 1.0;
	float x0 = 0.6;
	float y0 = 0.4;
	float sigma_x = 0.1;
	float sigma_y = 0.15;
	

	float theta = pi;
	float a = (cos(theta)*cos(theta))/(2.0*sigma_x*sigma_x) ;//+ sin(theta)*sin(theta)/(2.0*sigma_y*sigma_y);
	float b = -sin(2.0*theta)/(4.0*sigma_x*sigma_x) + sin(2.0*theta)/(4.0*sigma_y*sigma_y) ;
	float c = sin(theta)*sin(theta)/(2.0*sigma_x*sigma_x) + cos(theta)*cos(theta)/(2.0*sigma_y*sigma_y);
	float r = A*exp( - (a*(x-x0)*(x-x0) + 2.0*b*(x-x0)*(y-y0) + c*(y-y0)*(y-y0))) ;
	return r;

}



void update()
{
	float height;
	vec4 waveHN;
	vec3 normalvec;
	float gaussheight;
	vec2 pos = gl_TexCoord[0].xy;
	waveHN = waveHeightNormal(pos.x, pos.y);
	normalvec.x = waveHN.x;
	normalvec.y = waveHN.y;
	normalvec.z = waveHN.z;
	height = waveHN.w;
	height = (height+2.5)/8.0;
	gaussheight = gaussianHeight(pos.x, pos.y);

	vec3 eyePos = vec3(0.5,0,2);
	vec3 position = vec3(pos.x,pos.y,height);
	vec3 eye = normalize(eyePos - position);
	vec3 r = reflect(eye,normalvec);
	float ref = 0.1*r.x+0.1*r.y+0.8*r.z;
	vec4 colorreflection = vec4(0.2,0.2,ref,0.5);
	vec4 colorordinary = vec4(0.2, 0.05,height,1.0);
	vec4 color;
	if (obj==1)
		color = colorreflection;
	else
		color = colorordinary;

	

	if (gaussheight > height)
		gl_FragColor = vec4(0.2, gaussheight,0.05,1.0);
	else
		gl_FragColor = color;
	
	
}
void main()
{
	update();
}