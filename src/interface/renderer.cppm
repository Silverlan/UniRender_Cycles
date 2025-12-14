// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "util_enum_flags.hpp"

#include <util/math.h>
#include <session/session.h>
#include <scene/shader_graph.h>

export module pragma.scenekit.cycles:renderer;

import :ccl_shader;
import pragma.scenekit;

export namespace pragma::scenekit::cycles {
	void compute_tangents(ccl::Mesh *mesh, bool need_sign, bool active_render);
	class DisplayDriver;
	class OutputDriver;
	class Renderer : public pragma::scenekit::Renderer {
	  public:
		static std::shared_ptr<Renderer> Create(const pragma::scenekit::Scene &scene, std::string &outErr, Flags flags);
		static constexpr ccl::AttributeStandard ALPHA_ATTRIBUTE_TYPE = ccl::AttributeStandard::ATTR_STD_POINTINESS;

		enum class StateFlags : uint32_t {
			None = 0u,
			SkyInitialized = 1u,
			RenderingStarted = SkyInitialized << 1u,
			ProgressiveRefine = RenderingStarted << 1u,
			NativeDenoising = ProgressiveRefine << 1u,
			SessionWasStarted = NativeDenoising << 1u,
			ReloadSessionScheduled = SessionWasStarted << 1u
		};

		static Vector3 ToPragmaPosition(const ccl::float3 &pos);
		static ccl::float3 ToCyclesVector(const Vector3 &v);
		static ccl::float3 ToCyclesPosition(const Vector3 &pos);
		static ccl::float3 ToCyclesNormal(const Vector3 &n);
		static ccl::float2 ToCyclesUV(const Vector2 &uv);
		static ccl::Transform ToCyclesTransform(const pragma::math::ScaledTransform &t, bool applyRotOffset = false, bool inverseDirection = false);
		static float ToCyclesLength(float len);

		virtual ~Renderer() override;
		virtual void Wait() override;
		virtual void Start() override {} // TODO: Remove
		virtual float GetProgress() const override;
		virtual bool Stop() override;
		virtual bool Pause() override;
		virtual bool Resume() override;
		virtual bool Suspend() override;
		virtual bool Export(const std::string &path) override;
		virtual void Reset() override;
		virtual void Restart() override;
		virtual bool ShouldUseProgressiveFloatFormat() const override { return false; }
		virtual std::optional<std::string> SaveRenderPreview(const std::string &path, std::string &outErr) const override;
		virtual pragma::util::ParallelJob<image::ImageLayerSet> StartRender() override;
		virtual bool IsFeatureEnabled(Feature feature) const override;

		virtual bool BeginSceneEdit() override;
		virtual bool EndSceneEdit() override;
		virtual bool SyncEditedActor(const pragma::util::Uuid &uuid) override;
		virtual bool AddLiveActor(pragma::scenekit::WorldObject &actor) override;

		ccl::Object *FindCclObject(const Object &obj);
		const ccl::Object *FindCclObject(const Object &obj) const { return const_cast<Renderer *>(this)->FindCclObject(obj); }
		ccl::Mesh *FindCclMesh(const Mesh &mesh);
		const ccl::Mesh *FindCclMesh(const Mesh &mesh) const { return const_cast<Renderer *>(this)->FindCclMesh(mesh); }
		ccl::Light *FindCclLight(const Light &light);
		const ccl::Light *FindCclLight(const Light &light) const { return const_cast<Renderer *>(this)->FindCclLight(light); }

		ccl::Scene *operator->() { return m_cclScene; }
		const ccl::Scene *operator->() const { return const_cast<Renderer *>(this)->operator->(); }
		ccl::Scene &operator*() { return *operator->(); }
		const ccl::Scene &operator*() const { return const_cast<Renderer *>(this)->operator*(); }

		std::optional<uint32_t> FindCCLObjectId(const ccl::Object &o) const;
		ccl::Session *GetCclSession() { return m_cclSession.get(); }
		const ccl::Session *GetCclSession() const { return const_cast<Renderer *>(this)->GetCclSession(); }
		ccl::Scene *GetCclScene() { return m_cclScene; }
		const ccl::Scene *GetCclScene() const { return const_cast<Renderer *>(this)->GetCclScene(); }

		std::shared_ptr<CCLShader> GetCachedShader(const GroupNodeDesc &desc) const;
		void AddShader(CCLShader &shader, const GroupNodeDesc *optDesc = nullptr);

		DisplayDriver *GetDisplayDriver() { return m_displayDriver; }
		OutputDriver *GetOutputDriver() { return m_outputDriver; }
		//static void UpdateRenderTile(TileManager &tileManager,const ccl::RenderTile &tile,bool param);
		//static void WriteRenderTile(TileManager &tileManager,const ccl::RenderTile &tile);

		// For internal use only
		void SetStereoscopicEye(StereoEye eye);
	  private:
		Renderer(const Scene &scene, Flags flags);

		virtual void SetCancelled(const std::string &msg = "Cancelled by application.") override;
		bool InitializeBakingData();
		void FinalizeAndCloseCyclesScene();
		void CloseCyclesScene();
		void ApplyPostProcessing(image::ImageBuffer &imgBuffer, pragma::scenekit::Scene::RenderMode renderMode);
		void InitializeAlbedoPass(bool reloadShaders);
		void InitializeNormalPass(bool reloadShaders);
		void InitializePassShaders(const std::function<std::shared_ptr<GroupNodeDesc>(const Shader &)> &fGetPassDesc);
		void AddSkybox(const std::string &texture);
		virtual pragma::util::EventReply HandleRenderStage(RenderWorker &worker, pragma::scenekit::Renderer::ImageRenderStage stage, StereoEye eyeStage, pragma::scenekit::Renderer::RenderStageResult *optResult = nullptr) override;
		void WaitForRenderStage(RenderWorker &worker, float baseProgress, float progressMultiplier, const std::function<pragma::scenekit::Renderer::RenderStageResult()> &fOnComplete);
		void StartTextureBaking(RenderWorker &worker);
		virtual void PrepareCyclesSceneForRendering() override;
		virtual bool UpdateStereoEye(pragma::scenekit::RenderWorker &worker, pragma::scenekit::Renderer::ImageRenderStage stage, StereoEye &eyeStage) override;
		virtual void CloseRenderScene() override;
		virtual void FinalizeImage(image::ImageBuffer &imgBuf, StereoEye eyeStage) override;
		void InitStereoEye(StereoEye eyeStage);
		void ReloadProgressiveRender(bool clearExposure = true, bool waitForPreviousCompletion = false);
		int GetTileSize() const;

		void SetupRenderSettings(ccl::Scene &scene, ccl::Session &session, ccl::BufferParams &bufferParams, pragma::scenekit::Scene::RenderMode renderMode, uint32_t maxTransparencyBounces) const;

		void AddDebugSky();
		ccl::Mesh *AddDebugMesh();
		ccl::Object *AddDebugObject();
		ccl::Shader *AddDebugShader();
		void AddDebugLight();
		void InitializeDebugScene(const std::string &fileName, const std::vector<std::string> &xmlFileNames);
		void PopulateDebugScene();

		ccl::SessionParams GetSessionParameters(const pragma::scenekit::Scene &scene, const ccl::DeviceInfo &devInfo) const;
		ccl::BufferParams GetBufferParameters() const;
		void SyncLight(pragma::scenekit::Scene &scene, const pragma::scenekit::Light &light, bool update = false);
		void SyncCamera(const pragma::scenekit::Camera &cam, bool update = false);
		void SyncObject(const pragma::scenekit::Object &obj);
		void SyncMesh(const pragma::scenekit::Mesh &mesh);
		void InitializeSession(pragma::scenekit::Scene &scene, const ccl::DeviceInfo &devInfo);
		std::optional<ccl::DeviceInfo> InitializeDevice(const pragma::scenekit::Scene &scene, std::string &outErr);
		bool Initialize(pragma::scenekit::Scene &scene, std::string &outErr);
		pragma::scenekit::Scene::DeviceType m_deviceType = pragma::scenekit::Scene::DeviceType::CPU;
		std::unique_ptr<ccl::Session> m_cclSession = nullptr;
		DisplayDriver *m_displayDriver = nullptr;
		OutputDriver *m_outputDriver = nullptr;
		ccl::Scene *m_cclScene = nullptr;
		std::vector<std::shared_ptr<CCLShader>> m_cclShaders = {};
		std::unordered_map<const GroupNodeDesc *, size_t> m_shaderCache {};
		struct CclObjectInfo {
			ccl::Object *object = nullptr;
			pragma::math::ScaledTransform lastUpdatePose {};
		};
		std::unordered_map<const Object *, CclObjectInfo> m_objectToCclObject;
		std::unordered_map<std::string, const Object *> m_uuidToObject;
		std::unordered_map<const Mesh *, ccl::Mesh *> m_meshToCcclMesh;
		std::unordered_map<ccl::Mesh *, const Mesh *> m_cclMeshToMesh;
		std::unordered_map<const Light *, ccl::Light *> m_lightToCclLight;
		std::unordered_map<const pragma::scenekit::Light *, std::shared_ptr<CCLShader>> m_lightToShader;
		std::atomic<uint32_t> m_restartState = 0;
		StateFlags m_stateFlags = StateFlags::None;
		std::mutex m_cancelMutex;
		bool m_cancelled = false;

		std::unique_ptr<pragma::util::baking::BakeDataView> m_bakeData = nullptr;
		std::vector<pragma::util::baking::BakePixel> m_bakePixels;

		ccl::SessionParams m_sessionParams;
		ccl::BufferParams m_bufferParams;

		std::string m_curStatus;
		std::string m_curSubStatus;

		Scene::RenderMode m_renderMode = Scene::RenderMode::RenderImage;
	};
	using namespace pragma::math::scoped_enum::bitwise;
};
export {
	REGISTER_ENUM_FLAGS(pragma::scenekit::cycles::Renderer::StateFlags)
}
