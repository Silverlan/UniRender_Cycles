// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <spdlog/logger.h>
#include <scene/shader.h>
#include <scene/shader_graph.h>
#include <scene/scene.h>
#include <scene/shader_nodes.h>
#include <OpenImageIO/ustring.h>

module pragma.scenekit.cycles;

import pragma.scenekit;

ccl::NodeMathType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::math::MathType type)
{
	switch(type) {
	case pragma::scenekit::nodes::math::MathType::Add:
		return ccl::NodeMathType::NODE_MATH_ADD;
	case pragma::scenekit::nodes::math::MathType::Subtract:
		return ccl::NodeMathType::NODE_MATH_SUBTRACT;
	case pragma::scenekit::nodes::math::MathType::Multiply:
		return ccl::NodeMathType::NODE_MATH_MULTIPLY;
	case pragma::scenekit::nodes::math::MathType::Divide:
		return ccl::NodeMathType::NODE_MATH_DIVIDE;
	case pragma::scenekit::nodes::math::MathType::Sine:
		return ccl::NodeMathType::NODE_MATH_SINE;
	case pragma::scenekit::nodes::math::MathType::Cosine:
		return ccl::NodeMathType::NODE_MATH_COSINE;
	case pragma::scenekit::nodes::math::MathType::Tangent:
		return ccl::NodeMathType::NODE_MATH_TANGENT;
	case pragma::scenekit::nodes::math::MathType::ArcSine:
		return ccl::NodeMathType::NODE_MATH_ARCSINE;
	case pragma::scenekit::nodes::math::MathType::ArcCosine:
		return ccl::NodeMathType::NODE_MATH_ARCCOSINE;
	case pragma::scenekit::nodes::math::MathType::ArcTangent:
		return ccl::NodeMathType::NODE_MATH_ARCTANGENT;
	case pragma::scenekit::nodes::math::MathType::Power:
		return ccl::NodeMathType::NODE_MATH_POWER;
	case pragma::scenekit::nodes::math::MathType::Logarithm:
		return ccl::NodeMathType::NODE_MATH_LOGARITHM;
	case pragma::scenekit::nodes::math::MathType::Minimum:
		return ccl::NodeMathType::NODE_MATH_MINIMUM;
	case pragma::scenekit::nodes::math::MathType::Maximum:
		return ccl::NodeMathType::NODE_MATH_MAXIMUM;
	case pragma::scenekit::nodes::math::MathType::Round:
		return ccl::NodeMathType::NODE_MATH_ROUND;
	case pragma::scenekit::nodes::math::MathType::LessThan:
		return ccl::NodeMathType::NODE_MATH_LESS_THAN;
	case pragma::scenekit::nodes::math::MathType::GreaterThan:
		return ccl::NodeMathType::NODE_MATH_GREATER_THAN;
	case pragma::scenekit::nodes::math::MathType::Modulo:
		return ccl::NodeMathType::NODE_MATH_MODULO;
	case pragma::scenekit::nodes::math::MathType::Absolute:
		return ccl::NodeMathType::NODE_MATH_ABSOLUTE;
	case pragma::scenekit::nodes::math::MathType::ArcTan2:
		return ccl::NodeMathType::NODE_MATH_ARCTAN2;
	case pragma::scenekit::nodes::math::MathType::Floor:
		return ccl::NodeMathType::NODE_MATH_FLOOR;
	case pragma::scenekit::nodes::math::MathType::Ceil:
		return ccl::NodeMathType::NODE_MATH_CEIL;
	case pragma::scenekit::nodes::math::MathType::Fraction:
		return ccl::NodeMathType::NODE_MATH_FRACTION;
	case pragma::scenekit::nodes::math::MathType::Sqrt:
		return ccl::NodeMathType::NODE_MATH_SQRT;
	case pragma::scenekit::nodes::math::MathType::InvSqrt:
		return ccl::NodeMathType::NODE_MATH_INV_SQRT;
	case pragma::scenekit::nodes::math::MathType::Sign:
		return ccl::NodeMathType::NODE_MATH_SIGN;
	case pragma::scenekit::nodes::math::MathType::Exponent:
		return ccl::NodeMathType::NODE_MATH_EXPONENT;
	case pragma::scenekit::nodes::math::MathType::Radians:
		return ccl::NodeMathType::NODE_MATH_RADIANS;
	case pragma::scenekit::nodes::math::MathType::Degrees:
		return ccl::NodeMathType::NODE_MATH_DEGREES;
	case pragma::scenekit::nodes::math::MathType::SinH:
		return ccl::NodeMathType::NODE_MATH_SINH;
	case pragma::scenekit::nodes::math::MathType::CosH:
		return ccl::NodeMathType::NODE_MATH_COSH;
	case pragma::scenekit::nodes::math::MathType::TanH:
		return ccl::NodeMathType::NODE_MATH_TANH;
	case pragma::scenekit::nodes::math::MathType::Trunc:
		return ccl::NodeMathType::NODE_MATH_TRUNC;
	case pragma::scenekit::nodes::math::MathType::Snap:
		return ccl::NodeMathType::NODE_MATH_SNAP;
	case pragma::scenekit::nodes::math::MathType::Wrap:
		return ccl::NodeMathType::NODE_MATH_WRAP;
	case pragma::scenekit::nodes::math::MathType::Compare:
		return ccl::NodeMathType::NODE_MATH_COMPARE;
	case pragma::scenekit::nodes::math::MathType::MultiplyAdd:
		return ccl::NodeMathType::NODE_MATH_MULTIPLY_ADD;
	case pragma::scenekit::nodes::math::MathType::PingPong:
		return ccl::NodeMathType::NODE_MATH_PINGPONG;
	case pragma::scenekit::nodes::math::MathType::SmoothMin:
		return ccl::NodeMathType::NODE_MATH_SMOOTH_MIN;
	case pragma::scenekit::nodes::math::MathType::SmoothMax:
		return ccl::NodeMathType::NODE_MATH_SMOOTH_MAX;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::math::MathType::Add) == ccl::NodeMathType::NODE_MATH_ADD && umath::to_integral(pragma::scenekit::nodes::math::MathType::SmoothMax) == ccl::NodeMathType::NODE_MATH_SMOOTH_MAX);
	static_assert(umath::to_integral(pragma::scenekit::nodes::math::MathType::Count) == 40);
	// Unreachable
	return {};
}
ccl::NodeVectorTransformType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::vector_transform::Type type)
{
	switch(type) {
	case pragma::scenekit::nodes::vector_transform::Type::Vector:
		return ccl::NodeVectorTransformType::NODE_VECTOR_TRANSFORM_TYPE_VECTOR;
	case pragma::scenekit::nodes::vector_transform::Type::Point:
		return ccl::NodeVectorTransformType::NODE_VECTOR_TRANSFORM_TYPE_POINT;
	case pragma::scenekit::nodes::vector_transform::Type::Normal:
		return ccl::NodeVectorTransformType::NODE_VECTOR_TRANSFORM_TYPE_NORMAL;
	};
	static_assert(umath::to_integral(pragma::scenekit::nodes::vector_transform::Type::Count) == 4);
	// Unreachable
	return {};
}
ccl::NodeVectorMathType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::vector_math::MathType type)
{
	switch(type) {
	case pragma::scenekit::nodes::vector_math::MathType::Add:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_ADD;
	case pragma::scenekit::nodes::vector_math::MathType::Subtract:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_SUBTRACT;
	case pragma::scenekit::nodes::vector_math::MathType::Multiply:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_MULTIPLY;
	case pragma::scenekit::nodes::vector_math::MathType::Divide:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_DIVIDE;

	case pragma::scenekit::nodes::vector_math::MathType::CrossProduct:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_CROSS_PRODUCT;
	case pragma::scenekit::nodes::vector_math::MathType::Project:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_PROJECT;
	case pragma::scenekit::nodes::vector_math::MathType::Reflect:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_REFLECT;
	case pragma::scenekit::nodes::vector_math::MathType::DotProduct:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_DOT_PRODUCT;

	case pragma::scenekit::nodes::vector_math::MathType::Distance:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_DISTANCE;
	case pragma::scenekit::nodes::vector_math::MathType::Length:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_LENGTH;
	case pragma::scenekit::nodes::vector_math::MathType::Scale:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_SCALE;
	case pragma::scenekit::nodes::vector_math::MathType::Normalize:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_NORMALIZE;

	case pragma::scenekit::nodes::vector_math::MathType::Snap:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_SNAP;
	case pragma::scenekit::nodes::vector_math::MathType::Floor:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_FLOOR;
	case pragma::scenekit::nodes::vector_math::MathType::Ceil:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_CEIL;
	case pragma::scenekit::nodes::vector_math::MathType::Modulo:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_MODULO;
	case pragma::scenekit::nodes::vector_math::MathType::Fraction:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_FRACTION;
	case pragma::scenekit::nodes::vector_math::MathType::Absolute:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_ABSOLUTE;
	case pragma::scenekit::nodes::vector_math::MathType::Minimum:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_MINIMUM;
	case pragma::scenekit::nodes::vector_math::MathType::Maximum:
		return ccl::NodeVectorMathType::NODE_VECTOR_MATH_MAXIMUM;
	};
	static_assert(umath::to_integral(pragma::scenekit::nodes::vector_math::MathType::Add) == ccl::NodeVectorMathType::NODE_VECTOR_MATH_ADD && umath::to_integral(pragma::scenekit::nodes::vector_math::MathType::Maximum) == ccl::NodeVectorMathType::NODE_VECTOR_MATH_MAXIMUM);
	static_assert(umath::to_integral(pragma::scenekit::nodes::vector_math::MathType::Count) == 20);
	// Unreachable
	return {};
}

ccl::ustring pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::ColorSpace space)
{
	switch(space) {
	case pragma::scenekit::ColorSpace::Raw:
		return ccl::u_colorspace_raw;
	case pragma::scenekit::ColorSpace::Srgb:
		return ccl::u_colorspace_srgb;
	}
	static_assert(umath::to_integral(pragma::scenekit::ColorSpace::Count) == 3);
	// Unreachable
	return {};
}

ccl::NodeEnvironmentProjection pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::EnvironmentProjection projection)
{
	switch(projection) {
	case pragma::scenekit::EnvironmentProjection::Equirectangular:
		return ccl::NodeEnvironmentProjection::NODE_ENVIRONMENT_EQUIRECTANGULAR;
	case pragma::scenekit::EnvironmentProjection::MirrorBall:
		return ccl::NodeEnvironmentProjection::NODE_ENVIRONMENT_MIRROR_BALL;
	}
	static_assert(umath::to_integral(pragma::scenekit::EnvironmentProjection::Equirectangular) == ccl::NodeEnvironmentProjection::NODE_ENVIRONMENT_EQUIRECTANGULAR
	  && umath::to_integral(pragma::scenekit::EnvironmentProjection::MirrorBall) == ccl::NodeEnvironmentProjection::NODE_ENVIRONMENT_MIRROR_BALL);
	static_assert(umath::to_integral(pragma::scenekit::EnvironmentProjection::Count) == 2);
	// Unreachable
	return {};
}

ccl::ClosureType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::ClosureType type)
{
	switch(type) {
	case pragma::scenekit::ClosureType::BsdfMicroFacetMultiGgxGlass:
		return ccl::ClosureType::CLOSURE_BSDF_MICROFACET_MULTI_GGX_GLASS_ID;
	case pragma::scenekit::ClosureType::BsdfDiffuseToon:
		return ccl::ClosureType::CLOSURE_BSDF_DIFFUSE_TOON_ID;
	case pragma::scenekit::ClosureType::BsdfMicroFacetGgxGlass:
		return ccl::ClosureType::CLOSURE_BSDF_MICROFACET_GGX_GLASS_ID;
	}
	static_assert(umath::to_integral(pragma::scenekit::EnvironmentProjection::Equirectangular) == ccl::NodeEnvironmentProjection::NODE_ENVIRONMENT_EQUIRECTANGULAR
	  && umath::to_integral(pragma::scenekit::EnvironmentProjection::MirrorBall) == ccl::NodeEnvironmentProjection::NODE_ENVIRONMENT_MIRROR_BALL);
	static_assert(umath::to_integral(pragma::scenekit::ClosureType::Count) == 4);
	// Unreachable
	return {};
}

ccl::ImageAlphaType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::image_texture::AlphaType type)
{
	switch(type) {
	case pragma::scenekit::nodes::image_texture::AlphaType::Unassociated:
		return ccl::ImageAlphaType::IMAGE_ALPHA_UNASSOCIATED;
	case pragma::scenekit::nodes::image_texture::AlphaType::Associated:
		return ccl::ImageAlphaType::IMAGE_ALPHA_ASSOCIATED;
	case pragma::scenekit::nodes::image_texture::AlphaType::ChannelPacked:
		return ccl::ImageAlphaType::IMAGE_ALPHA_CHANNEL_PACKED;
	case pragma::scenekit::nodes::image_texture::AlphaType::Ignore:
		return ccl::ImageAlphaType::IMAGE_ALPHA_IGNORE;
	case pragma::scenekit::nodes::image_texture::AlphaType::Auto:
		return ccl::ImageAlphaType::IMAGE_ALPHA_AUTO;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::AlphaType::Unassociated) == ccl::ImageAlphaType::IMAGE_ALPHA_UNASSOCIATED && umath::to_integral(pragma::scenekit::nodes::image_texture::AlphaType::Auto) == ccl::ImageAlphaType::IMAGE_ALPHA_AUTO);
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::AlphaType::Count) == 5);
	// Unreachable
	return {};
}

ccl::InterpolationType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::image_texture::InterpolationType type)
{
	switch(type) {
	case pragma::scenekit::nodes::image_texture::InterpolationType::Linear:
		return ccl::InterpolationType::INTERPOLATION_LINEAR;
	case pragma::scenekit::nodes::image_texture::InterpolationType::Closest:
		return ccl::InterpolationType::INTERPOLATION_CLOSEST;
	case pragma::scenekit::nodes::image_texture::InterpolationType::Cubic:
		return ccl::InterpolationType::INTERPOLATION_CUBIC;
	case pragma::scenekit::nodes::image_texture::InterpolationType::Smart:
		return ccl::InterpolationType::INTERPOLATION_SMART;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::InterpolationType::Linear) == ccl::InterpolationType::INTERPOLATION_LINEAR && umath::to_integral(pragma::scenekit::nodes::image_texture::InterpolationType::Smart) == ccl::InterpolationType::INTERPOLATION_SMART);
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::InterpolationType::Count) == 4);
	// Unreachable
	return {};
}

ccl::ExtensionType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::image_texture::ExtensionType type)
{
	switch(type) {
	case pragma::scenekit::nodes::image_texture::ExtensionType::Repeat:
		return ccl::ExtensionType::EXTENSION_REPEAT;
	case pragma::scenekit::nodes::image_texture::ExtensionType::Extend:
		return ccl::ExtensionType::EXTENSION_EXTEND;
	case pragma::scenekit::nodes::image_texture::ExtensionType::Clip:
		return ccl::ExtensionType::EXTENSION_CLIP;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::ExtensionType::Repeat) == ccl::ExtensionType::EXTENSION_REPEAT && umath::to_integral(pragma::scenekit::nodes::image_texture::ExtensionType::Clip) == ccl::ExtensionType::EXTENSION_CLIP);
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::ExtensionType::Count) == 3);
	// Unreachable
	return {};
}

ccl::NodeImageProjection pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::image_texture::Projection type)
{
	switch(type) {
	case pragma::scenekit::nodes::image_texture::Projection::Flat:
		return ccl::NodeImageProjection::NODE_IMAGE_PROJ_FLAT;
	case pragma::scenekit::nodes::image_texture::Projection::Box:
		return ccl::NodeImageProjection::NODE_IMAGE_PROJ_BOX;
	case pragma::scenekit::nodes::image_texture::Projection::Sphere:
		return ccl::NodeImageProjection::NODE_IMAGE_PROJ_SPHERE;
	case pragma::scenekit::nodes::image_texture::Projection::Tube:
		return ccl::NodeImageProjection::NODE_IMAGE_PROJ_TUBE;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::Projection::Flat) == ccl::NodeImageProjection::NODE_IMAGE_PROJ_FLAT && umath::to_integral(pragma::scenekit::nodes::image_texture::Projection::Tube) == ccl::NodeImageProjection::NODE_IMAGE_PROJ_TUBE);
	static_assert(umath::to_integral(pragma::scenekit::nodes::image_texture::Projection::Count) == 4);
	// Unreachable
	return {};
}

ccl::NodeMappingType pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::mapping::Type type)
{
	switch(type) {
	case pragma::scenekit::nodes::mapping::Type::Point:
		return ccl::NodeMappingType::NODE_MAPPING_TYPE_POINT;
	case pragma::scenekit::nodes::mapping::Type::Texture:
		return ccl::NodeMappingType::NODE_MAPPING_TYPE_TEXTURE;
	case pragma::scenekit::nodes::mapping::Type::Vector:
		return ccl::NodeMappingType::NODE_MAPPING_TYPE_VECTOR;
	case pragma::scenekit::nodes::mapping::Type::Normal:
		return ccl::NodeMappingType::NODE_MAPPING_TYPE_NORMAL;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::mapping::Type::Point) == ccl::NodeMappingType::NODE_MAPPING_TYPE_POINT && umath::to_integral(pragma::scenekit::nodes::mapping::Type::Normal) == ccl::NodeMappingType::NODE_MAPPING_TYPE_NORMAL);
	static_assert(umath::to_integral(pragma::scenekit::nodes::mapping::Type::Count) == 4);
	// Unreachable
	return {};
}

ccl::NodeNormalMapSpace pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::normal_map::Space space)
{
	switch(space) {
	case pragma::scenekit::nodes::normal_map::Space::Tangent:
		return ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_TANGENT;
	case pragma::scenekit::nodes::normal_map::Space::Object:
		return ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_OBJECT;
	case pragma::scenekit::nodes::normal_map::Space::World:
		return ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_WORLD;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::normal_map::Space::Tangent) == ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_TANGENT && umath::to_integral(pragma::scenekit::nodes::normal_map::Space::World) == ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_WORLD);
	static_assert(umath::to_integral(pragma::scenekit::nodes::normal_map::Space::Count) == 3);
	// Unreachable
	return {};
}

ccl::NodeMix pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::mix::Mix mix)
{
	switch(mix) {
	case pragma::scenekit::nodes::mix::Mix::Blend:
		return ccl::NodeMix::NODE_MIX_BLEND;
	case pragma::scenekit::nodes::mix::Mix::Add:
		return ccl::NodeMix::NODE_MIX_ADD;
	case pragma::scenekit::nodes::mix::Mix::Mul:
		return ccl::NodeMix::NODE_MIX_MUL;
	case pragma::scenekit::nodes::mix::Mix::Sub:
		return ccl::NodeMix::NODE_MIX_SUB;
	case pragma::scenekit::nodes::mix::Mix::Screen:
		return ccl::NodeMix::NODE_MIX_SCREEN;
	case pragma::scenekit::nodes::mix::Mix::Div:
		return ccl::NodeMix::NODE_MIX_DIV;
	case pragma::scenekit::nodes::mix::Mix::Diff:
		return ccl::NodeMix::NODE_MIX_DIFF;
	case pragma::scenekit::nodes::mix::Mix::Dark:
		return ccl::NodeMix::NODE_MIX_DARK;
	case pragma::scenekit::nodes::mix::Mix::Light:
		return ccl::NodeMix::NODE_MIX_LIGHT;
	case pragma::scenekit::nodes::mix::Mix::Overlay:
		return ccl::NodeMix::NODE_MIX_OVERLAY;
	case pragma::scenekit::nodes::mix::Mix::Dodge:
		return ccl::NodeMix::NODE_MIX_DODGE;
	case pragma::scenekit::nodes::mix::Mix::Burn:
		return ccl::NodeMix::NODE_MIX_BURN;
	case pragma::scenekit::nodes::mix::Mix::Hue:
		return ccl::NodeMix::NODE_MIX_HUE;
	case pragma::scenekit::nodes::mix::Mix::Sat:
		return ccl::NodeMix::NODE_MIX_SAT;
	case pragma::scenekit::nodes::mix::Mix::Val:
		return ccl::NodeMix::NODE_MIX_VAL;
	case pragma::scenekit::nodes::mix::Mix::Color:
		return ccl::NodeMix::NODE_MIX_COL;
	case pragma::scenekit::nodes::mix::Mix::Soft:
		return ccl::NodeMix::NODE_MIX_SOFT;
	case pragma::scenekit::nodes::mix::Mix::Linear:
		return ccl::NodeMix::NODE_MIX_LINEAR;
	case pragma::scenekit::nodes::mix::Mix::Exclusion:
		return ccl::NodeMix::NODE_MIX_EXCLUSION;
	case pragma::scenekit::nodes::mix::Mix::Clamp:
		return ccl::NodeMix::NODE_MIX_CLAMP;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::mix::Mix::Blend) == ccl::NodeMix::NODE_MIX_BLEND && umath::to_integral(pragma::scenekit::nodes::mix::Mix::Clamp) == ccl::NodeMix::NODE_MIX_CLAMP);
	static_assert(umath::to_integral(pragma::scenekit::nodes::mix::Mix::Count) == 20);
	// Unreachable
	return {};
}
ccl::NodeVectorTransformConvertSpace pragma::scenekit::cycles::to_ccl_type(pragma::scenekit::nodes::vector_transform::ConvertSpace convertSpace)
{
	switch(convertSpace) {
	case pragma::scenekit::nodes::vector_transform::ConvertSpace::World:
		return ccl::NodeVectorTransformConvertSpace::NODE_VECTOR_TRANSFORM_CONVERT_SPACE_WORLD;
	case pragma::scenekit::nodes::vector_transform::ConvertSpace::Object:
		return ccl::NodeVectorTransformConvertSpace::NODE_VECTOR_TRANSFORM_CONVERT_SPACE_OBJECT;
	case pragma::scenekit::nodes::vector_transform::ConvertSpace::Camera:
		return ccl::NodeVectorTransformConvertSpace::NODE_VECTOR_TRANSFORM_CONVERT_SPACE_CAMERA;
	}
	static_assert(umath::to_integral(pragma::scenekit::nodes::vector_transform::ConvertSpace::World) == ccl::NodeVectorTransformConvertSpace::NODE_VECTOR_TRANSFORM_CONVERT_SPACE_WORLD
	  && umath::to_integral(pragma::scenekit::nodes::vector_transform::ConvertSpace::Camera) == ccl::NodeVectorTransformConvertSpace::NODE_VECTOR_TRANSFORM_CONVERT_SPACE_CAMERA);
	static_assert(umath::to_integral(pragma::scenekit::nodes::vector_transform::ConvertSpace::Count) == 3);
	// Unreachable
	return {};
}

pragma::scenekit::CCLShaderWrapper::CCLShaderWrapper(ccl::Shader &shader) : m_externallyOwnedShader {&shader} {}
pragma::scenekit::CCLShaderWrapper::CCLShaderWrapper(std::unique_ptr<ccl::Shader> &&shader) : m_cclShader {std::move(shader)} {}
ccl::Shader *pragma::scenekit::CCLShaderWrapper::GetShader() const { return m_externallyOwnedShader ? m_externallyOwnedShader : m_cclShader.get(); }
ccl::Shader *pragma::scenekit::CCLShaderWrapper::operator->() const { return GetShader(); }
ccl::Shader &pragma::scenekit::CCLShaderWrapper::operator*() const { return *GetShader(); }
std::unique_ptr<ccl::Shader> pragma::scenekit::CCLShaderWrapper::Steal()
{
	if(m_externallyOwnedShader)
		return {};
	auto res = std::move(m_cclShader);
	m_cclShader = {};
	return res;
}

pragma::scenekit::CCLShaderGraphWrapper::CCLShaderGraphWrapper(ccl::ShaderGraph &graph) : m_externallyOwnedGraph(&graph) {}
pragma::scenekit::CCLShaderGraphWrapper::CCLShaderGraphWrapper(std::unique_ptr<ccl::ShaderGraph> &&graph) : m_cclGraph(std::move(graph)) {}
ccl::ShaderGraph *pragma::scenekit::CCLShaderGraphWrapper::GetGraph() const { return m_externallyOwnedGraph ? m_externallyOwnedGraph : m_cclGraph.get(); }
ccl::ShaderGraph *pragma::scenekit::CCLShaderGraphWrapper::operator->() const { return GetGraph(); }
ccl::ShaderGraph &pragma::scenekit::CCLShaderGraphWrapper::operator*() const { return *GetGraph(); }
std::unique_ptr<ccl::ShaderGraph> pragma::scenekit::CCLShaderGraphWrapper::Steal()
{
	if(m_externallyOwnedGraph)
		return {};
	auto res = std::move(m_cclGraph);
	m_cclGraph = {};
	return res;
}

std::shared_ptr<pragma::scenekit::CCLShader> pragma::scenekit::CCLShader::Create(cycles::Renderer &renderer, CCLShaderWrapper cclShader, const GroupNodeDesc &desc, bool useCache)
{
	if(useCache) {
		auto shader = renderer.GetCachedShader(desc);
		if(shader)
			return shader;
	}
	cclShader->set_volume_sampling_method(ccl::VOLUME_SAMPLING_MULTIPLE_IMPORTANCE);

	auto graph = std::make_unique<ccl::ShaderGraph>();
	auto pShader = std::shared_ptr<CCLShader> {new CCLShader {renderer, std::move(cclShader), std::move(graph)}};
	pShader->m_flags |= Flags::CCLShaderOwnedByScene;

	pShader->InitializeNodeGraph(desc);
	if(useCache)
		renderer.AddShader(*pShader, &desc);
	return pShader;
}
std::shared_ptr<pragma::scenekit::CCLShader> pragma::scenekit::CCLShader::Create(cycles::Renderer &renderer, const GroupNodeDesc &desc)
{
	auto shader = renderer.GetCachedShader(desc);
	if(shader)
		return shader;
	auto pcclShader = std::make_unique<ccl::Shader>();
	auto *cclShader = pcclShader.get();
	cclShader->name = desc.GetName();
	renderer.GetCclScene()->shaders.push_back(std::move(pcclShader));
	shader = Create(renderer, *cclShader, desc, true);
	auto apiData = renderer.GetApiData();

	auto dontSimplify = false;
	apiData.GetFromPath("cycles/shader/dontSimplifyGraphs")(dontSimplify);
	if(!dontSimplify)
		shader->m_cclGraph->simplify(renderer.GetCclScene());

	auto dumpGraphs = false;
	apiData.GetFromPath("cycles/debug/dump_shader_graphs")(dumpGraphs);
	if(dumpGraphs) {
		auto &graph = *shader->m_cclGraph;
		auto localDumpPath = util::Path::CreatePath("temp/cycles/graph_dump");
		filemanager::create_path(localDumpPath.GetString());
		auto dumpPath = util::Path::CreatePath(filemanager::get_program_write_path()) + localDumpPath;
		auto idx = desc.GetIndex();
		std::string fileName = "graph_" + util::uuid_to_string(util::generate_uuid_v4()) + ".txt";
		auto filePath = dumpPath + util::Path::CreateFile(fileName);
		graph.dump_graph(filePath.GetString().c_str());
	}
	return shader;
}

pragma::scenekit::CCLShader::CCLShader(cycles::Renderer &renderer, CCLShaderWrapper cclShader, CCLShaderGraphWrapper cclShaderGraph) : m_renderer {renderer}, m_cclShader {std::move(cclShader)}, m_cclGraph {std::move(cclShaderGraph)} {}

pragma::scenekit::CCLShader::~CCLShader() {}

ccl::Shader *pragma::scenekit::CCLShader::operator->() { return m_cclShader.GetShader(); }
ccl::Shader *pragma::scenekit::CCLShader::operator*() { return m_cclShader.GetShader(); }

void pragma::scenekit::CCLShader::DoFinalize(Scene &scene)
{
	BaseObject::DoFinalize(scene);
	m_flags |= Flags::CCLShaderGraphOwnedByScene | Flags::CCLShaderOwnedByScene;
	auto cclGraph = m_cclGraph.Steal();
	if(!cclGraph)
		throw std::runtime_error {"Attempted to finalize externally-owned shader, this is not allowed!"};
	m_cclShader->set_graph(std::move(cclGraph));
	m_cclShader->tag_update(m_renderer.GetCclScene());
}

std::unique_ptr<pragma::scenekit::CCLShader::BaseNodeWrapper> pragma::scenekit::CCLShader::ResolveCustomNode(const std::string &typeName)
{
	if(typeName == pragma::scenekit::NODE_NORMAL_TEXTURE) {
		struct NormalNodeWrapper : public BaseNodeWrapper {
			virtual ccl::ShaderInput *FindInput(const std::string &name, ccl::ShaderNode **outNode) override
			{
				if(name == pragma::scenekit::nodes::normal_texture::IN_STRENGTH) {
					*outNode = normalMapNode;
					return pragma::scenekit::CCLShader::FindInput(*normalMapNode, pragma::scenekit::nodes::normal_map::IN_STRENGTH);
				}
				return nullptr;
			}
			virtual ccl::ShaderOutput *FindOutput(const std::string &name, ccl::ShaderNode **outNode) override
			{
				if(name == pragma::scenekit::nodes::normal_texture::OUT_NORMAL) {
					*outNode = normalMapNode;
					return pragma::scenekit::CCLShader::FindOutput(*normalMapNode, pragma::scenekit::nodes::normal_map::OUT_NORMAL);
				}
				return nullptr;
			}
			virtual const ccl::SocketType *FindProperty(const std::string &name, ccl::ShaderNode **outNode) override
			{
				if(name == pragma::scenekit::nodes::normal_texture::IN_FILENAME) {
					*outNode = imageTexNode;
					return imageTexNode->type->find_input(ccl::ustring {pragma::scenekit::nodes::image_texture::IN_FILENAME});
				}
				return nullptr;
			}
			virtual ccl::ShaderNode *GetOutputNode() override { return normalMapNode; }
			ccl::ImageTextureNode *imageTexNode = nullptr;
			ccl::NormalMapNode *normalMapNode = nullptr;
		};
		auto wrapper = std::make_unique<NormalNodeWrapper>();
		wrapper->imageTexNode = static_cast<ccl::ImageTextureNode *>(AddNode(pragma::scenekit::NODE_IMAGE_TEXTURE));
		assert(wrapper->imageTexNode);
		wrapper->imageTexNode->set_colorspace(ccl::u_colorspace_raw);
		wrapper->imageTexNode->set_alpha_type(ccl::ImageAlphaType::IMAGE_ALPHA_IGNORE);

		auto *sep = static_cast<ccl::SeparateRGBNode *>(AddNode(pragma::scenekit::NODE_SEPARATE_RGB));
		m_cclGraph->connect(FindOutput(*wrapper->imageTexNode, pragma::scenekit::nodes::image_texture::OUT_COLOR), FindInput(*sep, pragma::scenekit::nodes::separate_rgb::IN_COLOR));

		auto *cmb = static_cast<ccl::CombineRGBNode *>(AddNode(pragma::scenekit::NODE_COMBINE_RGB));
		m_cclGraph->connect(FindOutput(*sep, pragma::scenekit::nodes::separate_rgb::OUT_R), FindInput(*cmb, pragma::scenekit::nodes::combine_rgb::IN_G));
		m_cclGraph->connect(FindOutput(*sep, pragma::scenekit::nodes::separate_rgb::OUT_G), FindInput(*cmb, pragma::scenekit::nodes::combine_rgb::IN_R));
		m_cclGraph->connect(FindOutput(*sep, pragma::scenekit::nodes::separate_rgb::OUT_B), FindInput(*cmb, pragma::scenekit::nodes::combine_rgb::IN_B));

		wrapper->normalMapNode = static_cast<ccl::NormalMapNode *>(AddNode(pragma::scenekit::NODE_NORMAL_MAP));
		assert(wrapper->normalMapNode);
		wrapper->normalMapNode->set_space(ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_TANGENT);

		auto *normIn = FindInput(*wrapper->normalMapNode, pragma::scenekit::nodes::normal_map::IN_COLOR);
		m_cclGraph->connect(FindOutput(*cmb, pragma::scenekit::nodes::combine_rgb::OUT_IMAGE), normIn);
		return wrapper;
	}
	return nullptr;
}

ccl::ShaderNode *pragma::scenekit::CCLShader::AddNode(const std::string &typeName)
{
	auto *nodeType = ccl::NodeType::find(ccl::ustring {typeName});
	if(!nodeType)
		return nullptr;
	auto *snode = m_cclGraph->create_node(nodeType);
	if(snode == nullptr)
		return nullptr;

	auto name = GetCurrentInternalNodeName();
	snode->name = name;
	return snode;
}

void pragma::scenekit::CCLShader::InitializeNode(const NodeDesc &desc, std::unordered_map<const NodeDesc *, ccl::ShaderNode *> &nodeToCclNode, const GroupSocketTranslationTable &groupIoSockets)
{
	if(desc.IsGroupNode()) {
		auto &groupDesc = *static_cast<const GroupNodeDesc *>(&desc);
		auto &childNodes = groupDesc.GetChildNodes();
		for(auto &childNode : childNodes)
			InitializeNode(*childNode, nodeToCclNode, groupIoSockets);

		auto getCclSocket = [this, &groupIoSockets, &nodeToCclNode](const Socket &socket, bool input) -> std::optional<std::pair<ccl::ShaderNode *, std::string>> {
			auto it = groupIoSockets.find(socket);
			if(it != groupIoSockets.end())
				return input ? it->second.input : it->second.output;
			std::string socketName;
			auto *node = socket.GetNode(socketName);
			auto itNode = nodeToCclNode.find(node);
			if(itNode == nodeToCclNode.end()) {
				// m_scene.HandleError("Unable to locate ccl node for from node '" +node->GetName() +"'!");
				return {};
			}
			auto *cclNode = itNode->second;
			return std::pair<ccl::ShaderNode *, std::string> {cclNode, socketName};
		};
		auto &links = groupDesc.GetLinks();
		for(auto &link : links) {
			auto cclFromSocket = getCclSocket(link.fromSocket, false);
			auto cclToSocket = getCclSocket(link.toSocket, true);
			if(cclFromSocket.has_value() == false || cclToSocket.has_value() == false) {
				if(cclToSocket.has_value() && link.fromSocket.IsNodeSocket() && link.fromSocket.IsOutputSocket() == false) {
					std::string fromSocketName;
					auto *fromNode = link.fromSocket.GetNode(fromSocketName);
					if(fromNode) {
						auto *fromSocketDesc = fromNode->FindPropertyDesc(fromSocketName);
						// This is a special case where the input socket is actually a property,
						// so instead of linking, we just assign the property value directly.
						auto inputName = TranslateInputName(*cclToSocket->first, cclToSocket->second);
						auto *prop = FindProperty(*cclToSocket->first, inputName);
						if(fromSocketDesc && fromSocketDesc->dataValue.value) {
							if(prop)
								ApplySocketValue(*cclToSocket->first, inputName, *fromSocketDesc, *cclToSocket->first, *prop);
							else
								m_renderer.GetScene().HandleError("Attempted to use unknown property '" + inputName + "' with node of type '" + std::string {typeid(*cclToSocket->first).name()} + "'!");
						}
					}
				}
				continue;
			}
			auto *output = FindOutput(*cclFromSocket->first, cclFromSocket->second);
			auto *input = FindInput(*cclToSocket->first, cclToSocket->second);
			if(output == nullptr) {
				m_renderer.GetScene().HandleError("Invalid CCL output '" + cclFromSocket->second + "' for node of type '" + std::string {typeid(*cclFromSocket->first).name()} + "'!");
				continue;
			}
			if(input == nullptr) {
				m_renderer.GetScene().HandleError("Invalid CCL input '" + cclToSocket->second + "' for node of type '" + std::string {typeid(*cclToSocket->first).name()} + "'!");
				continue;
			}
			m_cclGraph->connect(output, input);
		}
		return;
	}
	auto &typeName = desc.GetTypeName();
	if(typeName == "output") {
		// Output node already exists by default
		nodeToCclNode[&desc] = m_cclGraph->output();
		return;
	}
	struct CclNodeWrapper : public pragma::scenekit::CCLShader::BaseNodeWrapper {
		virtual ccl::ShaderInput *FindInput(const std::string &name, ccl::ShaderNode **outNode) override
		{
			*outNode = node;
			return pragma::scenekit::CCLShader::FindInput(*node, name);
		}
		virtual ccl::ShaderOutput *FindOutput(const std::string &name, ccl::ShaderNode **outNode) override
		{
			*outNode = node;
			return pragma::scenekit::CCLShader::FindOutput(*node, name);
		}
		virtual const ccl::SocketType *FindProperty(const std::string &name, ccl::ShaderNode **outNode) override
		{
			*outNode = node;
			return node->type->find_input(ccl::ustring {name});
		}
		virtual ccl::ShaderNode *GetOutputNode() override { return node; }
		ccl::ShaderNode *node = nullptr;
	};

	auto *snode = AddNode(typeName);
	std::unique_ptr<pragma::scenekit::CCLShader::BaseNodeWrapper> wrapper = nullptr;
	auto isCclNode = false;
	if(snode != nullptr) {
		wrapper = std::make_unique<CclNodeWrapper>();
		static_cast<CclNodeWrapper *>(wrapper.get())->node = snode;
		isCclNode = true;
	}
	else {
		auto customNode = ResolveCustomNode(typeName);
		if(customNode == nullptr) {
			m_renderer.GetScene().HandleError("Unable to create ccl node of type '" + typeName + "': Invalid type?");
			return;
		}
		wrapper = std::move(customNode);
	}
	for(auto &pair : desc.GetInputs()) {
		ccl::ShaderNode *node;
		auto inputName = pair.first;
		if(isCclNode)
			inputName = TranslateInputName(*static_cast<CclNodeWrapper *>(wrapper.get())->node, inputName);
		auto *input = wrapper->FindInput(inputName, &node);
		if(input == nullptr) {
			m_renderer.GetScene().HandleError("Attempted to use unknown input '" + pair.first + "' with node of type '" + typeName + "'!");
			continue;
		}
		ApplySocketValue(*node, inputName, pair.second, *node, input->socket_type);
	}

	for(auto &pair : desc.GetProperties()) {
		ccl::ShaderNode *node;
		auto inputName = pair.first;
		if(isCclNode)
			inputName = TranslateInputName(*static_cast<CclNodeWrapper *>(wrapper.get())->node, inputName);
		auto *type = wrapper->FindProperty(inputName, &node);
		if(type == nullptr) {
			m_renderer.GetScene().HandleError("Attempted to use unknown property '" + pair.first + "' with node of type '" + typeName + "'!");
			continue;
		}
		ApplySocketValue(*node, inputName, pair.second, *node, *type);
	}

	nodeToCclNode[&desc] = wrapper->GetOutputNode();
}

template<typename TSrc, typename TDst>
static ccl::array<TDst> to_ccl_array(const std::vector<TSrc> &input, const std::function<TDst(const TSrc &)> &converter)
{
	ccl::array<TDst> output {};
	output.resize(input.size());
	for(auto i = decltype(input.size()) {0u}; i < input.size(); ++i)
		output[i] = converter(input.at(i));
	return output;
}

std::string pragma::scenekit::CCLShader::TranslateInputName(const ccl::ShaderNode &node, const std::string &inputName)
{
	// Some Cycles node socket names don't match ours (due to Cycles updates or other reasons), so we'll have to translate them here
	if(typeid(node) == typeid(ccl::MathNode)) {
		if(ustring::compare(inputName.c_str(), pragma::scenekit::nodes::math::IN_TYPE, false))
			return "math_type";
	}
	else if(typeid(node) == typeid(ccl::MappingNode)) {
		if(ustring::compare(inputName.c_str(), pragma::scenekit::nodes::mapping::IN_TYPE, false))
			return "mapping_type";
	}
	else if(typeid(node) == typeid(ccl::MixNode)) {
		if(ustring::compare(inputName.c_str(), pragma::scenekit::nodes::mix::IN_TYPE, false))
			return "mix_type";
	}
	else if(typeid(node) == typeid(ccl::VectorMathNode)) {
		if(ustring::compare(inputName.c_str(), pragma::scenekit::nodes::vector_math::IN_TYPE, false))
			return "math_type";
	}
	else if(typeid(node) == typeid(ccl::VectorTransformNode)) {
		if(ustring::compare(inputName.c_str(), pragma::scenekit::nodes::vector_transform::IN_TYPE, false))
			return "transform_type";
	}
	return inputName;
}

template<typename TCcl, typename TEnum>
static bool apply_translated_socket_value(const ccl::ShaderNode &shaderNode, const std::string &socketName, const std::string &targetSocketName, const pragma::scenekit::NodeSocketDesc &sockDesc, ccl::Node &node, const ccl::SocketType &sockType)
{
	if(typeid(shaderNode) != typeid(TCcl))
		return false;
	if(!ustring::compare(socketName.c_str(), targetSocketName.c_str(), false))
		return false;
	if(sockDesc.dataValue.type != pragma::scenekit::SocketType::Enum)
		return false;
	auto val = *static_cast<pragma::scenekit::STEnum *>(sockDesc.dataValue.value.get());
	val = pragma::scenekit::cycles::to_ccl_enum<TEnum>(val);
	node.set(sockType, val);
	return true;
}

void pragma::scenekit::CCLShader::ApplySocketValue(const ccl::ShaderNode &shaderNode, const std::string &socketName, const NodeSocketDesc &sockDesc, ccl::Node &node, const ccl::SocketType &sockType)
{
	if(sockDesc.dataValue.type == SocketType::Closure)
		return;

	if(apply_translated_socket_value<ccl::MathNode, pragma::scenekit::nodes::math::MathType>(shaderNode, socketName, "math_type", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::MappingNode, pragma::scenekit::nodes::mapping::Type>(shaderNode, socketName, "mapping_type", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::MixNode, pragma::scenekit::nodes::mix::Mix>(shaderNode, socketName, "mix_type", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::VectorMathNode, pragma::scenekit::nodes::vector_math::MathType>(shaderNode, socketName, "math_type", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::VectorTransformNode, pragma::scenekit::nodes::vector_transform::Type>(shaderNode, socketName, "transform_type", sockDesc, node, sockType))
		return;

	if(apply_translated_socket_value<ccl::ImageTextureNode, pragma::scenekit::nodes::image_texture::InterpolationType>(shaderNode, socketName, "interpolation", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::ImageTextureNode, pragma::scenekit::nodes::image_texture::ExtensionType>(shaderNode, socketName, "extension", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::ImageTextureNode, pragma::scenekit::nodes::image_texture::Projection>(shaderNode, socketName, "projection", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::ImageTextureNode, pragma::scenekit::nodes::image_texture::AlphaType>(shaderNode, socketName, "alpha_type", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::VectorTransformNode, pragma::scenekit::nodes::vector_transform::ConvertSpace>(shaderNode, socketName, "convert_from", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::VectorTransformNode, pragma::scenekit::nodes::vector_transform::ConvertSpace>(shaderNode, socketName, "convert_to", sockDesc, node, sockType))
		return;
	if(apply_translated_socket_value<ccl::NormalMapNode, pragma::scenekit::nodes::normal_map::Space>(shaderNode, socketName, "space", sockDesc, node, sockType))
		return;

	if(!sockDesc.dataValue.value) {
		auto &logger = pragma::scenekit::get_logger();
		if(logger)
			logger->error("Socket '{}' of shader node '{}' has invalid value!", socketName, shaderNode.name.string());
		return;
	}

	switch(sockDesc.dataValue.type) {
	case SocketType::Bool:
		static_assert(std::is_same_v<STBool, bool>);
		node.set(sockType, *static_cast<STBool *>(sockDesc.dataValue.value.get()));
		break;
	case SocketType::Float:
		static_assert(std::is_same_v<STFloat, float>);
		node.set(sockType, *static_cast<STFloat *>(sockDesc.dataValue.value.get()));
		break;
	case SocketType::Int:
		static_assert(std::is_same_v<STInt, int32_t>);
		node.set(sockType, *static_cast<STInt *>(sockDesc.dataValue.value.get()));
		break;
	case SocketType::Enum:
		static_assert(std::is_same_v<STEnum, int32_t>);
		node.set(sockType, *static_cast<STEnum *>(sockDesc.dataValue.value.get()));
		break;
	case SocketType::UInt:
		static_assert(std::is_same_v<STUInt, ccl::uint>);
		node.set(sockType, *static_cast<STUInt *>(sockDesc.dataValue.value.get()));
		break;
	case SocketType::Color:
	case SocketType::Vector:
	case SocketType::Point:
	case SocketType::Normal:
		{
			static_assert(std::is_same_v<STColor, Vector3> && std::is_same_v<STVector, Vector3> && std::is_same_v<STPoint, Vector3> && std::is_same_v<STNormal, Vector3>);
			auto &v = *static_cast<STVector *>(sockDesc.dataValue.value.get());
			node.set(sockType, ccl::float3 {v.x, v.y, v.z});
			break;
		}
	case SocketType::Point2:
		{
			static_assert(std::is_same_v<STPoint2, Vector2>);
			auto &v = *static_cast<STPoint2 *>(sockDesc.dataValue.value.get());
			node.set(sockType, ccl::float2 {v.x, v.y});
			break;
		}
	case SocketType::String:
		{
			static_assert(std::is_same_v<STString, std::string>);
			auto &v = *static_cast<std::string *>(sockDesc.dataValue.value.get());

			if (socketName == "filename") {
				// Make file paths absolute
				std::string absPath;
				if (filemanager::find_absolute_path(v, absPath)) {
					node.set(sockType, absPath.c_str());
					break;
				}
			}

			node.set(sockType, v.c_str());
			break;
		}
	case SocketType::Transform:
		{
			static_assert(std::is_same_v<STTransform, Mat4x3>);
			auto &v = *static_cast<Mat4x3 *>(sockDesc.dataValue.value.get());
			node.set(sockType, ccl::Transform {v[0][0], v[0][1], v[0][2], v[1][0], v[1][1], v[1][2], v[2][0], v[2][1], v[2][2], v[3][0], v[3][1], v[3][2]});
			break;
		}
	case SocketType::FloatArray:
		{
			static_assert(std::is_same_v<STFloatArray, std::vector<STFloat>>);
			auto &v = *static_cast<std::vector<STFloat> *>(sockDesc.dataValue.value.get());
			auto cclArray = to_ccl_array<float, float>(v, [](const float &v) -> float { return v; });
			node.set(sockType, cclArray);
			break;
		}
	case SocketType::ColorArray:
		{
			static_assert(std::is_same_v<STColorArray, std::vector<STColor>>);
			auto &v = *static_cast<std::vector<STColor> *>(sockDesc.dataValue.value.get());
			auto cclArray = to_ccl_array<Vector3, ccl::float3>(v, [](const Vector3 &v) -> ccl::float3 { return ccl::float3 {v.x, v.y, v.z}; });
			node.set(sockType, cclArray);
			break;
		}
	}
	static_assert(umath::to_integral(SocketType::Count) == 16);
}

void pragma::scenekit::CCLShader::ConvertGroupSocketsToNodes(const GroupNodeDesc &groupDesc, GroupSocketTranslationTable &outGroupIoSockets)
{
	// Note: Group nodes don't exist in Cycles, they're implicit and replaced by their contents.
	// To do so, we convert the input and output sockets to constant nodes and re-direct all links
	// that point to these sockets to the new nodes instead.
	auto convertGroupSocketsToNodes = [this, &groupDesc, &outGroupIoSockets](const std::unordered_map<std::string, NodeSocketDesc> &sockets, bool output) {
		for(auto &pair : sockets) {
			Socket socket {const_cast<GroupNodeDesc &>(groupDesc), pair.first, output};
			auto &socketDesc = pair.second;
			GroupSocketTranslation socketTranslation {};
			if(is_convertible_to(socketDesc.dataValue.type, SocketType::Float)) {
				auto *nodeMath = static_cast<ccl::MathNode *>(AddNode(NODE_MATH));
				assert(nodeMath);
				nodeMath->set_math_type(ccl::NodeMathType::NODE_MATH_ADD);
				nodeMath->set_value1(0.f);
				nodeMath->set_value2(0.f);

				if(socketDesc.dataValue.value) {
					auto v = socketDesc.dataValue.ToValue<float>();
					if(v.has_value())
						nodeMath->set_value1(*v);
				}
				socketTranslation.input = {nodeMath, nodes::math::IN_VALUE1};
				socketTranslation.output = {nodeMath, nodes::math::OUT_VALUE};
			}
			else if(is_convertible_to(socketDesc.dataValue.type, SocketType::Vector)) {
				auto *nodeVec = static_cast<ccl::VectorMathNode *>(AddNode(NODE_VECTOR_MATH));
				assert(nodeVec);
				nodeVec->set_math_type(ccl::NodeVectorMathType::NODE_VECTOR_MATH_ADD);
				nodeVec->set_vector1({0.f, 0.f, 0.f});
				nodeVec->set_vector2({0.f, 0.f, 0.f});

				if(socketDesc.dataValue.value) {
					auto v = socketDesc.dataValue.ToValue<Vector3>();
					if(v.has_value())
						nodeVec->set_vector1({v->x, v->y, v->z});
				}
				socketTranslation.input = {nodeVec, nodes::vector_math::IN_VECTOR1};
				socketTranslation.output = {nodeVec, nodes::vector_math::OUT_VECTOR};
			}
			else if(socketDesc.dataValue.type == pragma::scenekit::SocketType::Closure) {
				auto *mix = static_cast<ccl::MixClosureNode *>(AddNode(NODE_MIX_CLOSURE));
				assert(mix);
				mix->set_fac(0.f);

				socketTranslation.input = {mix, nodes::mix_closure::IN_CLOSURE1};
				socketTranslation.output = {mix, nodes::mix_closure::OUT_CLOSURE};
			}
			else {
				// m_scene.HandleError("Group node has socket of type '" +to_string(socketDesc.dataValue.type) +"', but only float and vector types are allowed!");
				continue;
			}
			outGroupIoSockets[socket] = socketTranslation;
		}
	};
	convertGroupSocketsToNodes(groupDesc.GetInputs(), false);
	convertGroupSocketsToNodes(groupDesc.GetProperties(), false);
	convertGroupSocketsToNodes(groupDesc.GetOutputs(), true);

	for(auto &node : groupDesc.GetChildNodes()) {
		if(node->IsGroupNode() == false)
			continue;
		ConvertGroupSocketsToNodes(static_cast<GroupNodeDesc &>(*node), outGroupIoSockets);
	}
}

void pragma::scenekit::CCLShader::InitializeNodeGraph(const GroupNodeDesc &desc)
{
	GroupSocketTranslationTable groupIoSockets;
	ConvertGroupSocketsToNodes(desc, groupIoSockets);

	std::unordered_map<const NodeDesc *, ccl::ShaderNode *> nodeToCclNode;
	InitializeNode(desc, nodeToCclNode, groupIoSockets);
}

const ccl::SocketType *pragma::scenekit::CCLShader::FindProperty(ccl::ShaderNode &node, const std::string &inputName)
{
	auto translatedInputName = TranslateInputName(node, inputName);
	auto it = std::find_if(node.type->inputs.begin(), node.type->inputs.end(), [&translatedInputName](const ccl::SocketType &socketType) { return ccl::string_iequals(socketType.name.string(), translatedInputName); });
	return (it != node.type->inputs.end()) ? &*it : nullptr;
}
ccl::ShaderInput *pragma::scenekit::CCLShader::FindInput(ccl::ShaderNode &node, const std::string &inputName)
{
	// return node.input(ccl::ustring{inputName}); // Doesn't work in some cases for some reason
	auto translatedInputName = TranslateInputName(node, inputName);
	for(auto *shInput : node.inputs) {
		if(ccl::string_iequals(shInput->socket_type.name.string(), translatedInputName))
			return shInput;
	}
	return nullptr;
}
ccl::ShaderOutput *pragma::scenekit::CCLShader::FindOutput(ccl::ShaderNode &node, const std::string &outputName)
{
	// return node.output(ccl::ustring{outputName}); // Doesn't work in some cases for some reason
	for(auto *shOutput : node.outputs) {
		if(ccl::string_iequals(shOutput->socket_type.name.string(), outputName))
			return shOutput;
	}
	return nullptr;
}

std::string pragma::scenekit::CCLShader::GetCurrentInternalNodeName() const { return "internal_" + std::to_string(m_cclGraph->nodes.size()); }
