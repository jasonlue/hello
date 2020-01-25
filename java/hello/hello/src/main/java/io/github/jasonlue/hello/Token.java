package io.github.jasonlue.hello;

import java.nio.ByteBuffer;
import java.util.UUID;

import org.apache.commons.codec.binary.Base64;

class Token {
    public static String token() {
        UUID uuid = UUID.randomUUID();
        ByteBuffer byteBuffer = ByteBuffer.allocate(16);
        byteBuffer.putLong(0, uuid.getLeastSignificantBits());
        byteBuffer.putLong(8, uuid.getMostSignificantBits());
        String token = Base64.encodeBase64URLSafeString(byteBuffer.array());
        return token;
    }

    public static void main(String[] args) {
        System.out.println(token());
    }
}