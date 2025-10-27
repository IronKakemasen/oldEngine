#include "PipelineSet.h"

PipelineComponents::PipelineComponents()
{
	cur_blendMode = kBlendModeNormal;
	cur_cullMode = CullMode::kBack;
	cur_shaderType = ShaderSetType::kV0P0;
}
