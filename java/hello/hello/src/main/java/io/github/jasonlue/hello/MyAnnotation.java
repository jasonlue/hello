package io.github.jasonlue.hello;

import java.lang.annotation.Annotation;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.reflect.Method;
import java.util.Map;
import java.lang.reflect.Field;

@Retention(RetentionPolicy.RUNTIME)
@interface NameAnnotation {
    String name();

    int val();
}

class DynamicNameAnnotation implements NameAnnotation {
    private String name;
    private int val;

    DynamicNameAnnotation(String name, int val) {
        this.name = name;
        this.val = val;
    }

    @Override
    public String name() {
        return this.name;
    }

    @Override
    public int val() {
        return this.val;
    }

    @Override
    public Class<? extends Annotation> annotationType() {
        return DynamicNameAnnotation.class;
    }
};

@NameAnnotation(name = "MyAnnotation", val = 777)
public class MyAnnotation {
    @NameAnnotation(name = "Name", val = 666)
    public static void main(String[] args) {
        try {
            Class<MyAnnotation> c = MyAnnotation.class;
            Method m = c.getMethod("main", String[].class);
            NameAnnotation anno = m.getAnnotation(NameAnnotation.class);
            System.out.println(anno.name() + " " + Integer.toString(anno.val()));

            // modify the annotation value.
            Method method = Class.class.getDeclaredMethod("annotationData", null);
            method.setAccessible(true);
            Object annotationData = method.invoke(MyAnnotation.class);
            Field annotations = annotationData.getClass().getDeclaredField("annotations");
            annotations.setAccessible(true);
            Map<Class<? extends Annotation>, Annotation> map = (Map<Class<? extends Annotation>, Annotation>) annotations
                    .get(annotationData);
            map.put(NameAnnotation.class, new DynamicNameAnnotation("name", 999));

            anno = MyAnnotation.class.getAnnotation(NameAnnotation.class);
            System.out.println(anno.name() + " " + Integer.toString(anno.val()));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void AnnotationTest() {

    }
}