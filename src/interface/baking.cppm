/*
* Copyright 2011-2013 Blender Foundation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

module;

#include "definitions.hpp"
#include "scene/bake.h"
#include <util_image_buffer.hpp>
#include <util/math.h>
#include <cinttypes>
#include <vector>

export module pragma.scenekit.cycles:baking;

import pragma.scenekit;

export namespace pragma::scenekit {
	namespace cycles {
		class Renderer;
	};
	namespace baking {
		void prepare_bake_data(const cycles::Renderer &renderer, Object &o, util::baking::BakePixel *pixelArray, uint32_t numPixels, uint32_t imgWidth, uint32_t imgHeight, bool useLightmapUvs = false);
	};
}
