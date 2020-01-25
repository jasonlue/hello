package io.github.jasonlue.hello;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class HelloTest {

    @BeforeClass
    public static void beforeClass() {
        System.out.println("beforeClass()");
    }

    @Before
    public void before() {
        System.out.println("before()");
    }

    @Test
    public void testA() {
        System.out.println("testA()");
    }

    @Test
    public void testB() {
        System.out.println("testB()");
    }

    @Test
    public void testC() {
        System.out.println("testC()");
    }

    @After
    public void after() {
        System.out.println("after()");
    }

    @AfterClass
    public static void afterClass() {
        System.out.println("afterClass()");
    }

    public static void main(String[] args) {
        System.out.println("Hello, Test!");
    }

}