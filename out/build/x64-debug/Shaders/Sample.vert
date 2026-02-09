#version 330 core

//Gets the data at Attrib Index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

//Tex coord/UV is at 0
//Accesses UV and assigns it to aTex
layout(location = 2) in vec2 aTex;

//Pass tex coord to frag shader
out vec2 texCoord;

//Projection matrix
uniform mat4 projection;

//creates a transform variable
//we'll assign the transformation matrix here later
uniform mat4 transform;

uniform mat4 view;

void main()
{
	//multiply the matrix to the vec4 ver of aPos to get final pos

	//gl_Position is predefined
	//denotes the final pos of the vertex/point
	gl_Position =					//click
	projection *					//Apply the lens
	view *							//Position the cam
	transform * vec4(aPos, 1.0);	//Position the model

	//Assign the UV
	texCoord = aTex;
}