// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <util/math.h>
#include <scene/shader_nodes.h>
#include <kernel/types.h>
#include <kernel/svm/types.h>

export module pragma.scenekit.cycles:ccl_shader;

import pragma.scenekit;

export namespace pragma::scenekit {
	namespace cycles {
		class Renderer;
		ccl::NodeMathType to_ccl_type(pragma::scenekit::nodes::math::MathType type);
		ccl::NodeVectorMathType to_ccl_type(pragma::scenekit::nodes::vector_math::MathType type);
		ccl::NodeVectorTransformType to_ccl_type(pragma::scenekit::nodes::vector_transform::Type type);
		ccl::ustring to_ccl_type(pragma::scenekit::ColorSpace space);
		ccl::NodeEnvironmentProjection to_ccl_type(pragma::scenekit::EnvironmentProjection projection);
		ccl::ClosureType to_ccl_type(pragma::scenekit::ClosureType type);
		ccl::ImageAlphaType to_ccl_type(pragma::scenekit::nodes::image_texture::AlphaType type);
		ccl::InterpolationType to_ccl_type(pragma::scenekit::nodes::image_texture::InterpolationType type);
		ccl::ExtensionType to_ccl_type(pragma::scenekit::nodes::image_texture::ExtensionType type);
		ccl::NodeImageProjection to_ccl_type(pragma::scenekit::nodes::image_texture::Projection type);
		ccl::NodeMappingType to_ccl_type(pragma::scenekit::nodes::mapping::Type type);
		ccl::NodeNormalMapSpace to_ccl_type(pragma::scenekit::nodes::normal_map::Space space);
		ccl::NodeMix to_ccl_type(pragma::scenekit::nodes::mix::Mix mix);
		ccl::NodeVectorTransformConvertSpace to_ccl_type(pragma::scenekit::nodes::vector_transform::ConvertSpace convertSpace);
		template<typename T>
		pragma::scenekit::STEnum to_ccl_enum(pragma::scenekit::STEnum uniEnum)
		{
			return static_cast<STEnum>(to_ccl_type(static_cast<T>(uniEnum)));
		}
	};
	struct GroupSocketTranslation {
		std::pair<ccl::ShaderNode *, std::string> input;
		std::pair<ccl::ShaderNode *, std::string> output;
	};
	using GroupSocketTranslationTable = std::unordered_map<Socket, GroupSocketTranslation, SocketHasher>;
	struct CCLNodeFactory;

	struct CCLShaderWrapper {
	  public:
		CCLShaderWrapper(ccl::Shader &shader);
		CCLShaderWrapper(std::unique_ptr<ccl::Shader> &&shader);
		ccl::Shader *GetShader() const;
		ccl::Shader *operator->() const;
		ccl::Shader &operator*() const;
		std::unique_ptr<ccl::Shader> Steal();
	  private:
		std::unique_ptr<ccl::Shader> m_cclShader;
		ccl::Shader *m_externallyOwnedShader = nullptr;
	};

	class CCLShaderGraphWrapper {
	  public:
		CCLShaderGraphWrapper(ccl::ShaderGraph &graph);
		CCLShaderGraphWrapper(std::unique_ptr<ccl::ShaderGraph> &&graph);
		ccl::ShaderGraph *GetGraph() const;
		ccl::ShaderGraph *operator->() const;
		ccl::ShaderGraph &operator*() const;
		std::unique_ptr<ccl::ShaderGraph> Steal();
	  private:
		std::unique_ptr<ccl::ShaderGraph> m_cclGraph;
		ccl::ShaderGraph *m_externallyOwnedGraph = nullptr;
	};

	class CCLShader : public std::enable_shared_from_this<CCLShader>, public BaseObject {
	  public:
		enum class Flags : uint8_t {
			None = 0u,

			CCLShaderOwnedByScene = 1u,
			CCLShaderGraphOwnedByScene = CCLShaderOwnedByScene << 1u
		};
		static std::shared_ptr<CCLShader> Create(cycles::Renderer &renderer, const GroupNodeDesc &desc);
		static std::shared_ptr<CCLShader> Create(cycles::Renderer &renderer, CCLShaderWrapper cclShader, const GroupNodeDesc &desc, bool useCache = false);
		static ccl::ShaderInput *FindInput(ccl::ShaderNode &node, const std::string &inputName);
		static ccl::ShaderOutput *FindOutput(ccl::ShaderNode &node, const std::string &outputName);
		static const ccl::SocketType *FindProperty(ccl::ShaderNode &node, const std::string &inputName);

		~CCLShader();
		void InitializeNodeGraph(const GroupNodeDesc &desc);

		ccl::Shader *operator->();
		ccl::Shader *operator*();
	  protected:
		CCLShader(cycles::Renderer &renderer, CCLShaderWrapper cclShader, CCLShaderGraphWrapper cclShaderGraph);
		virtual void DoFinalize(Scene &scene) override;
		void InitializeNode(const NodeDesc &desc, std::unordered_map<const NodeDesc *, ccl::ShaderNode *> &nodeToCclNode, const GroupSocketTranslationTable &groupIoSockets);
		static std::string TranslateInputName(const ccl::ShaderNode &node, const std::string &inputName);
		void ConvertGroupSocketsToNodes(const GroupNodeDesc &groupDesc, GroupSocketTranslationTable &outGroupIoSockets);
		static void ApplySocketValue(const ccl::ShaderNode &shaderNode, const std::string &socketName, const NodeSocketDesc &sockDesc, ccl::Node &node, const ccl::SocketType &sockType);
		std::string GetCurrentInternalNodeName() const;
		friend CCLNodeFactory;
	  private:
		struct BaseNodeWrapper {
			virtual ccl::ShaderInput *FindInput(const std::string &name, ccl::ShaderNode **outNode) = 0;
			virtual ccl::ShaderOutput *FindOutput(const std::string &name, ccl::ShaderNode **outNode) = 0;
			virtual const ccl::SocketType *FindProperty(const std::string &name, ccl::ShaderNode **outNode) = 0;
			virtual ccl::ShaderNode *GetOutputNode() = 0;
			virtual ~BaseNodeWrapper() = default;
		};
		std::unique_ptr<BaseNodeWrapper> ResolveCustomNode(const std::string &typeName);
		ccl::ShaderNode *AddNode(const std::string &type);

		ccl::Shader *GetCclShader() { return m_cclShader.GetShader(); }
		ccl::ShaderGraph *GetCclGraph() { return m_cclGraph.GetGraph(); }

		CCLShaderWrapper m_cclShader;
		CCLShaderGraphWrapper m_cclGraph;

		Flags m_flags = Flags::None;
		cycles::Renderer &m_renderer;
	};
	using namespace umath::scoped_enum::bitwise;
};
export {
	namespace umath::scoped_enum::bitwise {
		template<>
		struct enable_bitwise_operators<pragma::scenekit::CCLShader::Flags> : std::true_type {};
	}
}
