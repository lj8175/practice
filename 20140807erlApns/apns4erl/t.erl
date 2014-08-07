-module(t).
-compile(export_all).
-define(APNS_NAME,app_apns).

-include("apns.hrl").
-include("localized.hrl").

conn_apns() ->
     ssl:start(),
     apns:start(),
     apns:connect(
	?APNS_NAME,
        fun handle_apns_error/2,
        fun handle_apns_delete_subscription/1
      ).

send_message()->
   apns:send_message(?APNS_NAME, "e3ab84d2d0673c8776bb831a093fe61bea2180639993fa734bb5f0a686188eb5", "hello,world").
   %%apns:send_message(?APNS_NAME, "devicetoken31d1df3a324bb72c1ff2bcb3b87d33fd1a2b7578b359fb5494eff", "hello,world").

send_message(Msg) ->
apns:send_message(?APNS_NAME, #apns_msg{
      alert  = Msg ,
      badge  = 5,
      sound  = "beep.wav" ,
      expiry = 1448000749,
      device_token = "47de4e1f93c3f8baa0909ebff0495205c594abe7d0ebeef832c13052981f0cda"
    }).

handle_apns_error(MsgId, Status) ->
      error_logger:error_msg("error: ~p - ~p~n", [MsgId, Status]).

handle_apns_delete_subscription(Data) ->
      error_logger:info_msg("delete subscription: ~p~n", [Data]).
