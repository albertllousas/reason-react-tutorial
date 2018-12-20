type state = {
  loading: bool
};

type action = Fetching;

let component = ReasonReact.reducerComponent("RandomJoke");

let make = (~loadingMessage="loading ...", _children) => {
  ...component,
  initialState: () => {loading: true},
  reducer: (action, state) => switch (action) {
    | Fetching => ReasonReact.Update({...state, loading: true})
    },
  render: self =>
    <div>(self.state.loading ? ReasonReact.string(loadingMessage): ReasonReact.null)</div>
};