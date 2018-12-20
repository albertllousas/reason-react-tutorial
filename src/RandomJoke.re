
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
  reducer: (action, _state) => switch action {
    | FetchJoke => ReasonReact.NoUpdate
    | JokeFetched(_joke) => ReasonReact.NoUpdate
    | ErrorFetchingJoke(_error) => ReasonReact.NoUpdate
    },
  render: self => switch self.state {
  | Loading => <div> (ReasonReact.string(loadingMessage)) </div>
  | Show(_joke) => <div> (ReasonReact.string("TODO")) </div>
  | Error(_error) => <div> (ReasonReact.string("TODO")) </div>
  }
};