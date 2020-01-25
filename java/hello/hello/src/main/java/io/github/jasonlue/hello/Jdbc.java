package io.github.jasonlue.hello;

import java.sql.*;

import org.apache.commons.lang3.builder.ToStringBuilder;

class Jdbc {
    public static void main(String[] args) {
        try (Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/auth_local", "root",
//        try (Connection conn = DriverManager.getConnection("jdbc:postgresql://localhost:5432/sensor", "sensor",
                "He!!o2mysql")) {
            System.out.println("Connected to PostgreSQL database: sensor!");
            Statement stmt = conn.createStatement();
            System.out.println("reading from sensor.sensor.sensor..."); // default is set to sensor schema at db level.
            ResultSet rs = stmt.executeQuery("SELECT * FROM sensor");
            while (rs.next()) {
                System.out.println(new ToStringBuilder(null).append("sensor_id", rs.getString("sensor_id"))
                        .append("account_code", rs.getString("account_code"))
                        .append("created", rs.getTimestamp("created")).append("updated", rs.getTimestamp("updated"))
                        .toString());
            }

        } catch (SQLException e) {
            System.out.println("Connection failure.");
            e.printStackTrace();
        }
    }
}
