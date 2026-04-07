uniform vec2 u_resolution;

const float NOISE_GRANULARITY = 3.0/255.0;

float random(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float dist2(vec2 a, vec2 b)
{
    vec2 d = a - b;
    return sqrt(dot(d, d));
}

void main()
{
    // Gradient starts top left
    vec2 center = vec2(0.0, u_resolution.y);

    float d = dist2(gl_FragCoord.xy, center);
    float maxD = length(u_resolution);
    float t = clamp(d / maxD, 0.0, 1.0);


    vec3 start = vec3(0.11, 0.11, 0.11);
    vec3 end = vec3(0.02, 0.02, 0.02);

    vec3 fragmentColor = mix(start, end, t);
    fragmentColor += mix(-NOISE_GRANULARITY, NOISE_GRANULARITY, random(gl_FragCoord.xy));

    gl_FragColor = vec4(fragmentColor, 1.0);
}