/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>
    SPDX-FileCopyrightText: 2010 Sebastian Sauer <sebsauer@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <kcmodule.h>

#include "ui_zoom_config.h"

namespace KWin
{
class ZoomEffectConfig : public KCModule
{
    Q_OBJECT
public:
    explicit ZoomEffectConfig(QObject *parent, const KPluginMetaData &data);

public Q_SLOTS:
    void save() override;

private:
    Ui::ZoomEffectConfigForm m_ui;
};

} // namespace
