-module(tcp_server).
-compile(export_all).

%% parallel accept
start_parallel_server() ->
  {ok, Listen} = gen_tcp:listen(8175,[binary,{packet,4},{reuseaddr,true},{active,true}]),
  spawn(fun() -> par_connect(Listen) end).

par_connect(Listen) ->
  {ok, Socket} = gen_tcp:accept(Listen),
  spawn(fun() -> par_connect(Listen) end),
  loop(Socket).

%% accept once
start_nano_server() ->
  {ok, Listen} = gen_tcp:listen(8175,[binary,{packet,4},{reuseaddr,true},{active,true}]),
  {ok, Socket} = gen_tcp:accept(Listen),
  gen_tcp:close(Listen),
  loop(Socket).


loop(Socket) ->
  receive
    {tcp, Socket, Bin} ->
      io:format("Server received binary = ~p~n", [Bin]),
      Str = binary_to_term(Bin),
      io:format("Server (unpacked) ~p~n", [Str]),
      gen_tcp:send(Socket, term_to_binary(Str)),
      loop(Socket);
    {tcp_closed, Socket} ->
      io:format("Server socket closed~n")
  end.
