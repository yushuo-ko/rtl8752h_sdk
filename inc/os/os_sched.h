/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _OS_SCHED_H_
#define _OS_SCHED_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  OS          OSIF APIs
 * \defgroup    Schedule    Kernel Scheduler
 *
 * \brief       Manage the kernel scheduler functions.
 *
 * \ingroup     OS
 */


/**
 * os_sched.h
 *
 * \brief       Delay current task for a given period in milliseconds.
 *
 * \param[in]   ms  The amout of timer in milliseconds that the current task
 *                  should block.
 *
 * \return      None.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *          os_delay(100);
 *
 *          // Do something here per 100ms.
 *     }
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
void os_delay(uint32_t ms);

/**
 * os_sched.h
 *
 * \brief   Get the time in milliseconds since os_sched_start() API function was called.
 *
 * \param   None
 *
 * \return  The time in milliseconds. Note the time represented by a 32-bit integer
 *          may be overflowed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     uint32_t last_time;
 *
 *     // Get the last timestamp.
 *     last_time = os_sys_time_get();
 *
 *     for (;;)
 *     {
 *          // Wait for the next cycle.
 *          os_delay(100);
 *
 *          // Do something here per 100ms.
 *     }
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
uint64_t os_sys_time_get(void);

/**
 * os_sched.h
 *
 * \brief   Get the system tick since os_sched_start() API function was called.
 *
 * \param   None
 *
 * \return  The time in system tick. Note the tick represented by a 32-bit integer
 *          may be overflowed.
 *
 * <b>Example usage</b>
 * \code{.c}

* #define portTICK_PERIOD_MS   10

 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     uint32_t beg_tick;
 *     uint32_t end_tick;
 *     uint32_t time;
 *
 *     beg_tick = os_sys_tick_get();
 *
 *          // Do something here
 *
 *     end_tick = os_sys_tick_get();
 *
 *     //get correct passed time in milliseconds
 *     time = (end_tick - beg_tick) * portTICK_PERIOD_MS;
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
uint64_t os_sys_tick_get(void);

/**
 * os_sched.h
 *
 * \brief   Start the RTOS kernel scheduler.
 *
 * \param   None
 *
 * \return  The status of starting kernel scheduler.
 * \retval  true      Scheduler was started successfully.
 * \retval  false     Scheduler was failed to start.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     // Create at least one task before starting kernel scheduler.
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Start the kernel scheduler.
 *     os_sched_start();
 *
 *     // Will not get here unless a task calls os_sched_stop().
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
bool os_sched_start(void);

/**
 * os_sched.h
 *
 * \brief   Stop the RTOS kernel scheduler. All created tasks will be automatically
 *          deleted and multitasking (either preemptive or cooperative) stops.
 *
 * \param   None
 *
 * \return  The status of stopping kernel scheduler.
 * \retval  true      Scheduler was stopped successfully.
 * \retval  false     Scheduler was failed to stop.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *
 *          // At some point we want to end the real time kernel processing.
 *          os_sched_stop();
 *     }
 * }
 *
 * int test(void)
 * {
 *     // Create at least one task before starting kernel scheduler.
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Start the kernel scheduler.
 *     os_sched_start();
 *
 *     // Will not get here unless a task calls os_sched_stop().
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
bool os_sched_stop(void);

/**
 * os_sched.h
 *
 * \brief   Suspends the kernel scheduler without disabling interrupts. Context
 *          switches will not occur while the scheduler is suspended. After calling
 *          os_sched_suspend(), the calling task will continue to execute without
 *          risk of being swapped out until a call to os_sched_resume() has been made.
 *
 * \param   None
 *
 * \return  The status of suspending kernel scheduler.
 * \retval  true      Scheduler was suspended successfully.
 * \retval  false     Scheduler was failed to suspend.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *
 *          // At some point the task wants to perform a long operation, and do not
 *          // want to get swapped out.
 *          os_sched_suspend();
 *
 *          // The long operation.
 *
 *          // The operation is completed, and resume the scheduler.
 *          os_sched_resume();
 *     }
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
bool os_sched_suspend(void);

/**
 * os_sched.h
 *
 * \brief   Resume the kernel scheduler after it was suspended by a call
 *          to os_sched_suspend().
 *
 * \param   None
 *
 * \return  The status of resuming kernel scheduler.
 * \retval  true      Scheduler was resumed successfully.
 * \retval  false     Scheduler was failed to resume.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *
 *          // At some point the task wants to perform a long operation, and do not
 *          // want to get swapped out.
 *          os_sched_suspend();
 *
 *          // The long operation.
 *
 *          // The operation is completed, and resume the scheduler.
 *          os_sched_resume();
 *     }
 * }
 * \endcode
 *
 * \ingroup  Schedule
 */
bool os_sched_resume(void);

/* internal function */
bool os_sched_is_start(void);

typedef enum
{
    SCHEDULER_SUSPENDED = 0,
    SCHEDULER_NOT_STARTED = 1,
    SCHEDULER_RUNNING = 2
} SCHEDULER_STATE;

SCHEDULER_STATE os_sched_state_get(void);

void os_systick_handler(void);

uint64_t os_sys_tick_increase(uint32_t tick_increment);

void os_vector_table_update(void);

void os_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _OS_SCHED_H_ */
