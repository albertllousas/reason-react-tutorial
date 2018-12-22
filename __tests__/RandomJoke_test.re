open Jest;
open ReactTestRenderer;
open ExpectJs;

let joke = "Can a kangaroo jump higher than a house? Of course, a house doesn’t jump at all."

let data = {| {
  "id": "R7UfaahVfFd",
  "joke": "Can a kangaroo jump higher than a house? Of course, a house doesn’t jump at all.",
  "status": 200
  } |}; 

let json = data |> Json.parseOrRaise;

let fetchSuccessMock = () => Js.Promise.make((~resolve, ~reject) => resolve(. Some(joke)))

let fetchFailMock = () => Js.Promise.make((~resolve, ~reject) => resolve(. None))

describe("Decode", () => {
  test("Decode a json joke", () => {
    expect(RandomJoke.Decode.joke(json)) |> toBe(joke);
  });
});

describe("<RandomJoke />", () => {
  
  Enzyme.configureEnzyme(Enzyme.react_16_adapter()) |> ignore;

  test("render without crashing", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomJoke fetch={fetchSuccessMock}/>);
    expect(Js.Undefined.return(component)) |> toBeDefined;
  });

  test("should have a loading message while fetching", () => {
    let wrapper = Enzyme.shallow(<RandomJoke fetch={fetchSuccessMock} />);
    let expectedNode = <div> (ReasonReact.string("loading ...")) </div>;
    expect(Enzyme.Shallow.matchesElement(expectedNode, wrapper)) |> toBe(true)
  });

  testAsync("should have a joke when fetch succeed", (finish) => {
    let wrapper = Enzyme.shallow(<RandomJoke fetch={fetchSuccessMock} />);
    let expectedNode = <div> (ReasonReact.string({j|$joke|j})) </div>;
    Js.Global.setTimeout(() => {
      Enzyme.Shallow.matchesElement(expectedNode, wrapper)== true ? finish(pass) : finish(fail(""))
    }, 1) |> ignore;
  });

  testAsync("should have a error message when fetch fails", (finish) => {
    let wrapper = Enzyme.shallow(<RandomJoke fetch={fetchFailMock} />);
    let expectedNode = <div> (ReasonReact.string("Error fetching a joke, try again ...")) </div>;
    Js.Global.setTimeout(() => {
      Enzyme.Shallow.matchesElement(expectedNode, wrapper)== true ? finish(pass) : finish(fail(""))
    }, 1) |> ignore;
  });

  /* test("snapshot while loading", () => {
    let json = toJSON(create(<RandomJoke />));
    expect(json)  |> toMatchSnapshot;
  });*/

});