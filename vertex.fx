Texture2D _color_map : register(t0);
SamplerState _sampler : register(s0);

cbuffer cb_changes_per_frame : register(b0)
{
	matrix worldMatrix;
};

cbuffer cb_never_changes : register(b1)
{
	matrix viewMatrix;
};

cbuffer cb_change_on_resize : register(b2)
{
	matrix projMatrix;
};

struct vs_input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct vs_output
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

vs_output main(vs_input v)
{
	vs_output vo = (vs_output)0;
	vo.pos = mul(v.pos, worldMatrix);
	vo.pos = mul(vo.pos, viewMatrix);
	vo.pos = mul(vo.pos, projMatrix);
	return vo;
}
