set(PCK "cycles")

if (${PCK}_FOUND)
  return()
endif()

find_path(${PCK}_INCLUDE_DIR
  NAMES scene/scene.h
  HINTS
    ${PRAGMA_DEPS_DIR}/cycles/include
)

set(cycles_libs
	cycles_bvh
  cycles_device
  cycles_graph
  cycles_integrator
  cycles_kernel_cpu
  cycles_scene
  cycles_session
  cycles_subd
  cycles_util
  extern_cuew
  extern_hipew
  extern_sky
)

if(UNIX)
  set(cycles_libs ${cycles_libs} extern_libc_compat)
endif()

set(cycles_lib_list)
foreach(LIB IN LISTS cycles_libs)
  find_library(cycles_${LIB}
    NAMES ${LIB}
    HINTS ${PRAGMA_DEPS_DIR}/cycles/lib
  )
  list(APPEND cycles_lib_list cycles_${LIB})
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${PCK}
  REQUIRED_VARS ${cycles_lib_list} ${PCK}_INCLUDE_DIR
)

set(cycles_lib_paths)
foreach(LIB IN LISTS cycles_lib_list)
  list(APPEND cycles_lib_paths ${${LIB}})
endforeach()

if (cycles_FOUND)
  set(${PCK}_LIBRARIES   ${cycles_lib_paths})
  set(${PCK}_INCLUDE_DIRS ${${PCK}_INCLUDE_DIR})
endif()
