open Lwt;

open Cohttp;

open Cohttp_lwt_unix;

type handlers 'a 'b = {gets: list 'a, posts: list 'b};

let create () => {gets: [], posts: []};

let post url handler router => {gets: router.gets, posts: [(url, handler), ...router.posts]};

let get url handler router => {gets: [(url, handler), ...router.gets], posts: router.posts};

let not_found req body => Server.respond_string status::`OK body::"404" ();

let callback router => {
  let gets = List.rev router.gets;
  let posts = List.rev router.posts;
  fun _conn req body => {
    let path = req |> Request.uri |> Uri.path;
    let routes =
      switch (Request.meth req) {
      | `GET => gets
      | `POST => posts
      | _ => failwith "Unsupported request method"
      };
    let (url, handler) =
      try (List.find (fun (url, handler) => url == path) routes) {
      | Not_found => ("/404", not_found)
      };
    handler req body
  }
};
