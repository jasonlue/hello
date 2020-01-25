package io.github.jasonlue.hello;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

//6 major operations
//intermediate: map (map each element to another value), filter (filter conditions ), sorted
//terminal: forEach (operate on each element in collection), collect (a subset of collection), reduce (to one element).

//refined terminal: allMatch, anyMatch, count(), distinct(), empty(), findAny(), findFirst() toArray()

class Streams {
    public static void main(String[] args) {
        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9);
        numbers.stream().map(i -> 2 * i).forEach(i -> System.out.println(i));
        numbers.stream().filter(i -> i % 2 == 0).forEach(i -> System.out.println(i));
        numbers.stream().sorted((i, j) -> j - i).forEach(x -> System.out.println(x));

        int sum = numbers.stream().reduce(0, (r, i) -> r + i);
        int sum2 = numbers.stream().reduce(0, Integer::sum);
        int sum3 = numbers.stream().mapToInt(e -> e).sum(); // sum() only works on int, not Integer.
        long count = numbers.stream().count();

        int mul = numbers.parallelStream().reduce(1, (r, i) -> r * i);

        System.out.printf("sum: %d, %d, %d, mul: %d\n", sum, sum2, sum3, mul);
        List<Integer> squares = numbers.stream().filter(x -> x >= 3).map(x -> x * x).sorted((x, y) -> y - x)
                .collect(Collectors.toList());
        System.out.println(squares);

        // misc operations
        System.out.println("allMatch(): " + Boolean.toString(numbers.stream().allMatch(i -> i % 2 == 0)));
        System.out.printf("anyMatch(): %b\n", numbers.stream().anyMatch(i -> i % 2 == 0));
        System.out.printf("noneMatch(): %b\n", numbers.stream().noneMatch(i -> i % 2 == 0));


        //Collectors/CollectUtil
        

    }
}
