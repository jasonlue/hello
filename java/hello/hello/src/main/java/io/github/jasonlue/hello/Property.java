package io.github.jasonlue.hello;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Properties;

class Property {
    public static void main(String[] args) { // test Properties mechanism. https://www.baeldung.com/java-properties
        // usu. in file names with *.properties. such as local.properties
        // service-defaults.properties etc.
        // simple format K=V, both in string format.
        // Properties derived from HashMap<String,String>
        // use load(), store(), getProperty(key), getProperty(key,defaultVal),
        // setProperty(key, val), remove(key)
        String rootPath = Thread.currentThread().getContextClassLoader().getResource("").getPath();
        System.out.printf("rootPath=%s\n", rootPath);
        try { // set cwd = hello/hello
            Properties defaultProps = new Properties();
            defaultProps.load(new FileInputStream("etc/default.properties"));
            Properties appProps = new Properties(defaultProps);
            appProps.load(new FileInputStream("etc/app.properties"));

            System.out.printf("app.name=%s\n", appProps.getProperty("app.name"));
            System.out.printf("app.version=%s\n", appProps.getProperty("app.version"));
            System.out.printf("app.only.in.default=%s\n", appProps.getProperty("app.only.in.default"));
            System.out.printf("app.not.anywhere=%s\n", appProps.getProperty("app.not.anywhere", "default"));

            appProps.setProperty("app.running", "yeah");

            appProps.list(System.out);
            // the settings in default will not stored?? makes some sense, but not for all.
            // not good!
            appProps.store(new FileOutputStream("etc/running.properties"), "running.properties");

            ///////////////////////////////////////////////////////////////////////////////////////////////
            // apache.commons Configuration enhances the basic Properties class.
            // https://commons.apache.org/proper/commons-configuration/userguide/howto_properties.html
            // https://commons.apache.org/proper/commons-configuration/userguide/quick_start.html

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
