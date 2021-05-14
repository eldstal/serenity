/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <AK/RefPtr.h>
#include <AK/Vector.h>
#include <unistd.h>

namespace IPC {

class AutoCloseFileDescriptor : public RefCounted<AutoCloseFileDescriptor> {
public:
    AutoCloseFileDescriptor(int fd)
        : m_fd(fd)
    {
    }

    ~AutoCloseFileDescriptor()
    {
        if (m_fd != -1)
            close(m_fd);
    }

    int value() const { return m_fd; }

private:
    int m_fd;
};

struct MessageBuffer {
    Vector<u8, 1024> data;
    Vector<RefPtr<AutoCloseFileDescriptor>> fds;
};

enum class ErrorCode : u32 {
    PeerDisconnected
};

class Message {
public:
    virtual ~Message();

    virtual u32 endpoint_magic() const;
    virtual int message_id() const;
    virtual const char* message_name() const;
    virtual bool valid() const;
    virtual MessageBuffer encode() const;

protected:
    Message();
};

}
