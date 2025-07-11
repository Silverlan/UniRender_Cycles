include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

set(UNIRENDER_CYCLES_ENABLE_DEPENDENCIES 1 CACHE BOOL "Enable project dependencies?")
set(UNIRENDER_CYCLES_WITH_CUDA 1 CACHE BOOL "Enable CUDA support?")
set(UNIRENDER_CYCLES_WITH_OPTIX 0 CACHE BOOL "Enable OPTIX support?")

set(DEPENDENCY_CYCLES_LIBRARY_LOCATION "" CACHE FILEPATH "Path to cycles libraries.")
set(DEPENDENCY_CYCLES_DEPENDENCIES_LOCATION "" CACHE FILEPATH "Path to cycles dependencies.")

option(ROLLBACK_CYCLES_4_1_1 "Enable Rollback to Cycles 4.1.1" OFF)

set(PROJ_NAME UniRender_cycles)
pr_add_library(${PROJ_NAME} SHARED)

pr_add_dependency(${PROJ_NAME} util_raytracing TARGET PUBLIC)
pr_add_dependency(${PROJ_NAME} spdlog TARGET PUBLIC)

pr_init_module(${PROJ_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

pr_add_third_party_dependency(${PROJ_NAME} openimageio)
pr_add_third_party_dependency(${PROJ_NAME} openexr)
pr_add_third_party_dependency(${PROJ_NAME} jpeg)
pr_add_third_party_dependency(${PROJ_NAME} tiff)
pr_add_third_party_dependency(${PROJ_NAME} png)
pr_add_third_party_dependency(${PROJ_NAME} zstd)
pr_add_third_party_dependency(${PROJ_NAME} llvm)
pr_add_third_party_dependency(${PROJ_NAME} embree)
pr_add_third_party_dependency(${PROJ_NAME} openvdb)
pr_add_third_party_dependency(${PROJ_NAME} opencolorio)
pr_add_third_party_dependency(${PROJ_NAME} alembic)
pr_add_third_party_dependency(${PROJ_NAME} openimagedenoise)
pr_add_third_party_dependency(${PROJ_NAME} cycles)
pr_add_third_party_dependency(${PROJ_NAME} tbb)
pr_add_third_party_dependency(${PROJ_NAME} imath)

if(WIN32)
	pr_add_third_party_dependency(${PROJ_NAME} gflags)
	pr_add_third_party_dependency(${PROJ_NAME} glog)

	# Only needed for testing code
	# pr_add_third_party_dependency(${PROJ_NAME} pugixml)
endif()

# These have to match the definitions in the cycles project!
pr_add_compile_definitions(${PROJ_NAME}
	-DWITH_HIP
	-DWITH_HIP_DYNLOAD
	-DWITH_EMBREE
	-DEMBREE_MAJOR_VERSION=4
	-DWITH_OPENIMAGEDENOISE
	-DWITH_ALEMBIC
	-DWITH_OPENVDB
	-DWITH_NANOVDB
	-DWITH_OPENSUBDIV
	-DWITH_OCIO
	-DWITH_SYSTEM_PUGIXML
	PUBLIC
)
if(UNIRENDER_CYCLES_WITH_CUDA)
	pr_add_compile_definitions(${PROJ_NAME}
		-DWITH_CUDA
		-DWITH_CUDA_DYNLOAD
	)
endif()
if(UNIRENDER_CYCLES_WITH_OPTIX)
	pr_add_compile_definitions(${PROJ_NAME}
		-DWITH_OPTIX
	)
endif()
if(WIN32)
	pr_add_compile_definitions(${PROJ_NAME}
		-DWITH_KERNEL_SSE42
		-DWITH_KERNEL_AVX2
	)
endif()
pr_add_compile_definitions(${PROJ_NAME} -DCCL_NAMESPACE_BEGIN=namespace\ ccl\ { PUBLIC)
pr_add_compile_definitions(${PROJ_NAME} -DCCL_NAMESPACE_END=} PUBLIC)
if(ROLLBACK_CYCLES_4_1_1)
	pr_add_compile_definitions(${PROJ_NAME} -DCYCLES_ROLLBACK_411)
endif()

if(WIN32)
	pr_add_compile_definitions(${PROJ_NAME} -DWITH_CYCLES_LOGGING)
endif()
pr_add_compile_definitions(${PROJ_NAME} -DGLOG_NO_ABBREVIATED_SEVERITIES -DGOOGLE_GLOG_DLL_DECL=__declspec\(dllexport\))

if(WIN32)
	target_link_libraries(${PROJ_NAME} PRIVATE
		opengl32
		glu32
		version
		shlwapi
	)
endif()

pr_finalize(${PROJ_NAME})

