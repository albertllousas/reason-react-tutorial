
type joke = string;

type error = string;

type state =
  | Loading
  | Show(joke)
  | Error(error);

type action =
  | FetchJoke
  | JokeFetched(joke)
  | ErrorFetchingJoke(error);

let component = ReasonReact.reducerComponent("RandomJoke");

let make = (~loadingMessage="loading ...", _children) => {
  ...component,
  initialState: () => Loading,
  reducer: (action, state) => switch action {
    | FetchJoke => ReasonReact.NoUpdate
    | JokeFetched(joke) => ReasonReact.NoUpdate
    | ErrorFetchingJoke(error) => ReasonReact.NoUpdate
    },
  render: self => switch (self.state) {
  | Loading => <div> (ReasonReact.string(loadingMessage)) </div>
  | Show(joke) => <div> (ReasonReact.string("TODO")) </div>
  | Error(error) => <div> (ReasonReact.string("TODO")) </div>
  }
};