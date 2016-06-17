#version 330

struct LightSource {
    vec3 position;
    vec3 rgbIntensity;
};

in vec3 fragposition;
in vec3 fragnormal;

in VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} fs_in;

uniform sampler2D txt;
uniform sampler2D normalMap;
uniform bool use_texture;
uniform bool use_celshading;
uniform bool use_bump;

out vec4 fragColour;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

// Ambient light intensity for each RGB component.
uniform vec3 ambientIntensity;

float calculateLightStep(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
    return mix(edge0, edge1, T);
}

float calculateShadow()
{
    return 1.0f;
}

void main() {

    LightSource light = fs_in.light;

    vec3 fragPosition = fs_in.position_ES;

    vec2 xuv = vec2(-fragposition.z, -fragposition.y);
    vec2 yuv = vec2(fragposition.x, fragposition.z);
    vec2 zuv = vec2(fragposition.x, -fragposition.y);

    vec3 fragNormal;
    if(use_bump)
    {
        vec3 fragNormal1 = texture2D(normalMap, xuv).rgb * 2.0 - 1.0;
        vec3 fragNormal2 = texture2D(normalMap, yuv).rgb * 2.0 - 1.0;
        vec3 fragNormal3 = texture2D(normalMap, zuv).rgb * 2.0 - 1.0;

        fragNormal = normalize(fragNormal1+fragNormal2+fragNormal3);
    }
    else
    {
        fragNormal = fs_in.normal_ES;
    }

    vec3 l = normalize(light.position - fragPosition);
    vec3 v = normalize(-fragPosition.xyz);

    float normalL = max(dot(fragNormal, l), 0.0);

    vec3 diffuse, specular = vec3(0.0);;
    diffuse = material.kd * normalL;

    if (normalL > 0.0) {
        vec3 h = normalize(v + l);
        float n_dot_h = max(dot(fragNormal, h), 0.0);

        specular = material.ks * pow(n_dot_h, material.shininess);
    }

    float diffusePercentage = 1;
    float specularPercentage = 1;

    if(use_celshading)
    {
        vec3 N = normalize(fragNormal);
        vec3 L = normalize(light.position);
        vec3 Eye = fs_in.position_ES;
        vec3 H = normalize(L + Eye);

        diffusePercentage = max(0.0, dot(N, L));
        specularPercentage = max(0.0, dot(N, H));
        specularPercentage = pow(specularPercentage, material.shininess);
     
        const float A = 0.1;
        const float B = 0.3;
        const float C = 0.6;
        const float D = 1.0;
        float E = fwidth(diffusePercentage);

        if (diffusePercentage > A - E && diffusePercentage < A + E) diffusePercentage = calculateLightStep(A, B, E, diffusePercentage);
        else if (diffusePercentage > B - E && diffusePercentage < B + E) diffusePercentage = calculateLightStep(B, C, E, diffusePercentage);
        else if (diffusePercentage > C - E && diffusePercentage < C + E) diffusePercentage = calculateLightStep(C, D, E, diffusePercentage);
        else if (diffusePercentage < A) diffusePercentage = 0.0;
        else if (diffusePercentage < B) diffusePercentage = B;
        else if (diffusePercentage < C) diffusePercentage = C;
        else diffusePercentage = D;
     
        E = fwidth(specularPercentage);
        if (specularPercentage > 0.5 - E && specularPercentage < 0.5 + E)
        {
            specularPercentage = smoothstep(0.5 - E, 0.5 + E, specularPercentage);
        }
        else
        {
            specularPercentage = step(0.5, specularPercentage);
        }
    }

    float shadow = calculateShadow();

    vec3 colour = ambientIntensity + light.rgbIntensity * ( diffusePercentage * diffuse + specularPercentage * specular) * shadow;

    vec3 colorx = texture(txt, xuv).xyz;
    vec3 colory = texture(txt, yuv).xyz;
    vec3 colorz = texture(txt, zuv).xyz;

    if(use_texture) {
    	fragColour = vec4((colorx+colory+colorz)*colour, 1.0);
    } else {
        fragColour = vec4(colour, 1.0);
    }
}
