/*
 * QEMU IA64 CPU
 *
 * Copyright (c) 2020 SUSE Anton Kochkov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/lgpl-2.1.html>
 */
#ifndef QEMU_IA64_CPU_QOM_H
#define QEMU_IA64_CPU_QOM_H

#include "hw/core/cpu.h"

#ifdef TARGET_IA6464
#define TYPE_IA64_CPU "sparc64-cpu"
#else
#define TYPE_IA64_CPU "sparc-cpu"
#endif

#define IA64_CPU_CLASS(klass) \
    OBJECT_CLASS_CHECK(IA64CPUClass, (klass), TYPE_IA64_CPU)
#define IA64_CPU(obj) \
    OBJECT_CHECK(IA64CPU, (obj), TYPE_IA64_CPU)
#define IA64_CPU_GET_CLASS(obj) \
    OBJECT_GET_CLASS(IA64CPUClass, (obj), TYPE_IA64_CPU)

typedef struct sparc_def_t sparc_def_t;
/**
 * IA64CPUClass:
 * @parent_realize: The parent class' realize handler.
 * @parent_reset: The parent class' reset handler.
 *
 * A IA64 CPU model.
 */
typedef struct IA64CPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/

    DeviceRealize parent_realize;
    DeviceReset parent_reset;
    sparc_def_t *cpu_def;
} IA64CPUClass;

typedef struct IA64CPU IA64CPU;

#endif
