Texture2D _color_map : register(t0);
SamplerState _sampler : register(s0);

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
	vo.pos = v.pos;
	vo.tex0 = v.tex0;
	return vo;
}

float4 ps_main(vs_output frag) : SV_TARGET
{
	return _color_map.Sample(_sampler, frag.tex0);
}
