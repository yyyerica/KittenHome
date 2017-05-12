#version 330 core

//in vec2 TextCoord;
//uniform sampler2D tex;
in vec3 Normal;//һ�������Ӧһ��������
in vec3 FragPos;

out vec4 color;

uniform vec3 lightPos;

void main()
{
	vec3 objectColor = vec3(1.0,0.0,0.0);
	//color = texture(tex, TextCoord);
	//color = vec4(0.5, 1.0, 1.0, 1.0);
	vec3 lightColor = vec3(1.0,1.0,1.0);
//������
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;//��������ɫ
	//vec3 result = ambient * objectColor;
	//color = vec4(result,1.0f);

//�������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	//��ķ��������ǹ��λ��������Ƭ�ε�λ������֮���������
	float diff = max(dot(norm, lightDir), 0.0);
	//��norm��lightDir�������е�ˣ��������Ե�ǰƬ�ε�ʵ�ʵ�ɢ��Ӱ��
	//�����������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ����������ᵼ��ɢ�����ӱ�Ϊ������Ϊ�ˣ�����ʹ��max����������������֮��ϴ�Ĳ������Ӷ���֤ɢ�����Ӳ����ɸ���
	vec3 diffuse = diff * lightColor;
	//���ֵ�ٳ��Թ����ɫ���õ�ɢ�����ӣ�ɢ�����ɫ
	
	
	//��Ȼ��������һ������������ɫ��һ��ɢ�����ɫ�����ǰ�������ӣ�Ȼ��ѽ�������������ɫ�������Ƭ�����������ɫ��
	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0f);
}