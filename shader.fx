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

vs_output vs_main(vs_input v)
{
	vs_output vo = (vs_output)0;
	vo.pos = mul(v.pos, worldMatrix);
	vo.pos = mul(vo.pos, viewMatrix);
	vo.pos = mul(vo.pos, projMatrix);
	//vo.pos = v.pos;
	vo.tex0 = v.tex0;
	return vo;
}

float4 ps_main(vs_output frag) : SV_TARGET
{
	return _color_map.Sample(_sampler, frag.tex0);
}
