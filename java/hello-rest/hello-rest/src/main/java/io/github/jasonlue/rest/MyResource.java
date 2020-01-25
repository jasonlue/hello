
package io.github.jasonlue.rest;

import java.security.Principal;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import javax.annotation.security.RolesAllowed;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.SecurityContext;

// The Java class will be hosted at the URI path "/myresource"
@Path("/v1/sensor")
public class MyResource {
    @Context
    SecurityContext securityContext;
    private List<Resource> res = new ArrayList<Resource>();

    public MyResource() {
        for (int i = 0; i < 10; i++) {
            res.add(new Resource("name-" + Integer.toString(i), "value+" + Integer.toString(i),
                    "hidden#" + Integer.toString(i)));
        }
    }

    @Path("{resource_id}")
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Resource list(@PathParam("resource_id") int resource_id) {
 //       Principal p = securityContext.getUserPrincipal();
 //       String user = p.getName();
        return res.get(resource_id);
    }

//    @RolesAllowed("ADMIN")
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public List<Resource> listAll(@DefaultValue("") @QueryParam("name") String name) {
        return res.stream()/*.filter(r-> name == "" || r.name==name)*/.collect(Collectors.toList());
    }

    // @GET
    // @Produces("text/plain")
    // public String getItAsString() {
    // return res.toString();
    // }
}
