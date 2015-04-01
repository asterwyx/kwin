/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2013 Martin Gräßlin <mgraesslin@kde.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
#ifndef KWIN_SCENE_QPAINTER_H
#define KWIN_SCENE_QPAINTER_H

#include "scene.h"
#include "shadow.h"

#include "decorations/decorationrenderer.h"
#if HAVE_WAYLAND
namespace KWayland
{
namespace Client
{
class Buffer;
}
}
#endif

namespace KWin {

namespace Xcb {
    class Shm;
}

namespace Wayland
{
class WaylandBackend;
}
class FramebufferBackend;
class X11WindowedBackend;

class QPainterBackend
{
public:
    virtual ~QPainterBackend();
    virtual void present(int mask, const QRegion &damage) = 0;

    /**
     * @brief Returns the OverlayWindow used by the backend.
     *
     * A backend does not have to use an OverlayWindow, this is mostly for the X world.
     * In case the backend does not use an OverlayWindow it is allowed to return @c null.
     * It's the task of the caller to check whether it is @c null.
     *
     * @return :OverlayWindow*
     **/
    virtual OverlayWindow *overlayWindow();
    virtual bool usesOverlayWindow() const = 0;
    virtual void prepareRenderingFrame() = 0;
    /**
     * @brief Shows the Overlay Window
     *
     * Default implementation does nothing.
     */
    virtual void showOverlay();
    /**
     * @brief React on screen geometry changes.
     *
     * Default implementation does nothing. Override if specific functionality is required.
     *
     * @param size The new screen size
     */
    virtual void screenGeometryChanged(const QSize &size);
    /**
     * @brief Whether the creation of the Backend failed.
     *
     * The SceneQPainter should test whether the Backend got constructed correctly. If this method
     * returns @c true, the SceneQPainter should not try to start the rendering.
     *
     * @return bool @c true if the creation of the Backend failed, @c false otherwise.
     **/
    bool isFailed() const {
        return m_failed;
    }

    virtual QImage *buffer() = 0;
    virtual bool needsFullRepaint() const = 0;
    virtual void renderCursor(QPainter *painter);

protected:
    QPainterBackend();
    /**
     * @brief Sets the backend initialization to failed.
     *
     * This method should be called by the concrete subclass in case the initialization failed.
     * The given @p reason is logged as a warning.
     *
     * @param reason The reason why the initialization failed.
     **/
    void setFailed(const QString &reason);

private:
    bool m_failed;
};

#if HAVE_WAYLAND
class WaylandQPainterBackend : public QObject, public QPainterBackend
{
    Q_OBJECT
public:
    explicit WaylandQPainterBackend(Wayland::WaylandBackend *b);
    virtual ~WaylandQPainterBackend();

    virtual void present(int mask, const QRegion& damage) override;
    virtual bool usesOverlayWindow() const override;
    virtual void screenGeometryChanged(const QSize &size) override;
    virtual QImage *buffer() override;
    virtual void prepareRenderingFrame() override;
    virtual bool needsFullRepaint() const override;
private Q_SLOTS:
    void remapBuffer();
private:
    Wayland::WaylandBackend *m_backend;
    bool m_needsFullRepaint;
    QImage m_backBuffer;
    QWeakPointer<KWayland::Client::Buffer> m_buffer;
};

class X11WindowedQPainterBackend : public QObject, public QPainterBackend
{
    Q_OBJECT
public:
    X11WindowedQPainterBackend(X11WindowedBackend *backend);
    virtual ~X11WindowedQPainterBackend();

    QImage *buffer() override;
    bool needsFullRepaint() const override;
    bool usesOverlayWindow() const override;
    void prepareRenderingFrame() override;
    void present(int mask, const QRegion &damage) override;
    void screenGeometryChanged(const QSize &size);

private:
    bool m_needsFullRepaint = true;
    xcb_gcontext_t m_gc = XCB_NONE;
    QImage m_backBuffer;
    X11WindowedBackend *m_backend;
};

class FramebufferQPainterBackend : public QObject, public QPainterBackend
{
    Q_OBJECT
public:
    FramebufferQPainterBackend(FramebufferBackend *backend);
    virtual ~FramebufferQPainterBackend();

    QImage *buffer() override;
    bool needsFullRepaint() const override;
    bool usesOverlayWindow() const override;
    void prepareRenderingFrame() override;
    void present(int mask, const QRegion &damage) override;
    void renderCursor(QPainter *painter) override;

private:
    QImage m_renderBuffer;
    QImage m_backBuffer;
    FramebufferBackend *m_backend;
};
#endif

class SceneQPainter : public Scene
{
    Q_OBJECT

public:
    virtual ~SceneQPainter();
    virtual bool usesOverlayWindow() const override;
    virtual OverlayWindow* overlayWindow() override;
    virtual qint64 paint(QRegion damage, ToplevelList windows) override;
    virtual void paintGenericScreen(int mask, ScreenPaintData data) override;
    virtual CompositingType compositingType() const override;
    virtual bool initFailed() const override;
    virtual EffectFrame *createEffectFrame(EffectFrameImpl *frame) override;
    virtual Shadow *createShadow(Toplevel *toplevel) override;
    Decoration::Renderer *createDecorationRenderer(Decoration::DecoratedClientImpl *impl) override;
    void screenGeometryChanged(const QSize &size) override;

    QPainter *painter();

    static SceneQPainter *createScene(QObject *parent);

protected:
    virtual void paintBackground(QRegion region) override;
    virtual Scene::Window *createWindow(Toplevel *toplevel) override;

private:
    explicit SceneQPainter(QPainterBackend *backend, QObject *parent = nullptr);
    QScopedPointer<QPainterBackend> m_backend;
    QScopedPointer<QPainter> m_painter;
    class Window;
};

class SceneQPainter::Window : public Scene::Window
{
public:
    Window(SceneQPainter *scene, Toplevel *c);
    virtual ~Window();
    virtual void performPaint(int mask, QRegion region, WindowPaintData data) override;
protected:
    virtual WindowPixmap *createWindowPixmap() override;
private:
    void renderShadow(QPainter *painter);
    void renderWindowDecorations(QPainter *painter);
    SceneQPainter *m_scene;
};

class QPainterWindowPixmap : public WindowPixmap
{
public:
    explicit QPainterWindowPixmap(Scene::Window *window);
    virtual ~QPainterWindowPixmap();
    virtual void create() override;

    bool update(const QRegion &damage);
    const QImage &image();
private:
    QScopedPointer<Xcb::Shm> m_shm;
    QImage m_image;
};

class QPainterEffectFrame : public Scene::EffectFrame
{
public:
    QPainterEffectFrame(EffectFrameImpl *frame, SceneQPainter *scene);
    virtual ~QPainterEffectFrame();
    virtual void crossFadeIcon() override {}
    virtual void crossFadeText() override {}
    virtual void free() override {}
    virtual void freeIconFrame() override {}
    virtual void freeTextFrame() override {}
    virtual void freeSelection() override {}
    virtual void render(QRegion region, double opacity, double frameOpacity) override;
private:
    SceneQPainter *m_scene;
};

class SceneQPainterShadow : public Shadow
{
public:
    SceneQPainterShadow(Toplevel* toplevel);
    virtual ~SceneQPainterShadow();
    using Shadow::ShadowElements;
    using Shadow::ShadowElementTop;
    using Shadow::ShadowElementTopRight;
    using Shadow::ShadowElementRight;
    using Shadow::ShadowElementBottomRight;
    using Shadow::ShadowElementBottom;
    using Shadow::ShadowElementBottomLeft;
    using Shadow::ShadowElementLeft;
    using Shadow::ShadowElementTopLeft;
    using Shadow::ShadowElementsCount;
    using Shadow::shadowPixmap;
    using Shadow::topOffset;
    using Shadow::leftOffset;
    using Shadow::rightOffset;
    using Shadow::bottomOffset;
protected:
    virtual bool prepareBackend() override;
};

class SceneQPainterDecorationRenderer : public Decoration::Renderer
{
    Q_OBJECT
public:
    enum class DecorationPart : int {
        Left,
        Top,
        Right,
        Bottom,
        Count
    };
    explicit SceneQPainterDecorationRenderer(Decoration::DecoratedClientImpl *client);
    virtual ~SceneQPainterDecorationRenderer();

    void render() override;
    void reparent(Deleted *deleted) override;

    QImage image(DecorationPart part) const;

private:
    void resizeImages();
    QImage m_images[int(DecorationPart::Count)];
};

inline
bool SceneQPainter::usesOverlayWindow() const
{
    return m_backend->usesOverlayWindow();
}

inline
OverlayWindow* SceneQPainter::overlayWindow()
{
    return m_backend->overlayWindow();
}

inline
QPainter* SceneQPainter::painter()
{
    return m_painter.data();
}

inline
const QImage &QPainterWindowPixmap::image()
{
    return m_image;
}

} // KWin

#endif // KWIN_SCENEQPAINTER_H
