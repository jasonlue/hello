
event new_event(name:string, args:call_argument_vector)
    {
    print name;
    for(i in args)
        {
        print fmt("[%d] %s(%s) = %s", i, args[i]$name, args[i]$type_name, args[i]?$value ? args[i]$value : args[i]$default_val);
        }
    print "";
    }

event bro_init()
    {
    local myset = vector(1,2,3,4);
    for(i in myset)
        {#always index or key; never value. as in vector, set, table.
        print i;
        }   
    }

event bro_done()
    {
        local t1 : time = current_time();
        local t2 : time = network_time();
        local diff : interval = t2 - t1;
        print t1, t2, diff;
        
        local p : port;
        p = 80/tcp;
        print p, get_port_transport_proto(p), port_to_count(p);

        local a : addr;
        a = 1.2.3.4;
        local n : subnet;
        n = 192.168.0.0/16;
        local m: subnet;
        m = a / 16;
        print a, n, m, m == n, a in n, a in m;

    }