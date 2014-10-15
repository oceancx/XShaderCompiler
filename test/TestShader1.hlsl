
// HLSL Translator: Shader Test 1
// 09/10/2014

// Vertex shader

cbuffer VertexParam : register(b0)
{
	float4x4 wvpMatrix : packoffset(c0);
	nointerpolation float3 normal	: NORMAL : packoffset(c4.x), test : packoffset(c5.x);
};

struct VertexIn
{
	float3 coord	: POSITION;
	float3 normal	: NORMAL;
	float2 texCoord	: TEXCOORD0;
	float4 color	: COLOR;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float2 texCoord	: TEXCOORD0;
	float4 color	: COLOR;
};

VertexOut VS(VertexIn inp)
{
	VertexOut outp = (VertexOut)0;
	
	// Vertex transformation
	outp.position	= mul(wvpMatrix, float4(inp.coord, 1.0));
	outp.texCoord	= inp.texCoord;
	
	// Per-vertex lighting
	float3 lightDir = (float3)0.5;//{ 0.5, -0.5, 1.0 };
	
	float NdotL		= dot(normalize(inp.normal), -normalize(lightDir));
	float shading	= max(0.2, NdotL);

	outp.color		= inp.color * shading;
	
	return outp;
}

// Pixel shader

/*Texture2D tex : register(t0);
SamplerState samplerState : register(s0);*/

float4 PS(VertexOut inp) : SV_Target0
{
	float3 interpColor = float3(1.0, 0.0, 0.0);
	
	float4 diffuse = lerp((float4)1.0, tex.Sample(samplerState, inp.texCoord), inp.position.x);
	
	return inp.color * diffuse;
}

// Compute shader

[numthreads(10, 1, 1)]
void CS(uint3 threadID : SV_DispatchThreadID)
{
	// expression tests
	float x = 3 * (float)-threadID.x;
	int y = (int)x * 2 + 2 - (int)(x + 0.5) + (int)(float)(z) + 9;
	float a = 1, b = 2 + (a += 4);
	
	// Loop test
	[unroll(4)]
	for (int i = 0; i < 10; ++i)
		for (int y = 0; y < 20; y++)
		{
			// Conidition test
			[branch]
			if (x > y + 2)
				;//i++;
			else if (x == 2)
			{
				int y;
				i += 4;
			} else { int z; x = y; }
		}
	
	while (test(x))
		do {
			test2(y, x);
		} while ((bool)(x) == true);
	
	// Switch test
	switch (x)
	{
		case 1:
		{
			int x = 5;
			;
			;;;
			{
				;;;
			}
		}
		case 2:
			break;
		default:
			break;
	}
	
	//...
}

