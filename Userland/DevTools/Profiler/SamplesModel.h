/*
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGUI/Model.h>

namespace Profiler {

class Profile;

class SamplesModel final : public GUI::Model {
public:
    static NonnullRefPtr<SamplesModel> create(Profile& profile)
    {
        return adopt_ref(*new SamplesModel(profile));
    }

    enum Column {
        SampleIndex,
        Timestamp,
        ProcessID,
        ThreadID,
        ExecutableName,
        LostSamples,
        InnermostStackFrame,
        __Count
    };

    virtual ~SamplesModel() override;

    virtual int row_count(const GUI::ModelIndex& = GUI::ModelIndex()) const override;
    virtual int column_count(const GUI::ModelIndex& = GUI::ModelIndex()) const override;
    virtual String column_name(int) const override;
    virtual GUI::Variant data(const GUI::ModelIndex&, GUI::ModelRole) const override;
    virtual void update() override;

private:
    explicit SamplesModel(Profile&);

    Profile& m_profile;

    GUI::Icon m_user_frame_icon;
    GUI::Icon m_kernel_frame_icon;
};

}
