/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/JsonValue.h>
#include <AK/StringView.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto text = StringView(data, size);
    auto json = JsonValue::from_string(text);
    if (!json.has_value()) {
        warnln("Couldn't parse JSON");
        return 1;
    }
    return 0;
}
