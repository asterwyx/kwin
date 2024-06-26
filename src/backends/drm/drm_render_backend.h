/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2022 Xaver Hugl <xaver.hugl@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "drm_plane.h"

#include <memory>

namespace KWin
{

class DrmPipelineLayer;
class DrmVirtualOutput;
class DrmPipeline;
class DrmOutputLayer;
class DrmOverlayLayer;

class DrmRenderBackend
{
public:
    virtual ~DrmRenderBackend() = default;

    virtual std::shared_ptr<DrmPipelineLayer> createDrmPlaneLayer(DrmPipeline *pipeline, DrmPlane::TypeIndex type) = 0;
    virtual std::shared_ptr<DrmOutputLayer> createLayer(DrmVirtualOutput *output) = 0;
};

}
