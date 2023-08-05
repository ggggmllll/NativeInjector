package com.gumuluo.injector;

public class ConfigBuilder {

    private static final ConfigBuilder builder = new ConfigBuilder();
    public static final int ARCH_ARM32 = 0;
    public static final int ARCH_ARM64 = 1;
    public static final int MODE_PLT_HOOK = 0;
    public static final int MODE_INLINE_HOOK_UNIQUE = 1;
    public static final int MODE_INLINE_HOOK_SHARED = 2;
    public static final int MODE_SINGLE_INST_HOOK = 3;
    public static final int MODE_PTRACE_TRAP_HOOK = 4;
    public static final int MODE_NO_TRACE_HOOK = 5;
    public static final int MODE_SO_INJECT = 6;
    private int arch = ARCH_ARM64;
    private int mode = MODE_SO_INJECT;
    private LibLoader loader = null;
    private int pid = -1;

    public ConfigBuilder setArch(int arch) {
        this.arch = arch;
        return this;
    }

    public ConfigBuilder setMode(int mode) {
        this.mode = mode;
        return this;
    }

    public ConfigBuilder setPid(int pid) {
        this.pid = pid;
        return this;
    }

    public BasicConfig build(BasicConfig config) {
        config.setArch(arch);
        config.setMode(mode);
        config.setPid(pid);
        config.setLoader(loader);
        return config;
    }

//    public BasicConfig build(int type) {
//
//    }

    public BasicConfig build() {
        return build(new BasicConfig());
    }

    public void setLoader(LibLoader loader) {
        this.loader = loader;
    }

    public static ConfigBuilder getBuilder() {
        return builder;
    }
}
