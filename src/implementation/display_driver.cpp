// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <util/half.h>

module pragma.scenekit.cycles;

import pragma.scenekit;

static void dump_image_file(const std::string &name, pragma::image::ImageBuffer &imgBuf)
{
	auto f = pragma::fs::open_file("temp/cycles_driver_output_" + name + ".hdr", pragma::fs::FileMode::Write | pragma::fs::FileMode::Binary);
	if(f) {
		pragma::fs::File fp {f};
		pragma::image::save_image(fp, imgBuf, pragma::image::ImageFormat::HDR, 1.f);
	}
}

pragma::scenekit::cycles::BaseDriver::BaseDriver(uint32_t width, uint32_t height) : m_width {width}, m_height {height} {}

////////////

pragma::scenekit::cycles::DisplayDriver::DisplayDriver(pragma::scenekit::TileManager &tileManager, uint32_t width, uint32_t height) : BaseDriver {width, height}, m_tileManager {tileManager}
{
	m_postProcessingThread = std::thread {[this]() {
		while(m_ppThreadRunning)
			RunPostProcessing();
	}};

	pragma::util::set_thread_name(m_postProcessingThread, "cycles_display_driver_pp");
}
pragma::scenekit::cycles::DisplayDriver::~DisplayDriver()
{
	m_ppThreadRunning = false;
	m_postProcessingCondition.notify_one();
	m_postProcessingThread.join();
}
void pragma::scenekit::cycles::DisplayDriver::UpdateTileResolution(uint32_t tileWidth, uint32_t tileHeight)
{
	if(tileWidth == m_tileWidth && tileHeight == m_tileHeight)
		return;
	m_tileWidth = tileWidth;
	m_tileHeight = tileHeight;
	m_numTilesX = m_width / m_tileWidth;
	if((m_width % m_tileWidth) > 0)
		++m_numTilesX;
	auto numTilesY = m_height / m_tileHeight;
	if((m_height % m_tileHeight) > 0)
		++numTilesY;
	auto tileCount = m_numTilesX * numTilesY;
	m_tmpImageBuffers.reserve(tileCount);
	m_mappedTileImageBuffers.reserve(tileCount);
	m_pendingForPpTileImageBuffers.reserve(tileCount);
	for(auto i = decltype(tileCount) {0u}; i < tileCount; ++i) {
		m_tmpImageBuffers.push_back(image::ImageBuffer::Create(static_cast<void *>(nullptr), tileWidth, tileHeight, image::Format::RGBA16));
		m_mappedTileImageBuffers.push_back(image::ImageBuffer::Create(tileWidth, tileHeight, image::Format::RGBA16));
		m_pendingForPpTileImageBuffers.push_back(image::ImageBuffer::Create(tileWidth, tileHeight, image::Format::RGBA16));
	}
}
uint32_t pragma::scenekit::cycles::DisplayDriver::GetTileIndex(uint32_t x, uint32_t y) const
{
	auto numTilesX = m_numTilesX;
	return (y / m_tileHeight) * numTilesX + (x / m_tileWidth);
}
bool pragma::scenekit::cycles::DisplayDriver::update_begin(const Params &params, int effectiveWidth, int effectiveHeight)
{
	auto inBounds = (params.full_offset.x >= 0 && params.full_offset.y >= 0 && params.full_offset.x + effectiveWidth <= m_width && params.full_offset.y + effectiveHeight <= m_height);
	assert(inBounds);
	if(!inBounds)
		return false;
	m_mappedSize = {effectiveWidth, effectiveHeight};
	m_mappedOffset = {params.full_offset.x, params.full_offset.y};
	m_mappedTileIndex = GetTileIndex(params.full_offset.x, params.full_offset.y);
	return true;
}
void pragma::scenekit::cycles::DisplayDriver::update_end()
{
	m_mappedSize = {0, 0};
	m_mappedOffset = {0, 0};
	m_mappedTileIndex = 0;
}
void pragma::scenekit::cycles::DisplayDriver::RunPostProcessing()
{
	struct TileData {
		TileInfo info;
		std::vector<uint8_t> data;
	};
	std::queue<TileData> tileInfos;
	{ // Note: This has to be scoped!
		std::unique_lock<std::mutex> lock {m_postProcessingMutex};
		m_postProcessingCondition.wait(lock, [this]() -> bool { return !m_imageBufferReadyForPp.empty() || !m_ppThreadRunning; });
		if(!m_ppThreadRunning)
			return;
		while(!m_imageBufferReadyForPp.empty()) {
			auto tileInfo = m_imageBufferReadyForPp.front();
			m_imageBufferReadyForPp.pop();

			auto &src = *m_pendingForPpTileImageBuffers[tileInfo.tileIndex];
			TileData tileData {};
			tileData.info = tileInfo;
			tileData.data.resize(tileInfo.effectiveSize.x * tileInfo.effectiveSize.y * src.GetPixelSize());
			memcpy(tileData.data.data(), src.GetData(), pragma::util::size_of_container(tileData.data));
			tileInfos.push(std::move(tileData));
		}
	}

	while(!tileInfos.empty()) {
		auto &tileData = tileInfos.front();
		auto &tileInfo = tileData.info;

		auto &inputTile = m_tileManager.GetInputTiles()[tileInfo.tileIndex];
		inputTile.x = tileInfo.tileOffset.x;
		inputTile.y = tileInfo.tileOffset.y;
		inputTile.w = tileInfo.effectiveSize.x;
		inputTile.h = tileInfo.effectiveSize.y;
		inputTile.index = tileInfo.tileIndex;
		inputTile.sample = 1; // Not accurate, but doesn't matter here
		inputTile.flags |= pragma::scenekit::TileManager::TileData::Flags::HDRData;
		inputTile.data = std::move(tileData.data);

		auto &imgBuf = m_tmpImageBuffers[tileInfo.tileIndex];
		imgBuf->Reset(inputTile.data.data(), inputTile.w, inputTile.h, imgBuf->GetFormat());
		imgBuf->Flip(true, true);

		// This is deprecated; Denoising is now handled through Cycles
		static auto denoise = false;
		if(denoise) {
			denoise::Info denoiseInfo {};
			denoiseInfo.hdr = true;
			denoiseInfo.width = inputTile.w;
			denoiseInfo.height = inputTile.h;
			pragma::scenekit::denoise::ImageData output {};
			output.data = inputTile.data.data();
			output.format = imgBuf->GetFormat();
			pragma::scenekit::denoise::ImageInputs inputs {};
			inputs.beautyImage = output;
			denoise::denoise(denoiseInfo, inputs, output);
		}
		static auto enablePp = true;
		if(enablePp)
			m_tileManager.ApplyPostProcessingForProgressiveTile(inputTile);
		m_tileManager.AddRenderedTile(std::move(inputTile));

		tileInfos.pop();
	}

	m_tileWritten = true;
}
ccl::half4 *pragma::scenekit::cycles::DisplayDriver::map_texture_buffer()
{
	static_assert(sizeof(ccl::half4) == sizeof(uint16_t) * 4);
	return static_cast<ccl::half4 *>(m_mappedTileImageBuffers[m_mappedTileIndex]->GetData());
}
void pragma::scenekit::cycles::DisplayDriver::unmap_texture_buffer()
{
	// We want to keep the overhead here to a bare minimum, in order to avoid
	// stalling the Cycles renderer.
	// To do that, we'll postpone the post-processing to a separate thread, and let
	// Cycles continue with a different image buffer immediately.
	std::scoped_lock lock {m_postProcessingMutex};

	uint32_t tileIndex = m_mappedTileIndex;
	auto &mappedImageBuffer = m_mappedTileImageBuffers[tileIndex];

	/*static auto debugDumpAsFile = false;
	if(debugDumpAsFile) {
		debugDumpAsFile = false;
		dump_image_file("combined", *mappedImageBuffer);
	}*/

	auto &pendingForPpImageBuffer = m_pendingForPpTileImageBuffers[tileIndex];
	std::swap(mappedImageBuffer, pendingForPpImageBuffer);
	m_imageBufferReadyForPp.push({tileIndex, m_mappedOffset, m_mappedSize});
	m_postProcessingCondition.notify_one();
}
void pragma::scenekit::cycles::DisplayDriver::clear()
{
	// TODO: m_pendingForPpImageBuffer should be cleared as well?
	for(auto &imgBuf : m_mappedTileImageBuffers) {
		auto *data = imgBuf->GetData();
		memset(data, 0, imgBuf->GetSize());
	}
}
void pragma::scenekit::cycles::DisplayDriver::next_tile_begin() {}
void pragma::scenekit::cycles::DisplayDriver::draw(const Params &params) {}

////////////

pragma::scenekit::cycles::OutputDriver::OutputDriver(const std::vector<std::pair<PassType, image::Format>> &passes, uint32_t width, uint32_t height) : BaseDriver {width, height}, m_passes {passes}
{
	m_tileData.resize(width * height);
	Reset();
}
void pragma::scenekit::cycles::OutputDriver::Reset()
{
	m_imageBuffers.clear();
	m_imageBuffers.reserve(m_passes.size());
	for(auto &pair : m_passes) {
		auto imgBuf = image::ImageBuffer::Create(m_width, m_height, pair.second);
		m_imageBuffers[pair.first] = PassInfo {std::string {magic_enum::enum_name(pair.first)}, imgBuf};
	}
}
void pragma::scenekit::cycles::OutputDriver::DebugDumpImages()
{
	for(auto &pair : m_imageBuffers)
		dump_image_file(std::string {magic_enum::enum_name(pair.first)}, *pair.second.imageBuffer);
}
std::shared_ptr<pragma::image::ImageBuffer> pragma::scenekit::cycles::OutputDriver::GetImageBuffer(PassType pass) const
{
	auto it = m_imageBuffers.find(pass);
	return (it != m_imageBuffers.end()) ? it->second.imageBuffer : nullptr;
}
const std::unordered_map<pragma::scenekit::PassType, pragma::scenekit::cycles::OutputDriver::PassInfo> &pragma::scenekit::cycles::OutputDriver::GetImageBuffers() const { return m_imageBuffers; }
void pragma::scenekit::cycles::OutputDriver::write_render_tile(const Tile &tile)
{
	for(auto &pair : m_imageBuffers) {
		auto &info = pair.second;
		auto &imgBuf = info.imageBuffer;
		assert(imgBuf->IsFloatFormat());
		if(!tile.get_pass_pixels(info.passName, imgBuf->GetChannelCount(), reinterpret_cast<float *>(m_tileData.data())))
			continue;
		memcpy(imgBuf->GetData(), m_tileData.data(), pragma::util::size_of_container(m_tileData));
	}

	static auto debugDumpAsFile = false;
	if(debugDumpAsFile) {
		debugDumpAsFile = false;
		DebugDumpImages();
	}
}

bool pragma::scenekit::cycles::OutputDriver::update_render_tile(const Tile &tile)
{
	//write_render_tile(tile);
	return false;
}

static float intToFloat(int32_t i)
{
	union {
		int i;
		float f;
	} u;
	u.i = i;
	return u.f;
}
void pragma::scenekit::cycles::OutputDriver::SetBakeData(const pragma::util::baking::BakeDataView &bakeData) { m_bakeData = &bakeData; }
bool pragma::scenekit::cycles::OutputDriver::read_render_tile(const Tile &tile)
{
	if(!m_bakeData)
		return false;
	auto &bakeData = *m_bakeData;

	auto w = tile.size.x;
	auto h = tile.size.y;
	auto x = tile.offset.x;
	auto y = tile.offset.y;
	std::vector<float> primitiveData;
	std::vector<float> differentialData;
	primitiveData.resize(w * h * 4);
	differentialData.resize(w * h * 4);

	for(int ty = 0; ty < h; ty++) {
		size_t offset = ty * w;
		float *primitive = primitiveData.data() + 3 * offset;
		float *differential = differentialData.data() + 4 * offset;

		size_t bake_offset = (y + ty) * bakeData.bakeImageWidth + x;
		const auto *bake_pixel = bakeData.bakePixels + bake_offset;

		for(int tx = 0; tx < w; tx++) {
			if(bake_pixel->objectId != bakeData.objectId) {
				primitive[0] = intToFloat(-1);
				primitive[1] = intToFloat(-1);
				primitive[2] = intToFloat(-1);
			}
			else {
				primitive[0] = bake_pixel->uv[0];
				primitive[1] = bake_pixel->uv[1];
				primitive[2] = intToFloat(bake_pixel->primitiveId);

				differential[0] = bake_pixel->du_dx;
				differential[1] = bake_pixel->du_dy;
				differential[2] = bake_pixel->dv_dx;
				differential[3] = bake_pixel->dv_dy;
			}

			primitive += 3;
			differential += 4;
			bake_pixel++;
		}
	}

	auto imgPrimitive = image::ImageBuffer::Create(primitiveData.data(), w, h, image::Format::RGB32, true);
	auto imgDifferential = image::ImageBuffer::Create(differentialData.data(), w, h, image::Format::RGBA32, true);
	tile.set_pass_pixels("BakePrimitive", imgPrimitive->GetChannelCount(), reinterpret_cast<float *>(imgPrimitive->GetData()));
	tile.set_pass_pixels("BakeDifferential", imgDifferential->GetChannelCount(), reinterpret_cast<float *>(imgDifferential->GetData()));
	return true;
}
