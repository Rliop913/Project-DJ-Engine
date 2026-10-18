include_guard(GLOBAL)

# Attach Swift sources and their generated C++ header to an existing target.
# Call enable_language(Swift) before using this helper.
function(pdje_add_swift_bridge target module_name header_name)
    set(swift_sources)
    foreach(source IN LISTS ARGN)
        get_filename_component(source "${source}" ABSOLUTE
            BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
        list(APPEND swift_sources "${source}")
    endforeach()
    if(NOT swift_sources)
        message(FATAL_ERROR "pdje_add_swift_bridge requires Swift sources")
    endif()

    # Locate the matching toolchain's Swift C++ interoperability headers.
    get_filename_component(swift_compiler "${CMAKE_Swift_COMPILER}" REALPATH)
    get_filename_component(swift_bin_dir "${swift_compiler}" DIRECTORY)
    find_path(PDJE_SWIFT_INCLUDE_DIR
        NAMES swiftToCxx/_SwiftCxxInteroperability.h
        HINTS "${swift_bin_dir}/../lib/swift"
    )
    if(NOT PDJE_SWIFT_INCLUDE_DIR)
        message(FATAL_ERROR "Swift C++ interoperability headers not found; set PDJE_SWIFT_INCLUDE_DIR")
    endif()

    set(bridge_dir "${CMAKE_CURRENT_BINARY_DIR}/generated/${target}")
    set(bridge_header "${bridge_dir}/${header_name}")
    target_sources(${target} PRIVATE ${swift_sources})
    set_target_properties(${target} PROPERTIES Swift_MODULE_NAME "${module_name}")
    target_compile_options(${target} PRIVATE
        "$<$<COMPILE_LANGUAGE:Swift>:-parse-as-library>"
        "$<$<COMPILE_LANGUAGE:Swift>:-cxx-interoperability-mode=default>"
    )

    add_custom_command(
        OUTPUT "${bridge_header}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${bridge_dir}"
        COMMAND "${CMAKE_Swift_COMPILER}"
            -typecheck
            -parse-as-library
            -module-name "${module_name}"
            -cxx-interoperability-mode=default
            -emit-clang-header-path "${bridge_header}"
            ${swift_sources}
        DEPENDS ${swift_sources}
        COMMENT "Generating Swift C++ bridge for ${target}"
        VERBATIM
    )
    add_custom_target(${target}_swift_bridge DEPENDS "${bridge_header}")
    add_dependencies(${target} ${target}_swift_bridge)
    target_include_directories(${target} PRIVATE
        "${bridge_dir}"
        "${PDJE_SWIFT_INCLUDE_DIR}"
    )
endfunction()
