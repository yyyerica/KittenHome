#version 330 core

//in vec2 TextCoord;
//uniform sampler2D tex;
in vec3 Normal;//一个顶点对应一个法向量
in vec3 FragPos;

out vec4 color;

uniform vec3 lightPos;

void main()
{
	vec3 objectColor = vec3(1.0,1.0,0.0);
	//color = texture(tex, TextCoord);
	//color = vec4(0.5, 1.0, 1.0, 1.0);
	vec3 lightColor = vec3(1.0,1.0,1.0);
//环境光
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;//环境光颜色
	//vec3 result = ambient * objectColor;
	//color = vec4(result,1.0f);

//漫反射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	//光的方向向量是光的位置向量与片段的位置向量之间的向量差
	float diff = max(dot(norm, lightDir), 0.0);
	//对norm和lightDir向量进行点乘，来计算光对当前片段的实际的散射影响
	//如果两个向量之间的角度大于90度，点乘的结果就会变成负数，这样会导致散射因子变为负数。为此，我们使用max函数返回两个参数之间较大的参数，从而保证散射因子不会变成负数
	vec3 diffuse = diff * lightColor;
	//结果值再乘以光的颜色，得到散射因子（散射光颜色
	
	
	//既然我们有了一个环境光照颜色和一个散射光颜色，我们把它们相加，然后把结果乘以物体的颜色，来获得片段最后的输出颜色。
	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0f);
}