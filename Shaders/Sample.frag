#version 330 core

//Texture to be passed
uniform sampler2D tex0;
//Should receive the texCoord from the vertex shader (_out_ in vertex should have corresponding _in_ in frag, same var name)
in vec2 texCoord;

out vec4 FragColor; //returns a Color
//Simple shader that colors the model Red

void main()
{
	//				  r    g   b   a	Ranges 0->1
	FragColor = vec4(0.7f, 0.0f, 0.0f, 1.0f); //Sets the color of the Fragment

	//Assign tex color using the function
	FragColor = texture(tex0, texCoord);
}