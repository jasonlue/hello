package io.github.jasonlue.hello;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

class Collections {
    public static void main(String[] args) {
        List<Integer> names = new ArrayList<Integer>();
        names.add(1);
        names.add(2);
        Iterator it = names.iterator();
        while (it.hasNext()) {
            System.out.println(it.next());
        }

        Map<Integer, String> map = new HashMap<Integer, String>();
        map.put(1, "1");
        map.put(2, "2");
        map.put(3, "3");
        for (Map.Entry m : map.entrySet()) {
            System.out.println(m.getKey() + " " + m.getValue());
        }
    }
}
