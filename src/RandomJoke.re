open Js.Promise;

type joke = string;

type error = string;

type state =
  | Loading
  | Show(joke)
  | Error(error);

type action =
  | FetchJoke
  | JokeFetched(joke)
  | ErrorFetchingJoke;

module Decode = {
  let joke = json: joke => Json.Decode.(field("joke", string, json));
};

let fetchJoke = () =>
  Fetch.fetchWithInit(
    "https://icanhazdadjoke.com/",
    Fetch.RequestInit.make(
      ~headers=Fetch.HeadersInit.make({"Accept": "application/json"}),
      (),
    ),
  )
  |> then_(Fetch.Response.json)
  |> then_(json => json |> Decode.joke |> (joke => Some(joke) |> resolve))
  |> catch(_error => resolve(None));

let component = ReasonReact.reducerComponent("RandomJoke");

let createReducer = (fetch) => (action, _state) =>
  switch (action) {
  | FetchJoke =>
    ReasonReact.UpdateWithSideEffects(
      Loading,
      (
        component =>
          fetch()
          |> then_(result =>
               switch (result) {
               | Some(joke) => resolve(component.send(JokeFetched(joke)))
               | None => resolve(component.send(ErrorFetchingJoke))
               }
             )
          |> ignore
      ),
    )
  | JokeFetched(joke) => ReasonReact.Update(Show(joke))
  | ErrorFetchingJoke =>
    ReasonReact.Update(Error("Error fetching a joke, try again ..."))
  };

let make = (~loadingMessage="loading ...", ~fetch=fetchJoke, _children) => {
  ...component,
  initialState: () => Loading,
  reducer: createReducer(fetch),
  didMount: self => self.send(FetchJoke),
  render: self =>
    switch (self.state) {
    | Loading => <div> {ReasonReact.string(loadingMessage)} </div>
    | Show(joke) => <div> {ReasonReact.string(joke)} </div>
    | Error(error) => <div> {ReasonReact.string(error)} </div>
    },
};
