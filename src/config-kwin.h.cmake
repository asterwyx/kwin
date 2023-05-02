#define KWIN_PLUGIN_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"

#cmakedefine01 KWIN_BUILD_DECORATIONS
#cmakedefine01 KWIN_BUILD_KCMS
#cmakedefine01 KWIN_BUILD_NOTIFICATIONS
#cmakedefine01 KWIN_BUILD_SCREENLOCKER
#cmakedefine01 KWIN_BUILD_TABBOX
#cmakedefine01 KWIN_BUILD_ACTIVITIES
#define KWIN_CONFIG "kwinrc"
#define KWIN_VERSION_STRING "${PROJECT_VERSION}"
#define XCB_VERSION_STRING "${XCB_VERSION}"
#define KWIN_KILLER_BIN "${CMAKE_INSTALL_FULL_LIBEXECDIR}/kwin_killer_helper"
#cmakedefine01 HAVE_X11_XCB
#cmakedefine01 HAVE_X11_XINPUT
#cmakedefine01 HAVE_GBM_BO_GET_FD_FOR_PLANE
#cmakedefine01 HAVE_GBM_BO_CREATE_WITH_MODIFIERS2
#cmakedefine01 HAVE_MEMFD
#cmakedefine01 HAVE_BREEZE_DECO
#cmakedefine01 HAVE_SCHED_RESET_ON_FORK
#cmakedefine01 HAVE_ACCESSIBILITY
#cmakedefine01 HAVE_XKBCOMMON_NO_SECURE_GETENV
#if HAVE_BREEZE_DECO
#define BREEZE_KDECORATION_PLUGIN_ID "${BREEZE_KDECORATION_PLUGIN_ID}"
#endif

#cmakedefine01 PipeWire_FOUND

#cmakedefine01 HAVE_XWAYLAND_LISTENFD
