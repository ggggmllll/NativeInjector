package com.gumuluo.injector;

public interface InjectorImpl {
    /**/

    void init(int pid, int arch, int mode);

    String dumpStack();

    int injectLib(String path);

    int pltHook(String path, String old_func_name, String new_func_name);

    int inlineHook(long addr, String code, int type);

    //int singleInstHook(long addr, String code, int type);

    int trapHook(long addr, int[] action, int type);

    //int noTraceHook(long addr, int[] action);
}
