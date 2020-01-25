package io.github.jasonlue.hello;

import java.util.ArrayList;
import java.util.List;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        System.out.println("Hello World!");
        wildcast();
    }

    private static void wildcast() {
        List<? extends Number> numbers = new ArrayList<Number>();
        List<? extends Number> ints = new ArrayList<Integer>();
        List<? extends Number> doubles = new ArrayList<Double>();

    }

}
