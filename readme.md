# reason react - tutorial

:construction: **Note:** Reason react, tool chain and testing tools are in an early stage an most of them are still experimental, so we should expect main changes  or new libraries and possibly this tutorial will become updated.

## Getting Started
#### Installing
`npm install`
#### Building
`npm bundle`
#### Running the tests
`npm test`
#### Run app locally
`npm serve`
# Tutorial
The idea of this project is to create a simple react app with reasonml but not using a prebuild-template, instead what we want is to build a project from scratch, so we will be aware of all the steps that should be done in terms of building, coding, dependencies or design patterns.
__Assumption__: this is not a react tutorial, we will assume that the reader knows about how react works in terms of components lifecycle or virtual-dom.

- [Setup project](#setup-project)
  * [Init project and tools](#init-project-and-tools)
  * [Setup reason-react](#setup-reason-react)
  * [Adding react code](#adding-react-code)
  * [Web bundling](#web-bundling)
- [Coding time: Random Joke App](#coding-time--random-joke-app)
  * [Add testing dependencies](#add-testing-dependencies)
  * [First iteration :  show a remote random joke](#first-iteration----show-a-remote-random-joke)
    + [Writing our first test](#writing-our-first-test)
    + [Design the component](#design-the-component)
    + [Create a stateful component](#create-a-stateful-component)
    + [Fetching data](#fetching-data)
    + [The reducer](#the-reducer)
    + [Rendering](#rendering)
    + [Important design note](#important-design-note)
  * [Second iteration : refresh joke](#second-iteration)


# Setup project

## Init project and tools

```bash
mkdir your_awesome_app_name
cd your_awesome_app_name
npm init
```

To install BuckleScript (compiler that turns Reason code into JavaScript code) & Reason locally:

```bash
npm install --save-dev bs-platform
```
Generate a simple project template

```bash
./node_modules/bs-platform/lib/bsb -init . -theme basic-reason
```

This command should have generated a simple project scaffold with a simple file `src/Demo.re`:

```ocaml
Js.log("Hello, BuckleScript and Reason!");
```

Since we are have generated already a project we should update scripts in `package.json` in order to compile and run reason code:

```json
"scripts": {
    "test": "echo \"Error: no test specified\" && exit 1",
    "build": "bsb -make-world",
    "start": "bsb -make-world -w",
    "clean": "bsb -clean-world"
  },
```

Since we now have a .js file, we can run it with node.js:
```bash
npm run build
node src/Demo.bs.js
Hello, BuckleScript and Reason!
```

## Setup reason-react

Now we have to add react, in theory if we create a project from scratch with `bsb` and `-theme react` it would generate all the templating, but since we want to understand what we are doing let's do it manually.

First, install react:
```bash
npm install react react-dom reason-react
```

We should also change buckle script `bsconfig.json` to enable react builds adding a couple of lines, [more info](https://bucklescript.github.io/docs/en/build-configuration):

```json
"reason": {"react-jsx": 2},
"bs-dependencies": ["reason-react"],
```

## Adding react code

Now we should be able to add and compile some reason react code.

We will create a simple stateless react component in reason `HelloWorldReact.re`:

```ocaml
let component = ReasonReact.statelessComponent("HelloWorldReact");

let make = (_) => {
  ...component, /* spread the template's other defaults into here  */
  render: _self => <div> HelloWorld </div>
};

```
We should render it into the dom `Index.re`:
```ocaml
ReactDOMRe.renderToElementWithId(<HelloWorldReact />, "index");
```

And create a simple html to visualize it `index.html`:
```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Reason React - Hello world</title>
</head>
<body>
  <div id="index"></div>
  <script src="./Index.bs.js"></script>
</body>
</html>
```
Now compile and run!

```bash
npm run build
open src/index.html
```
And ... errors in the browser `Uncaught ReferenceError: require is not defined at Index.bs.js:4`

Sadly, we won't see anything because now we are just exposing a bs file but what we really need to make it work in the browser is add all the dependencies. We can do this in two different ways:

- Adding all the required scripts to the html (old way)
- Bundling all the dependecencies in one single js file (modern way)

## Web bundling

Bundling is the process to combines multiple files into a single file, besides that, optimization processes will be applied as well.

We will need a web bundler to automate all this process, so we will choose [Parcel](https://parceljs.org/), because why not?

```bash
npm install parcel-bundler --save-dev
```
Add bundle script to our `package.json`:

```json
{
  ...
  "scripts": {
   ...
   "serve": "parcel ./src/index.html",
   "bundle": "parcel build ./src/index.html"
  }
}
```

We can now bundle and serve our application in dev mode:

```bash
npm run serve
```

Or prepare the bundle for production:

```bash
npm run bundle
```

# Coding time: Random Joke App

Let's create an small real app that consumes a public api, our idea is simple, show a random joke:

- We will use a real service to get a joke : https://icanhazdadjoke.com/api
- First iteration: We will show a random joke.
- Second iteration: We will refresh the random joke with a button.

## Add testing dependencies

As a testing tools we want to use `jest` `react-test-renderer` and `enzyme`, but we should use the buckle-script bindings:

```bash
npm install --save-dev npm @glennsl/bs-jest bs-react-test-renderer bs-enzyme enzyme-adapter-react-16
```

Then add all the dependencies in your `bsconfig.json` as well:

```json
{
  ...
"bs-dependencies": ["reason-react", "@glennsl/bs-jest", "bs-enzyme", "bs-react-test-renderer"],
}
```

Add an script runner in `package.json`:

```json
{
  ...
  "scripts": {
   ...
   "test": "npm run build && jest",
   ...
  }
}
```

## First iteration :  show a remote random joke

### Writing our first test
Now, we can do some TDD and write our first test `__tests__/RandomJoke_test.re`:

```ocaml

open Jest;

describe("<RandomJoke />", () => {
  open ExpectJs;

  test("render", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomJoke />);
    expect(Js.Undefined.return(component)) |> toBeDefined;
  });
});
```

We should create the component `src/RandomJoke.re`:

```ocaml
let component = ReasonReact.statelessComponent("RandomJoke");

let make = (_) => {
  ...component,
  render: _self => <div> {ReasonReact.string("todo")} </div>
};
```

Some notes:
- In OCaml/Reason all functions should be wrap into a modules `module MyModule = {...}`, but by default files are map to a module, so we don't need code it.
- In ReasonReact, instead of passing the whole "class" ReasonReact.createElement function, you'd instead declare a make function, it will be desugared to something like `ReasonReact.element(RandomJoke.make(...))`

And run the test:

```bash
npm test
 PASS  __tests__/RandomJoke_test.bs.js
  <RandomJoke />
    ✓ render (5ms)

Test Suites: 1 passed, 1 total
Tests:       1 passed, 1 total
Snapshots:   0 total
Time:        0.97s, estimated 1s
Ran all test suites.
```
Great! Now we are ready to add some production code!

### Design the component

Before start adding more tests, we could do some TDD inside-out, so we will think about the big picture and the possible solution instead using tests to drive us through the design process.

If we think in our component, we could think about it as an state machine that stores the status of the component and can change it depending on the action it receives.

<p align="center">
  <img src="state-diagram.png">
</p>

States:
- Loading
- Show
- Error

Actions:
- FetchJoke
- JokeFetched
- ErrorFetchingJoke

Translated to some code:

```ocaml
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
```
ReasonML let us create [sum types](https://en.wikipedia.org/wiki/Algebraic_data_type) with pipe operator; they call them [variants](https://reasonml.github.io/docs/en/variant).

### Create a stateful component

Since we want to show a remote joke, our first state should will be `Loading` and what we want to see is a loading message while we are fetching the data. To make it more interesting we will pass our loading message to the component `__tests__/RandomJoke_test.re`.

First, as following TDD approach `red`, `green`, `refactor`, we should write some test an make it `RED`:

```ocaml
 open ReactTestRenderer;
 ...
  test("snapshot while loading", () => {
    let json = toJSON(create(<RandomJoke />));
    expect(json)  |> toMatchSnapshot;
  });

  test("snapshot while loading changing the default message", () => {
    let component = toJSON(create(<RandomJoke loadingMessage="Wait for the joke ..."/>));
    expect(component) |> toMatchSnapshot;
  });
```

Second, think about how to make our tests pass and make them `GREEN`: 
```ocaml
let component = ReasonReact.statelessComponent("RandomJoke");

let make = (~loadingMessage="loading ...", _children) => {
  ...component,
  render: _self => <div> (ReasonReact.string(loadingMessage)) </div>
};
```
And finally, `REFACTOR` improving the existing implementation, in my personal opinion, a code that you could commit.

```ocaml
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
  reducer: ((), _) => ReasonReact.NoUpdate,,
  render: self => switch self.state {
    | Loading => <div> (ReasonReact.string(loadingMessage)) </div>
    | Show(_joke) => <div> (ReasonReact.string("TODO")) </div>
    | Error(_error) => <div> (ReasonReact.string("TODO")) </div>
  }
};
```
Some comments:
- `ReasonReact.reducerComponent` define a [stateful component](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer) and it comes with "reducer" (like Redux) built in. So for small apps we can use it, for big apps there are some libraries like [reductive](https://github.com/reasonml-community/reductive).
- If you are not familiar with the term reducers, they specify how the application's state changes in response to actions sent. Remember that actions only describe what happened, but don't describe how the application's state changes.
- `(~loadingMessage="loading ...", _children)`: Props are just the labeled arguments that we specify with `~` and `_children` just should be specified always as last argument, adding `_` in order to skip warnings in compile time. 
- We have added all the states in render function to the code in order to skip all the compilation warnings that reason gives us about the pattern-matching exhaustiveness.

### Fetching data

The next step is interact with the external API, what would be need to do that?

- A json encode/decoder [bs-json](https://github.com/glennsl/bs-json)
- An http client [bs-fetch](https://github.com/reasonml-community/bs-fetch)

Follow the instructions to install.

_Note_: For now on, for the sake of the tutorial, we understand we are all doing TDD and we will skip the tests and just add the production code here, the tests are in `_tests_/`, take a look if you want.

Taking a look on the response that we get from the external API:
```json
{
  "id": "R7UfaahVfFd",
  "joke": "My dog used to chase people on a bike a lot. It got so bad I had to take his bike away.",
  "status": 200
}
```
Here we have our joke decoder, that parses the response to a single string joke:
```ocaml
type joke = string;

module Decode = {
  let joke = json: joke => Json.Decode.(field("joke", string, json));
};
```
Then, we have to create the function to call to the real API using `bs-fetch` http client:

```bash
open Js.Promise

let fetchJoke = () =>
    Fetch.fetchWithInit(
      "https://icanhazdadjoke.com/",
      Fetch.RequestInit.make(~headers=Fetch.HeadersInit.make({"Accept": "application/json"}),())
      )
    |> then_(Fetch.Response.json)
    |> then_(json => json |> Decode.joke |> (joke => Some(joke) |> resolve))
    |> catch(_error => resolve(None));
```
Notes about the code:
- `open Js.Promise` opening the module let us to not use the prefixing for all the function calls like `then_`.
- `Fetch.RequestInit.make( ... ,())`, the last argument is the unit () at the end, just to tell reasonml that if we don't provide all arguments we are not currying the function.
- The operator `|>` is called reverse-application operator or pipe operator. It lets you chain function calls: x |> f is the same as f(x). It is useful to combine functions. 
- The result type is `option`, that only accepts Some or None and we don't need to specify the return type because is inferred by the compiler. 


### The reducer

As we said before, [the reducer](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer#actions-reducer) will be the one that centralizes all the component state updates.

Checking the sourcecode, the `reducer` is just a function:
 ```ocaml
 ('action, 'state) => update('state, 'retainedProps, 'action)`
 ```
 Where `'` is the way to create generic types in reasonml and the result type `update` is also another generic type:

 ```ocaml
update('state, 'retainedProps, 'action) =
  | NoUpdate
  | Update('state)
  | SideEffects(self('state, 'retainedProps, 'action) => unit)
  | UpdateWithSideEffects(
      'state,
      self('state, 'retainedProps, 'action) => unit,
    )
 ```

 So here we have the reducer function:

 ```ocaml
let createReducer = (fetch) => (action, _state) => switch action {
  | FetchJoke => ReasonReact.UpdateWithSideEffects(Loading, 
                 component => fetch() |> then_(result =>
                        switch result {
                          | Some(joke) => resolve(component.send(JokeFetched(joke)))
                          | None => resolve(component.send(ErrorFetchingJoke))
                        }
                  )  |> ignore
      )
  | JokeFetched(joke) => ReasonReact.Update(Show(joke))
  | ErrorFetchingJoke => ReasonReact.Update(Error("Error fetching a joke, try again ..."))
  }
 ```
Behavior:
1. This function will be called each time `self.send` with an action is called. Again, the infer system will fail if we try to call it with a type that doesn't match.
2. As a result type the function will update the state trough any of the functions provided.
3. If we have updated the state, under the hood the component will call setState(..) that will batch the changes and re-render the component.

Comments:
- We have curryfied the function to pass it the fetch function as a form of dependency injection, allowing us to create the real reducer.
- `switch` statement, reason as a cousin of OCaml (funtional language), provide us with a powerful mechanism called _pattern matching_, and we will use it as much as possible because **It is NOT a switch-case**
- The compiler will complain if we don't use the result type of a funtion, so we should use `|> ignore`.
- Maybe it seems to be a dynamic language because we are not providing the argument types in the function `(action, _state) => ...`, but again here the system is intelligent enough to infer the types.

### Rendering

Finally the last step to complete the first iteration is enrich the render function and put all the code together:

```ocaml
open Js.Promise

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
      Fetch.RequestInit.make(~headers=Fetch.HeadersInit.make({"Accept": "application/json"}),())
      )
    |> then_(Fetch.Response.json)
    |> then_(json => json |> Decode.joke |> (joke => Some(joke) |> resolve))
    |> catch(_error => resolve(None));

let component = ReasonReact.reducerComponent("RandomJoke");

let createReducer = (fetch) => (action, _state) => switch action {
  | FetchJoke => ReasonReact.UpdateWithSideEffects(Loading, 
                 component => fetch() |> then_(result =>
                        switch result {
                          | Some(joke) => resolve(component.send(JokeFetched(joke)))
                          | None => resolve(component.send(ErrorFetchingJoke))
                        }
                  )  |> ignore
      )
  | JokeFetched(joke) => ReasonReact.Update(Show(joke))
  | ErrorFetchingJoke => ReasonReact.Update(Error("Error fetching a joke, try again ..."))
  }

let make = (~loadingMessage="loading ...", ~fetch = fetchJoke, _children) => {
  ...component,
  initialState: () => Loading,
  reducer: createReducer(fetch),
  didMount: self => self.send(FetchJoke),
  render: self => switch self.state {
    | Loading => <div> (ReasonReact.string(loadingMessage)) </div>
    | Show(joke) => <div> (ReasonReact.string(joke)) </div>
    | Error(error) => <div> (ReasonReact.string(error)) </div>
  }
};
```

### Important design note

 Since this is a tutorial and we are generating a simple html we only have one component mixing all the application concerns. For a more complicated examples we would suggest different alternatives:
1. Decouple behaviour and fetching from html generation in two diferent components using Presentational and Container Components
2. If we want to reuse code across diferent components, use a render prop or HoC patterns to generalize fetching.
3. If the application gets bigger and complex use another approach to handle a global state.

### Second iteration

TODO
