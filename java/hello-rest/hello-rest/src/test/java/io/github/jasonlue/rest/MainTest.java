
package io.github.jasonlue.rest;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.WebTarget;

import org.glassfish.grizzly.http.server.HttpServer;

import junit.framework.TestCase;

public class MainTest extends TestCase {

    private HttpServer httpServer;

    private WebTarget target;

    public MainTest(String testName) {
        super(testName);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        // start the Grizzly2 web container
        httpServer = Main.startServer();

        // create the client
        Client c = ClientBuilder.newClient();
        target = c.target(Main.BASE_URI);
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();

        httpServer.shutdownNow();
    }

    /**
     * Test to see that the message "Got it!" is sent in the response.
     */
    public void testMyResource() {
        String responseMsg = target.path("myresource").request().get(String.class);
        assertEquals("Got it!", responseMsg);
    }

    /**
     * Test if a WADL document is available at the relative path "application.wadl".
     */
    // public void testApplicationWadl() {
    // String serviceWadl = target.path("application.wadl").
    // accept(MediaType.WADL).get(String.class);

    // assertTrue(serviceWadl.length() > 0);
    // }
}
