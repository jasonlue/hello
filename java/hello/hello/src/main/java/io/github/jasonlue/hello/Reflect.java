package io.github.jasonlue.hello;

import java.lang.reflect.*;
import java.util.*;

class Reflect {

    private String privateName;

    public Reflect() {
        privateName = "reflect";
    }

    public Reflect(String name) {
        privateName = name;
    }

    public void hello() {
        System.out.println("hello");
    }

    public int echo(int i) {
        return i;
    }

    private void inPrivate() {
        System.out.println("in private()");
    }

    public static void main(String[] args) {
        try {
            Reflect reflect = new Reflect();
            Class<?> cls = reflect.getClass();
            Constructor ctor = cls.getConstructor();
            System.out.println("constructor: " + ctor.getName());
            Method[] methods = cls.getMethods();
            List<Method> methodList = Arrays.asList(methods);
            methodList.stream().forEach(m -> System.out.println(m.getName()));

            // method call.
            Method hello = cls.getDeclaredMethod("hello");
            hello.invoke(reflect);

            Method echo = cls.getDeclaredMethod("echo", int.class);
            System.out.printf("echo(666) returns %d\n", echo.invoke(reflect, 666));

            Method inPrivate = cls.getDeclaredMethod("inPrivate");
            inPrivate.invoke(reflect);

            Field field = cls.getDeclaredField("privateName");
            field.set(reflect, "TouchPrivate");
            System.out.println(field.get(reflect));

        } catch (NoSuchMethodException e) {
            System.out.println("constructor doesn't exist.");
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}