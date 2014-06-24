struct VS_IN
{
	float4 pos : POSITION;
	row_major matrix mvp : WORLDVIEW;
};

float4 vs_main(VS_IN v) : SV_POSITION
{
	return mul(v.pos, v.mvp);
}

float4 ps_main(float4 pos : SV_POSITION) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}