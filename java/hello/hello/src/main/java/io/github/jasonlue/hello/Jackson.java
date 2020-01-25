package io.github.jasonlue.hello;

import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.PropertyNamingStrategy;
import com.fasterxml.jackson.databind.annotation.JsonNaming;


import org.apache.commons.lang3.builder.ToStringBuilder;

class Jackson {
    public static void main(String[] args) { // use jackson to serirlalize and deserialize objects.
        ObjectMapper objectMapper = new ObjectMapper();
        try {
            OptionalSerialization(objectMapper);
            PropertyNamingStrategy(objectMapper);
            DeserializationFeature(objectMapper);
            Collections(objectMapper);
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void OptionalSerialization(ObjectMapper objectMapper) throws Exception{
        class Resource{
            public String id = "id_0";
            @JsonInclude(JsonInclude.Include.NON_NULL)
            public String optional = null;
        }
        class IncludeResource extends Resource{
            @JsonInclude(JsonInclude.Include.ALWAYS)
            public String getOptional(){
                return optional;
            }
        }

        Resource r = new Resource();
        IncludeResource ir = new IncludeResource();
        System.out.printf("resource: %s\n", objectMapper.writeValueAsString(r));
        System.out.printf("included resorces: %s\n", objectMapper.writeValueAsString(ir));
    }
    
    public static void Collections(ObjectMapper objectMapper) throws Exception{
        List<Sensor> sensors = new ArrayList<Sensor>();
        for (int i = 0; i < 10; i++) {
            sensors.add(new Sensor("sensor_id_" + Integer.toString(i), "account_code_" + Integer.toHexString(i + 100)));
        }
        System.out.println(ToStringBuilder.reflectionToString(sensors));

        String s = objectMapper.writeValueAsString(sensors);
        System.out.println(s);
        List<Sensor> l = objectMapper.readValue(s, new TypeReference<List<Sensor>>() {});
        System.out.println(l);
    }

    public static void DeserializationFeature(ObjectMapper objectMapper) throws Exception {
        Sensor exact = objectMapper.readValue("{\"sensorId\" : \"sensor_id_0\", \"account_code\":\"account_code_0\"}",
                Sensor.class);
        Sensor missing = objectMapper.readValue("{\"account_code\":\"account_code_0\"}", Sensor.class);
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        Sensor extra = objectMapper.readValue(
                "{\"sensorId\" : \"sensor_id_0\", \"account_code\":\"account_code_0\", \"extra\": true}", Sensor.class);
        objectMapper.configure(DeserializationFeature.FAIL_ON_NULL_FOR_PRIMITIVES, false);
        // enum numbers / names.
        objectMapper.configure(DeserializationFeature.FAIL_ON_NUMBERS_FOR_ENUMS, false);
        objectMapper.setPropertyNamingStrategy(PropertyNamingStrategy.SNAKE_CASE);

    }

    public static void PropertyNamingStrategy(ObjectMapper objectMapper) throws Exception {
        // default naming, straight property name.

        class Default{//LowerCamelCase
            public String sensorId = "sensor_id_camel";
        }
        System.out.printf("Camel: %s", objectMapper.writeValueAsString(new Default()));

        @JsonNaming(PropertyNamingStrategy.SnakeCaseStrategy.class)
        class Snake{
            public String sensorId = "sensor_id_snake";
        }
        System.out.printf("Snake: %s", objectMapper.writeValueAsString(new Snake()));

        @JsonNaming(PropertyNamingStrategy.UpperCamelCaseStrategy.class)
        class Pascal{
            public String sensorId = "sensor_id_pascal";
        }
        System.out.printf("Pascal: %s", objectMapper.writeValueAsString(new Pascal()));

        @JsonNaming(PropertyNamingStrategy.KebabCaseStrategy.class)
        class Kebab{
            public String sensorId = "sensor_id_kebab";
        }
        System.out.printf("Kebab: %s", objectMapper.writeValueAsString(new Kebab()));


        //////////////////////////////////////////////////////////////////////////////
        //setting globally for all classes?
        //
        //
        // Trick: setting need to be before the instance to be serialized created. 
        //Jackson sticks to the objectMapper when instance is created.

        //////////////////////////////////////////////////////////////////////////
        Default def = new Default();

        // default: sensorId
        //depends on @JsonNaming annotation.
        System.out.println("DEFAULT property naming strategy.");
        System.out.println(objectMapper.writeValueAsString(def));
        PropertyNamingStrategy pns = objectMapper.getPropertyNamingStrategy();

        //lower camel: sensorId, this is the default.
        System.out.println("LOWER_CAMEL_CASE property naming strategy");
        objectMapper.setPropertyNamingStrategy(PropertyNamingStrategy.LOWER_CAMEL_CASE);
        pns = objectMapper.getPropertyNamingStrategy();
        System.out.println(objectMapper.writeValueAsString(def));

        System.out.println("SNAKE_CASE property naming strategy");
        objectMapper.setPropertyNamingStrategy(PropertyNamingStrategy.SNAKE_CASE);
        pns = objectMapper.getPropertyNamingStrategy();
        System.out.println("Switch to Snake");
        Sensor snake = new Sensor("sensor_id_snake", "sensor_code_snake");
        System.out.printf("serialize sensor: %s\n", objectMapper.writeValueAsString(new Default()));
        System.out.printf("serialize snake: %s\n", objectMapper.writeValueAsString(def));

        // upper camel: SensorId
        System.out.println("UPPER_CAMEL_CASE property naming strategy");
        objectMapper.setPropertyNamingStrategy(PropertyNamingStrategy.UPPER_CAMEL_CASE);
        System.out.println(objectMapper.writeValueAsString(def));

        // sensorid
        System.out.println("LOWER_CASE property naming strategy");
        objectMapper.setPropertyNamingStrategy(PropertyNamingStrategy.LOWER_CASE);
        System.out.println(objectMapper.writeValueAsString(def));

        // sensor-id
        System.out.println("KEBAB_CASE property naming strategy");
        objectMapper.setPropertyNamingStrategy(PropertyNamingStrategy.KEBAB_CASE);
        System.out.println(objectMapper.writeValueAsString(def));

        // switch back to default?
        System.out.println("KEBAB_CASE property naming strategy");
        objectMapper.setPropertyNamingStrategy(null);
        System.out.println(objectMapper.writeValueAsString(def));
    }
}