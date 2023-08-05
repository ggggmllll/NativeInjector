package com.gumuluo.injector;

public class BasicConfig {

    private int arch;
    private int mode;
    private int pid;
    private LibLoader loader;

    public BasicConfig() {
    }

    public int getArch() {
        return arch;
    }

    public void setArch(int arch) {
        this.arch = arch;
    }

    public int getMode() {
        return mode;
    }

    public void setMode(int mode) {
        this.mode = mode;
    }

    public int getPid() {
        return pid;
    }

    public void setPid(int pid) {
        this.pid = pid;
    }

    public LibLoader getLoader() {
        return loader;
    }

    public void setLoader(LibLoader loader) {
        this.loader = loader;
    }
}
