add_subdirectory(standalone)
if (X11_XCB_FOUND)
    add_subdirectory(windowed)
endif()
