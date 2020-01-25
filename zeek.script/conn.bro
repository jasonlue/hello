global local_subnets : vector of subnet =[192.168.1.0/24,192.68.2.0/24, 172.16.0.0/20, 172.16.16.0/20, 172.16.32.0/20, 172.16.48.0/20];
global conns : set[string];

#event new_connection(c:connection)
event connection_state_remove(c:connection)
{
    if(|conns|<10)
    {
        if( c$uid !in conns)
        {
            add conns[c$uid];
            local service : string = "";
            if(|c$service|>0)
            {
                for(j in c$service)
                    service += j + " ";
            }
            else
            {
                service = "-";
            }
            if( c?$conn)
            {#fields to write to log files.
                local i = c$conn;
                local parents = "(empty)";
                if(i?$tunnel_parents && |i$tunnel_parents|>0)
                {
                    parents = "(";
                    for(k in i$tunnel_parents)
                        parents += k + " ";
                    parents += ")";
                }
                print i$ts, i$uid, i$id$orig_h, port_to_count(i$id$orig_p), i$id$resp_h, port_to_count(i$id$resp_p),i$proto,i?$service?i$service:"-",i?$duration?fmt("%f",i$duration):"-",
                    i?$orig_bytes?fmt("%d",i$orig_bytes):"-", i?$resp_bytes?fmt("%d",i$resp_bytes):"-", 
                    i?$conn_state?i$conn_state:"-", i?$local_orig ? i$local_orig : F, i?$local_resp? i$local_resp : F, 
                    i$missed_bytes, i$history, i$orig_pkts, i$orig_ip_bytes,
                    i$resp_pkts, i$resp_ip_bytes,parents;
            }
            else
            {
                print c$start_time, c$uid, c$id$orig_h, port_to_count(c$id$orig_p),
                    c$id$resp_h, port_to_count(c$id$resp_p), get_port_transport_proto(c$id$orig_p), service, c$duration;
            }
        }
    }
}

