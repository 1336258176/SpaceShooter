macro(CopyDLL target_name dll_name)
    if (WIN32)
        if (IS_ABSOLUTE "${dll_name}")
            set(dll_path "${dll_name}")
        else()
            set(dll_path "${SDL2_LIBDIR}/${dll_name}")
        endif()

        add_custom_command(
            TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${dll_path} $<TARGET_FILE_DIR:${target_name}>
        )
    endif()
endmacro(CopyDLL)