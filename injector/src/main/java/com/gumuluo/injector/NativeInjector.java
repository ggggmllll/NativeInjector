package com.gumuluo.injector;

import android.os.Process;
import android.util.Log;

import androidx.annotation.NonNull;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

public class NativeInjector {

    public static final int ERR_SUCCESS = 0;
    public static final int ERR_PERMISSION = 1;
    private static final int ERR_ABI = 2;
    private static final int ERR_IO = 4;
    private static final int ERR_XML = 8;
    private static final int ERR_EXEC = 16;
    private static final Map<String, Integer> abiMap;
    private static final String tag = "Injector";
    private static int err = ERR_SUCCESS;
    private static int arch;
    private static int pid;

    static {

        if (Process.myUid() != 0) {
            Log.e(tag, "permission denied");
            err |= ERR_PERMISSION;
        }

        abiMap = new HashMap<>();
        abiMap.put("armeabi", ConfigBuilder.ARCH_ARM32);
        abiMap.put("armeabi-v7a", ConfigBuilder.ARCH_ARM32);
        abiMap.put("arm64-v8a", ConfigBuilder.ARCH_ARM64);
    }

    @NonNull
    private static boolean isAppRunning(String name) {
        List<String> str = new ArrayList<>();

        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(Runtime.getRuntime().exec("/bin/ps -a").getInputStream()));
            String line;

            while ((line = reader.readLine()) != null) {
                str.add(line);
            }

        } catch (IOException e) {
            e.printStackTrace();
            err = ERR_EXEC;
        }

        if (str.contains(name))
            return false;

        for (String line: str) {
            if (line.contains(name)) {
                String[] contains = line.split(" ");
                pid = Integer.valueOf(contains[1]);
                break;
            }
        }

        return true;
    }

    @NonNull
    private static void getProcArch(String name) {
        Document doc = null;

        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse(new File("/data/system/packages.xml"));
        } catch (ParserConfigurationException e) {
            err |= ERR_XML;
            e.printStackTrace();
        } catch (IOException e) {
            err |= ERR_IO;
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        }

        Element root = doc.getDocumentElement();
        NodeList packages = doc.getElementsByTagName("package");

        for (int i = 0; i < packages.getLength(); i++) {
            Element element = (Element) packages.item(i);

            if (name.equals(element.getAttribute("name")))
                arch = abiMap.containsKey(element.getAttribute("primaryCpuAbi"))? abiMap.get(element.getAttribute("primaryCpuAbi")): ERR_ABI;
        }

        if (arch == ERR_ABI)
            err |= ERR_ABI;
    }

    public static void init(String name, LibLoader loader, int mode) {

        if (err != ERR_SUCCESS) {
            Log.e(tag, "err" + err);
            return;
        }

        if (!isAppRunning(name))
            return;

        ConfigBuilder builder = ConfigBuilder.getBuilder();
        builder.setLoader(loader);
        builder.setPid(pid);
        getProcArch(name);
        builder.setArch(arch);
        BasicConfig config = builder.build();
        InjectorImpl injector = config.getLoader().getLib();
        injector.init(config.getPid(), config.getArch(), config.getMode());
    }
}