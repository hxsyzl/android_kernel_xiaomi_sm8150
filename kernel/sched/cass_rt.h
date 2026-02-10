// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 Sultan Alsawaf <sultan@kerneltoast.com>.
 */

#ifdef CONFIG_SCHED_CASS
int cass_select_task_rq_rt(struct task_struct *p, int prev_cpu, int sd_flag,
			   int wake_flags, int sibling_count_hint);

/* Use CASS. A dummy wrapper ensures the replaced function is still "used". */
static inline void *select_task_rq_rt_dummy(void)
{
	return (void *)select_task_rq_rt;
}
#define select_task_rq_rt cass_select_task_rq_rt
#endif /* CONFIG_SCHED_CASS */

#if defined(CONFIG_SCHED_CASS) && defined(CONFIG_SMP)
/*
 * sched_class::select_task_rq(p, task_cpu, sd_flag, flags)
 * vs
 * cass_select_task_rq_rt(p, prev_cpu, sd_flag, wake_flags, sibling_count_hint)
 *
 * We need an adapter because the function signatures differ.
 * The standard select_task_rq takes 4 arguments.
 * cass_select_task_rq_rt takes 5 arguments.
 */
static int select_task_rq_rt_adapter(struct task_struct *p, int task_cpu, int sd_flag, int flags)
{
    return cass_select_task_rq_rt(p, task_cpu, sd_flag, flags, 0);
}
#undef select_task_rq_rt
#define select_task_rq_rt select_task_rq_rt_adapter
#endif
