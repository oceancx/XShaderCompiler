
// Intrinsic Test 1
// 01/03/2017

float4 VS(float x : LOD) : SV_Position
{
    float y = max(1, x);
    return y;
}
