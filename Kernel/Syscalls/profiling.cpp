/*
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/CoreDump.h>
#include <Kernel/FileSystem/FileDescription.h>
#include <Kernel/FileSystem/VirtualFileSystem.h>
#include <Kernel/PerformanceManager.h>
#include <Kernel/Process.h>
#include <Kernel/Time/TimeManagement.h>

namespace Kernel {

bool g_profiling_all_threads;
PerformanceEventBuffer* g_global_perf_events;

KResultOr<int> Process::sys$profiling_enable(pid_t pid)
{
    REQUIRE_NO_PROMISES;

    if (pid == -1) {
        if (!is_superuser())
            return EPERM;
        ScopedCritical critical;
        if (g_global_perf_events)
            g_global_perf_events->clear();
        else
            g_global_perf_events = PerformanceEventBuffer::try_create_with_size(32 * MiB).leak_ptr();

        ScopedSpinLock lock(g_processes_lock);
        g_profiling_all_threads = true;
        Process::for_each([](auto& process) {
            PerformanceManager::add_process_created_event(process);
            return IterationDecision::Continue;
        });
        TimeManagement::the().enable_profile_timer();
        return 0;
    }

    ScopedSpinLock lock(g_processes_lock);
    auto process = Process::from_pid(pid);
    if (!process)
        return ESRCH;
    if (process->is_dead())
        return ESRCH;
    if (!is_superuser() && process->uid() != euid())
        return EPERM;
    if (!process->create_perf_events_buffer_if_needed())
        return ENOMEM;
    process->set_profiling(true);
    TimeManagement::the().enable_profile_timer();
    return 0;
}

KResultOr<int> Process::sys$profiling_disable(pid_t pid)
{
    REQUIRE_NO_PROMISES;

    if (pid == -1) {
        if (!is_superuser())
            return EPERM;
        ScopedCritical critical;
        g_profiling_all_threads = false;
        TimeManagement::the().disable_profile_timer();
        return 0;
    }

    ScopedSpinLock lock(g_processes_lock);
    auto process = Process::from_pid(pid);
    if (!process)
        return ESRCH;
    if (!is_superuser() && process->uid() != euid())
        return EPERM;
    if (!process->is_profiling())
        return EINVAL;
    TimeManagement::the().disable_profile_timer();
    process->set_profiling(false);
    return 0;
}

KResultOr<int> Process::sys$profiling_free_buffer(pid_t pid)
{
    REQUIRE_NO_PROMISES;

    if (pid == -1) {
        if (!is_superuser())
            return EPERM;

        OwnPtr<PerformanceEventBuffer> perf_events;

        {
            ScopedCritical critical;

            perf_events = g_global_perf_events;
            g_global_perf_events = nullptr;
        }

        return 0;
    }

    ScopedSpinLock lock(g_processes_lock);
    auto process = Process::from_pid(pid);
    if (!process)
        return ESRCH;
    if (!is_superuser() && process->uid() != euid())
        return EPERM;
    if (process->is_profiling())
        return EINVAL;
    process->delete_perf_events_buffer();
    return 0;
}
}
