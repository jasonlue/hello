package io.github.jasonlue.hello;

//used by jackson & Hibernate examples.
import java.sql.Timestamp;
import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.fasterxml.jackson.databind.PropertyNamingStrategy;
import com.fasterxml.jackson.databind.annotation.JsonNaming;

import org.apache.commons.lang3.builder.ToStringBuilder;

//hibernat orm requires sojo java beans. ie. all attributes private with gettters/setters
//same class for app, json & hibernate.
@Entity // indicates this is an entity in db. ie. a row in table.
@Table(name = "sensor") // entity in db table sensor.

// jackson json serializer only serialize public attributes, either the
// attribute is public or its getter/setting public, if name not specified,
// assume the same as field name.
//@JsonPropertyOrder({ "sensor_id" }) // set sensor_id as the first field in json.
//@JsonNaming(PropertyNamingStrategy.SnakeCaseStrategy.class)
//@JsonNaming(PropertyNamingStrategy.UpperCamelCaseStrategy.class)
//@JsonNaming(PropertyNamingStrategy.KebabCaseStrategy.class)
public class Sensor {
    private String sensorId;

    private String accountCode;

    private Timestamp created;

    private Timestamp updated;

    public Sensor() {

    }

    public Sensor(String sensorId, String accountCode) {
        this.sensorId = sensorId;
        this.accountCode = accountCode;
        this.created = new Timestamp(new Date().getTime());
        this.updated = new Timestamp(new Date().getTime());
    }

    @Override
    public String toString() {
        return ToStringBuilder.reflectionToString(this);
    }

    @Id
    @Column(name = "sensor_id")
   // @JsonProperty("sensor_id")    
    public String getSensorId() {
        return sensorId;
    }

    public void setSensorId(String sensorId) {  
        this.sensorId = sensorId;
    }

    @Column(name = "account_code")
    // @JsonProperty("account_code")
    public String getAccountCode() {
        return accountCode;
    }

    public void setAccountCode(String accountCode) {
        this.accountCode = accountCode;
    }

    @Column(name = "created")
    @JsonIgnore // ignore created field in json serialization.
    public Timestamp getCreated() {
        return created;
    }

    public void setCreated(Timestamp created) {
        this.created = created;
    }

    @Column(name = "updated")
    @JsonIgnore // ignore created field in json serialization.
    public Timestamp getUpdated() {
        return updated;
    }

    public void setUpdated(Timestamp updated) {
        this.updated = updated;
    }
}
