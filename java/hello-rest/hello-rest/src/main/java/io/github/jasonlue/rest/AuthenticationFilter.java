
package io.github.jasonlue.rest;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.container.ContainerRequestContext;
import javax.ws.rs.container.ContainerRequestFilter;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.SecurityContext;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.ext.Provider;

@Provider
public class AuthenticationFilter implements ContainerRequestFilter {
    public static final String AUTHENTICATION_HEADER = "Authorization";

    @Override
    public void filter(ContainerRequestContext requestContext) throws WebApplicationException {
        // http authentication:
        // Authorization: <type> <credentials>
        // Autorization: Basic base64 encoded user/password
        // example: Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l
        String authCredentials = requestContext.getHeaderString(AUTHENTICATION_HEADER);

        // better injected
        AuthenticationService authenticationService = new AuthenticationService();

        boolean authenticationStatus = authenticationService.authenticate(authCredentials);

        if (!authenticationStatus) {
            throw new WebApplicationException(Status.UNAUTHORIZED);
        }
    }
}