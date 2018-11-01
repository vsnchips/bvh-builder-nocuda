#pragma kernel CSMain

[numthreads(8,8,1)] // The wavefront size

void CSMain[id.xy] = snoise(vec3)
