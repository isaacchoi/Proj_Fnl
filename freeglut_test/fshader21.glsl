#version 150

in vec4 color;
out vec4 fColor;


void
main()
{
	fColor = color;
	/*if(color == 0)
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fColor = vec4(0.0, 0.0, 1.0, 1.0);*/
}
