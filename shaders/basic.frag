#version 330 core
out vec4 FragColor;

in vec2 fragCoord;

uniform float scale;
uniform vec2 center;
uniform vec2 c;
uniform vec2 winSize;

const int MAX_I = 100;

// R is the escape radius; it is how big z may get before being deemed divergent.
const float R = 2.5;

void main()
{
	vec2 z = (fragCoord * scale) + center;
	z.x *= winSize.x/1024;
	z.y *= winSize.y/1024;

	for(int i = 0; i < MAX_I; i++)
	{
		// Has the point z gotten too big to reasonably converge?
		if(dot(z,z) > R*R)
		{
			float r = float(i)/float(MAX_I);
			float g = r*r*r;
			float b = r/2 + 0.1;
			FragColor = vec4(r, g, b, 1.0);
			return;
		}

		// f(z) = z^3 + z + c
		//z = vec2(z.x*z.x*z.x - 3*z.x*z.y*z.y, 3*z.x*z.x*z.y - z.y*z.y*z.y) + z + c;

		// f(z) = z^3 + 0.5z^2 - 0.7z + c
		//z = vec2(z.x*z.x*z.x - 3*z.x*z.y*z.y, 3*z.x*z.x*z.y - z.y*z.y*z.y) + 0.5*vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) - 0.7*z + c;

		// f(z) = x^2 + c
		z = vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) + c;

		// f(z) = x^2 * c
		//z = vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) * c;

		// f(z) = z^4 + c
		//z = vec2(z.x*z.x*z.x*z.x - 6*z.x*z.x*z.y*z.y + z.y*z.y*z.y*z.y, 4*z.x*z.x*z.x*z.y - 4*z.x*z.y*z.y*z.y) + c;
	}

	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
