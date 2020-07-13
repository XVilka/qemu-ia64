static void ia64_cpu_class_init(ObjectClass *oc, void *data)
{
    IA64CPUClass *scc = IA64_CPU_CLASS(oc);
    CPUClass *cc = CPU_CLASS(oc);
    DeviceClass *dc = DEVICE_CLASS(oc);

    device_class_set_parent_realize(dc, ia64_cpu_realizefn,
                                    &scc->parent_realize);
    device_class_set_props(dc, ia64_cpu_properties);

    device_class_set_parent_reset(dc, ia64_cpu_reset, &scc->parent_reset);

    cc->class_by_name = ia64_cpu_class_by_name;
    cc->parse_features = ia64_cpu_parse_features;
    cc->has_work = ia64_cpu_has_work;
    cc->do_interrupt = ia64_cpu_do_interrupt;
    cc->cpu_exec_interrupt = ia64_cpu_exec_interrupt;
    cc->dump_state = ia64_cpu_dump_state;
    cc->set_pc = ia64_cpu_set_pc;
    cc->synchronize_from_tb = ia64_cpu_synchronize_from_tb;
    cc->gdb_read_register = ia64_cpu_gdb_read_register;
    cc->gdb_write_register = ia64_cpu_gdb_write_register;
    cc->tlb_fill = ia64_cpu_tlb_fill;
#ifndef CONFIG_USER_ONLY
    cc->do_transaction_failed = ia64_cpu_do_transaction_failed;
    cc->do_unaligned_access = ia64_cpu_do_unaligned_access;
    cc->get_phys_page_debug = ia64_cpu_get_phys_page_debug;
    cc->vmsd = &vmstate_ia64_cpu;
#endif
    cc->disas_set_info = cpu_ia64_disas_set_info;
    cc->tcg_initialize = ia64_tcg_init;

    cc->gdb_num_core_regs = 125;
}

static const TypeInfo ia64_cpu_type_info = {
    .name = TYPE_IA64_CPU,
    .parent = TYPE_CPU,
    .instance_size = sizeof(IA64CPU),
    .instance_init = ia64_cpu_initfn,
    .abstract = true,
    .class_size = sizeof(IA64CPUClass),
    .class_init = ia64_cpu_class_init,
};

static void ia64_cpu_cpudef_class_init(ObjectClass *oc, void *data)
{
    IA64CPUClass *scc = IA64_CPU_CLASS(oc);
    scc->cpu_def = data;
}

static void ia64_register_cpudef_type(const struct ia64_def_t *def)
{
    char *typename = ia64_cpu_type_name(def->name);
    TypeInfo ti = {
        .name = typename,
        .parent = TYPE_IA64_CPU,
        .class_init = ia64_cpu_cpudef_class_init,
        .class_data = (void *)def,
    };

    type_register(&ti);
    g_free(typename);
}

static void ia64_cpu_register_types(void)
{
    int i;

    type_register_static(&ia64_cpu_type_info);
    for (i = 0; i < ARRAY_SIZE(ia64_defs); i++) {
        ia64_register_cpudef_type(&ia64_defs[i]);
    }
}

type_init(ia64_cpu_register_types)
