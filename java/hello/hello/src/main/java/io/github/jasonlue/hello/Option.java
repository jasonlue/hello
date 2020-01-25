package io.github.jasonlue.hello;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

class USB {
    public String getVersion() {
        return "3.0";
    }
}

class Soundcard {
    private USB usb = null;

    public USB getUSB() {
        return new USB();
    }

    public Optional<USB> getOptionalUSB() {
        return usb == null ? Optional.empty() : Optional.of(usb);
    }
}

class Computer {
    private Soundcard sc = null;

    public Soundcard getSoundcard() {
        return sc;
    }

    public Optional<Soundcard> getOptionalSoundcard() {
        return sc == null ? Optional.empty() : Optional.of(sc);
    }
}

class Option{
    public static void main(String[] args) {

        // https://www.oracle.com/technetwork/articles/java/java8-optional-2175753.html
        // Optional<T> and strema are hand in hand.
        // Optional<T> like stream on collection with 0 or 1 element of type T.
        // not useful to use directly, but can be useful after filtering.
        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9);
        Optional<Integer> findAny = numbers.stream().filter(i -> i % 3 == 0).findAny();
        Optional<Integer> findFirst = numbers.stream().filter(i -> i % 5 == 0).findFirst();
        System.out.printf("findAny? %d, findFirst?%d\n", findAny.orElse(0), findFirst.orElse(0)); // returns the value
                                                                                                  // or the zero of the
                                                                                                  // type.
        findFirst.filter(i -> i > 10).ifPresent(i -> System.out.println(i));

        Optional<String> emptyStr = Optional.empty();
        Optional<String> solidStr = Optional.of("solidStr");
        solidStr.map(s -> s.toLowerCase()).ifPresent(s -> System.out.println(s));
        // since member function takes this as first param. we can simplify it.
        solidStr.map(String::toLowerCase).ifPresent(s -> System.out.println(s));
        emptyStr.orElse("empty");

        Optional<String> x = Optional.empty();
        Optional<String> y = solidStr.map(String::toLowerCase).map(String::toUpperCase);
        y.ifPresent(s -> System.out.println(s));

        try {
            Computer c = new Computer();
            String version = c.getSoundcard().getUSB().getVersion(); // handle exception.
        } catch (Exception e) {
            e.printStackTrace();
        }
        {   //flatMap to reduce Optional<Optional<T>> to Optional<T>
            Computer computer = new Computer();
            String version = computer.getOptionalSoundcard().flatMap(Soundcard::getOptionalUSB).map(USB::getVersion).orElse("0.0");
            System.out.println("USB version: " + version);

        }
    }
}