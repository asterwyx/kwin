kcoreaddons_add_plugin(screencast INSTALL_NAMESPACE "kwin/plugins")
target_sources(screencast PRIVATE
    main.cpp
    outputscreencastsource.cpp
    pipewirecore.cpp
    regionscreencastsource.cpp
    screencastbuffer.cpp
    screencastmanager.cpp
    screencastsource.cpp
    screencaststream.cpp
    windowscreencastsource.cpp
)

ecm_qt_declare_logging_category(screencast
    HEADER kwinscreencast_logging.h
    IDENTIFIER KWIN_SCREENCAST
    CATEGORY_NAME kwin_screencast
    DEFAULT_SEVERITY Warning
)

target_link_libraries(screencast kwin KF6::I18n PkgConfig::PipeWire Libdrm::Libdrm)
