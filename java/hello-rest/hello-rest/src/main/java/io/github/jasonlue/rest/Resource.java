package io.github.jasonlue.rest;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;

@JsonPropertyOrder("value")
public class Resource{
    @JsonProperty("json-name")
    public String name;
    public String value;
    private String hidden;

    public Resource(){
        name = "name";
        value = "value";
        hidden = "hidden";        
    }
    public Resource(String name, String value, String hidden){
        this.name = name;
        this.value = value;
        this.hidden = hidden;
    }
}