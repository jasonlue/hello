package io.github.jasonlue.hello;

import java.util.ArrayList;
import java.util.List;

 class Generics {
    public static void main(String[] args){
        List objs = new ArrayList();
        objs.add(1);

        List<?> unknowns = new ArrayList<Number>();
        unknowns.add(null);
        //unknowns.add(1); //not allowed, compiling error.

        //Event T derived from S, List<T> & List<S> have no connections. 
        List<Number> nums = new ArrayList<Number>();
        //nums = new ArrayList<Object>(); //compiling error. although Number extends Object, List<Number> doesn't extend List<Object>

        nums.add(1.0);//subtype is fine.
        nums.add(Integer.valueOf(2));
        nums.stream().forEach(s->System.out.println(s));

//      List<Number> nums3 = new ArrayList<Integer>(); //compiling error.
//wildcards defines relationship between containers.
        List<? extends Number> extendsNums = new ArrayList<Integer>();
        extendsNums = new ArrayList<Number>();
        //extendsNums = new ArrayList<Object>(); //compiling error.

        //List<?super Number> superNums = new ArrayList<Integer>(); //compiling error
        List<?super Number> superNums = new ArrayList<Number>();
        //superNums = new ArrayList<Integer>(); //compiling error
        superNums = new ArrayList<Object>(); 

    }
}