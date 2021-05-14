/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

extern bool __stdio_is_initialized;
#ifdef DEBUG
void __assertion_failed(const char* msg)
{
    dbgln("USERSPACE({}) ASSERTION FAILED: {}", getpid(), msg);
    if (__stdio_is_initialized)
        warnln("ASSERTION FAILED: {}", msg);

    Syscall::SC_set_coredump_metadata_params params {
        { "assertion", strlen("assertion") },
        { msg, strlen(msg) },
    };
    syscall(SC_set_coredump_metadata, &params);

#ifdef ASSERT_IS_EXIT
    //This way, the fuzzer doesn't interpret assertion failures as a crash.
    if (__stdio_is_initialized)
        warnln("Debeaked assert().");
    exit(0);
#else
    abort();
#endif

}
#endif
}

void _abort()
{

#ifdef ASSERT_IS_EXIT
    //This way, the fuzzer doesn't interpret assertion failures as a crash.
    if (__stdio_is_initialized)
        warnln("Debeaked abort().");
    exit(0);
#else

    asm volatile("ud2");
    __builtin_unreachable();
#endif
}
