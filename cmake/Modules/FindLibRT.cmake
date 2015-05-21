# FindLibRT
# --------
# Defines:
# LIBRT_FOUND - true if librt has been found and can be used
# LIBRT_LIBRARIES - libraries to link against librt



find_library (LIBRT_LIBRARY NAMES rt)

set (LIBRT_LIBRARIES ${LIBRT_LIBRARY})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (LibRT
                                   REQUIRED_VARS LIBRT_LIBRARY)

mark_as_advanced(LIBRT_LIBRARY)
