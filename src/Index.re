open Cohttp_lwt_unix;

let router =
  Router.create () |>
  Router.get "/" (fun req body => Server.respond_string status::`OK body::"hey!" ()) |>
  Router.post "/" (fun req body => Server.respond_string status::`OK body::"posted!" ());

let server =
  Server.create mode::(`TCP (`Port 8000)) (Server.make callback::(Router.callback router) ());

ignore (Lwt_main.run server);
