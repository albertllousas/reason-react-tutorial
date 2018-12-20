open Jest;

describe("<RandomRemoteDog />", () => {
  open ExpectJs;

  test("render", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomRemoteDog />);
    expect(Js.Undefined.return(component)) |> toBeDefined;
  });

  test("snapshot while loading", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomRemoteDog />);
    expect(Js.Undefined.return(component)) |> toMatchSnapshot;
  });

  test("snapshot while loading changing the default message", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomRemoteDog loadingMessage="Wait for the dog ..."/>);
    expect(Js.Undefined.return(component)) |> toMatchSnapshot;
  });

});