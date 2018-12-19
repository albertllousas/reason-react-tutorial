let component = ReasonReact.statelessComponent("HelloWorldReact");

let make = (_) => {
  ...component, /* spread the template's other defaults into here  */
  render: _self => <div> {ReasonReact.string("Hello world!")} </div>
};