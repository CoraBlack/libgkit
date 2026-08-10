#shader vertex
#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 v_Color;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    v_Color = aColor;
}

#shader fragment
#version 410 core
out vec4 FragColor;

in vec3 v_Color;

void main()
{
    FragColor = vec4(v_Color, 1.0);
}
