/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
    SPDX-FileCopyrightText: 2021 Ismael Asensio <isma.af@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "rulebooksettingsbase.h"
#include <KSharedConfig>

namespace KWin
{
class Rules;
class RuleSettings;

class RuleBookSettings : public RuleBookSettingsBase
{
public:
    RuleBookSettings(KSharedConfig::Ptr config, QObject *parent = nullptr);
    RuleBookSettings(QObject *parent = nullptr);
    ~RuleBookSettings();

    QList<Rules *> rules() const;

    bool usrSave() override;
    void usrRead() override;
    bool usrIsSaveNeeded() const;

    int ruleCount() const;
    std::optional<int> indexForId(const QString &id) const;
    RuleSettings *ruleSettingsAt(int row) const;
    RuleSettings *insertRuleSettingsAt(int row);
    void removeRuleSettingsAt(int row);
    void moveRuleSettings(int srcRow, int destRow);

private:
    static QString generateGroupName();

private:
    QList<RuleSettings *> m_list;
    QStringList m_storedGroups;
};

}
