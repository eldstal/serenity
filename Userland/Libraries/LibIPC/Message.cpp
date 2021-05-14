/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibIPC/Message.h>

namespace IPC {

Message::Message()
{
}

Message::~Message()
{
}

// Hacks to get clang build to succeed.
// Message can't be abstract.

u32 Message::endpoint_magic() const {
  return 0xdeadbeef;
}

int Message::message_id() const {
  return 0xdeadbeef;
}

const char* Message::message_name() const {
  return "Not a real message type. Who knows?";
}

bool Message::valid() const {
  return false;
}

MessageBuffer Message::encode() const {
  return MessageBuffer();
}

}
