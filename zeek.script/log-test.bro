#simplest logging 
module LogTest;
export
{
    redef enum Log::ID += {LOG}; #add my log ID in. used as index into loggers.
    type Info : record
    {
        creator: string &default="jason" &log;
    };
}
event bro_init()
{
    Log::create_stream(LogTest::LOG, [$columns=LogTest::Info, $path="log_test"]);    
}
event bro_done()
{
    Log::write(LogTest::LOG, []);
}